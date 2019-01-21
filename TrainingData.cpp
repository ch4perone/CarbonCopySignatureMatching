#include <sstream>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <experimental/filesystem>
#include <dirent.h>
#include <sys/types.h>
#include <cassert>
#include "TrainingData.h"

double TrainingData::maxAugmentingRotation = 10;
double TrainingData::maxAugmentingShearFactor = 0.2f;

TrainingData::TrainingData(string path, int resolution) {
    files = getFilesInDirectory(path, ".sgn.tsv");
    sort(files.begin(), files.end());
    
    this->resolution = resolution;

    for(int i = 0; i < files.size(); ++i) {

        //init ground truth output vector
        vector<double> Y(files.size(), 0);
        Y[i] = 1.f;

        string file = files[i];
        ifstream f(file, ios::in);
        while(!f.eof()) {
            string line;
            getline(f, line);

            stringstream ss(line);
            vector<pair<int, int>> track;
            int x,y;
            while(ss >> x >> y) {
                track.emplace_back(x,y);
            }
            if(!track.empty()) { //Drop empty lines
                DataPiece dp = generateDataPiece(track, resolution);
                dp.setY(Y);
                assert(dp.X.size() == resolution * 2);
                trainingPieces.push_back(dp);
            }
        }
    }

}



DataPiece TrainingData::generateDataPiece(vector<pair<int, int>> &track, int resolution) {
    /*
     * Crucial algorithm to extract direction vectors from pixel tracks
     *      Select pixel coordinates according to network resolution
     *      Connect selected coordinates by a direction vector
     *      Save normalized direction vector features as data piece
     */

    vector<double> inputVector;
    while (track.size() < 2*resolution) {
        interpolateTrack(track);
    }

    //Begin connect first coordinate
    pair<int, int> coordinate1 = track[0];
    int steps = static_cast<int>(track.size() / resolution);

    //Connect intermediate coordinate iteratively
    for (int i = 1; i < resolution; ++i) {
        pair<int, int> coordinate2 = track[i*steps];
        pair<double, double> vec = normalizedDirectionVector(coordinate1, coordinate2);

        inputVector.push_back(vec.first);
        inputVector.push_back(vec.second);

        coordinate1 = coordinate2;
    }

    //Connect with last coordinate
    pair<int, int> coordinate2 = track.back();
    pair<double, double> vec = normalizedDirectionVector(coordinate1, coordinate2);
    inputVector.push_back(vec.first);
    inputVector.push_back(vec.second);

    return DataPiece(inputVector, track);
}

void TrainingData::shuffleTrainingData() {
    random_shuffle(trainingPieces.begin(), trainingPieces.end());
}

void TrainingData::splitTestTrainingData(float trainingProportion, bool printNotification) {
    int testSampleSize = (int) ceil(trainingPieces.size() * (1 - trainingProportion));
    for (int i = 0; i < testSampleSize; ++i) {
        DataPiece testDP = trainingPieces.back();
        trainingPieces.pop_back();
        testPieces.push_back(testDP);
    }
    if (printNotification) {
        cout << "Training data split into " << int(trainingProportion * 100) << "% (" << getTrainingSampleSize() << ") for training and " << int((1 - trainingProportion) * 100) << "% (" << getTestSampleSize() << ") for testing" << endl;
    }
}

DataPiece TrainingData::getNextTrainingDataPiece() {
    DataPiece dp = trainingPieces[trainingIndex];
    ++trainingIndex;
    return dp;
}

DataPiece TrainingData::getNextTestDataPiece() {
    DataPiece dp = testPieces[testIndex];
    ++testIndex;
    return dp;
}

float TrainingData::getTrainingProgress() {
    return float(trainingIndex) / float(trainingPieces.size());
}

float TrainingData::getTestProgress() {
    return float(testIndex) / float(testPieces.size());
}

void TrainingData::restartTraining() {
    trainingIndex = 0;
}


void TrainingData::restartTesting() {
    testIndex = 0;
}

void TrainingData::flushTrainingProgressToConsole(int currentEpoch, int maxEpoch, double recentLoss) {
    cout << "\r[epoch " << currentEpoch << "/" << maxEpoch << "]" << " progress: " << int(getTrainingProgress() * 100) << "% " << "loss: " << recentLoss <<  flush;
    //TODO make fancy progress bar
}

void TrainingData::interpolateTrack(vector<pair<int, int>> &track) {
    vector<pair<int, int>> interpolation;

    for (int i = 0; i < track.size() - 1; ++i) {
        pair<int, int> coordinate1 = track[i];
        pair<int, int> coordinate2 = track[i+1];
        pair<int, int> inbetween = {(coordinate1.first + coordinate2.first) / 2, (coordinate1.second + coordinate2.second) / 2};
        interpolation.push_back(coordinate1);
        interpolation.push_back(inbetween);
    }
    interpolation.push_back(track.back());
    track = interpolation;

}

pair<double, double> TrainingData::normalizedDirectionVector(pair<int, int> coordinate1, pair<int, int> coordinate2) {
    double x = coordinate2.first - coordinate1.first;
    double y = coordinate2.second - coordinate1.second;
    double length = sqrt(x*x + y*y);
    x /= length;
    y /= length;
    return {x, y};
}

vector<string> TrainingData::getFilesInDirectory(string path, string extension) {
    vector<string> filenames;

    struct dirent *entry;
    DIR *dir = opendir(path.c_str());

    if (dir == NULL) {
        cout << "Error loading files from: " << path << endl;
        exit(1);
    }
    while ((entry = readdir(dir)) != NULL) {
        string name(entry->d_name);
        if (hasFileEnding(name, extension))
            filenames.push_back(path + "/" + name);
    }
    closedir(dir);

    return filenames;
}

bool TrainingData::hasFileEnding(string &file, string &ending) {
    if (ending.size() > file.size()) return false;
    return std::equal(ending.rbegin(), ending.rend(), file.rbegin());
}

vector<string> TrainingData::getFiles() {
    return files;
}

string TrainingData::getMetaInfo(int finalEpoch, double finalTestLoss) {
    string metaInfo = "#***Training info***\n#Training files: ";
    for (string &file : files) {
        metaInfo+=file + " ";
    }


    if(augmented) {
        metaInfo += "\n#Trained on initial number of " + to_string(trainingInitSize) + " data tracks. By data augmentation increased to a total number of " + to_string(trainingPieces.size()) + " data tracks.";
    } else {
        metaInfo += "\n#Trained on " + to_string(trainingPieces.size()) + " data tracks.";
    }
    metaInfo += "\n#Tested on " + to_string(testPieces.size()) + " data tracks.";
    metaInfo += "\n#Epochs trained: " + to_string(finalEpoch);
    metaInfo += "\n#Final test loss: " + to_string(finalTestLoss) + "\n#";

    return metaInfo;
}

int TrainingData::getTrainingSampleSize() {
    return (int) trainingPieces.size();
}

int TrainingData::getTestSampleSize() {
    return (int) testPieces.size();
}

void TrainingData::augmentTrainingData(int times) {
    augmented = true;
    trainingInitSize = static_cast<int>(trainingPieces.size());
    for (int i = 0; i < trainingInitSize; ++i) {
        DataPiece ori = trainingPieces[i];
        for (int k = 0; k < times; ++k) {

            DataPiece aug = augmentDataPiece(ori, resolution);
            aug.setY(ori.Y);
            assert(aug.X.size() == resolution * 2);

            trainingPieces.push_back(aug);
        }
    }
}

DataPiece TrainingData::augmentDataPiece(DataPiece ori, int resolution) {
    vector<pair<int, int>> track = ori.originalTrack;

    /*
     * Rotate track
     */

    double angle = 2 * maxAugmentingRotation * (double) rand() / (double) RAND_MAX - maxAugmentingRotation;
    rotateTrack(track, angle);

    /*
     * Shear track
     */

    angle = 360 * (double) rand() / (double) RAND_MAX;
    double shearFactor = maxAugmentingShearFactor * (double) rand() / (double) RAND_MAX;
    pair<double, double> dir = make_pair(0, shearFactor);
    rotateVector(dir, angle);
    shearTrack(track, dir);

    DataPiece aug = generateDataPiece(track, resolution);
    aug.setY(ori.Y);
    assert(aug.X.size() == resolution * 2);

    return aug;
}

void TrainingData::rotateTrack(vector<pair<int, int>> &track, double angle) {

    int center_x = 0;
    int center_y = 0;

    for (pair<int,int> v : track) {
        center_x+=v.first;
        center_y+=v.second;
    }

    center_x /= track.size();
    center_y /= track.size();

    for (pair<int,int> &v : track) {
        pair<int, int> dif;
        dif.first = v.first - center_x;
        dif.second = v.second - center_y;

        rotateVector(dif, angle);

        v.first = dif.first + center_x;
        v.second = dif.second + center_y;
    }
}

void TrainingData::rotateVector(pair<int, int> &v, double angle) {
    angle*= M_PI/ 180;

    double x,y;
    x = double(v.first) * cos(angle) - double(v.second) * sin(angle);
    y = double(v.first) * sin(angle) + double(v.second) * cos(angle);

    v.first = (int) x;
    v.second = (int) y;

}

void TrainingData::rotateVector(pair<double, double> &v, double angle) {
    angle*= M_PI/ 180;

    double x,y;
    x = v.first * cos(angle) - v.second * sin(angle);
    y = v.first * sin(angle) + v.second * cos(angle);

    v.first = x;
    v.second = y;

}

void TrainingData::shearTrack(vector<pair<int, int>> &track, pair<double, double> &dir) {
    pair<int,int> origin = track[0];
    for (pair<int, int> &v : track) {
        v.first += int(dir.first * v.second);
        v.second += int(dir.second * v.first);
    }

    pair<int, int> shift = make_pair(origin.first -track[0].first, origin.second - track[0].second);
    for (pair<int, int> &v : track) {
        v.first += shift.first;
        v.second += shift.second;
    }

}
