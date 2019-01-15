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
        cout << files[i] << endl;
        //init output vector
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


TrainingData::~TrainingData() {
    //Maybe clean up all DataPieces
}

void TrainingData::getTopology(vector<unsigned> &topology) {
    topology.push_back(18);
    topology.push_back(20);
    topology.push_back(10);
    topology.push_back(5);
    /*string line;
    string label;

    getline(m_trainingDataFile, line);
    stringstream ss(line);

    ss >> label;

    if(this->isEOF() || label.compare("topology:") != 0)
        abort(); //TODO actually abort()
    while(!ss.eof()) {
        unsigned n;
        ss >> n;
        topology.push_back(n);
    }

    return;
*/
}


unsigned TrainingData::getNextInputs(vector<double> &inputVals) {
    inputVals.clear();


    //cout << (double) m_trainingDataFiles.size() * rand() / (double) RAND_MAX << endl;
    c_inputNum = (double) (total_inputNum + 1) * rand() / (double) RAND_MAX; //+1 due to trash
    int c_inputLine = (double) input_cube[c_inputNum].size() * rand() / (double) RAND_MAX;



    inputVals = input_cube[c_inputNum][c_inputLine];

    return inputVals.size();
}

unsigned TrainingData::getTargetOutputs(vector<double> &targetOutputVals) {
    targetOutputVals.clear();

    for (int i = 0; i < total_inputNum; ++i) {
        double targetVal = i == c_inputNum ? 1 : 0;
        targetOutputVals.push_back(targetVal);
    }
    return (unsigned int) targetOutputVals.size();

}

DataPiece TrainingData::generateDataPiece(vector<pair<int, int>> &track) {
    /*
     * Crucial algorithm to extract direction vectors from pixel tracks
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


    /*
    for (int i = steps; i < track.size(); i+=steps) {
        pair<int, int> coordinate2 = track[i];
        pair<double, double> vec = normalizedDirectionVector(coordinate1, coordinate2);

        inputVector.push_back(vec.first);
        inputVector.push_back(vec.second);

        coordinate1 = coordinate2;
    }

    if (track.size() % resolution == 0) {
        pair<int, int> coordinate2 = track.back();
        pair<double, double> vec = normalizedDirectionVector(coordinate1, coordinate2);

        inputVector.push_back(vec.first);
        inputVector.push_back(vec.second);
    }

    if (inputVector.size() == 2*resolution + 2) { //todo find out why sometimes 1 vector / 2 coord too many
        inputVector.pop_back();
        inputVector.pop_back();
    }*/

    return DataPiece(inputVector);
}

void TrainingData::shuffleTrainingData() {
    random_shuffle(trainingPieces.begin(), trainingPieces.end());
}

void TrainingData::splitTestTrainingData(float trainingProportion) {
    //TODO now
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
    cout << "interpolate" << endl;
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
    /*for(auto& p: experimental::filesystem::recursive_directory_iterator(path))
    {
        if(p.path().extension() == extension)
            std::cout << p << '\n';
    }
    return vector<string>();*/
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
