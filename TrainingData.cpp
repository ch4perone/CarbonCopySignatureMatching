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


TrainingData::TrainingData(int resolution) {
    this->resolution = resolution;
}

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
                DataPiece dp = generateDataPiece(track);
                dp.setY(Y);
                assert(dp.X.size() == resolution * 2);
                trainingPieces.push_back(dp);
            }
        }
    }

}



DataPiece TrainingData::generateDataPiece(vector<pair<int, int>> &track) {
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
            filenames.push_back(path+ "/" +name);
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

string TrainingData::getMetaInfo(int finalEpoch) {
    //TODO
    return "";
}

int TrainingData::getTrainingSampleSize() {
    return trainingPieces.size();
}

int TrainingData::getTestSampleSize() {
    return testPieces.size();
}

void TrainingData::augmentTrainingData(int times) {
    //fields
    double maxDegree = 5;


    int trainingInitSize = static_cast<int>(trainingPieces.size());
    for (int i = 0; i < trainingInitSize; ++i) {
        for (int k = 0; k < times; ++k) {
            DataPiece ori = trainingPieces[i];
            vector<pair<int, int>> track = ori.originalTrack;

            //TODO get random degree in range
            double degree;

            //rotateTrack(track, degree);
            //sheerTrack(track);

            DataPiece aug = generateDataPiece(track);
            aug.setY(ori.Y);
            assert(aug.X.size() == resolution * 2);
            trainingPieces.push_back(aug);
        }
    }
}
