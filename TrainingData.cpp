#include <sstream>
#include <fstream>
#include <algorithm>
#include <iostream>
#include "TrainingData.h"


TrainingData::TrainingData(string path, int resolution) {
    //todo: load all files of type .signature.tsv
    vector<string> files;
    this->resolution = resolution;

    for(string file : files) {
        ifstream f(file, ios::in);
        while(!f.eof()) {
            string line;
            getline(f, line);

            DataPiece dp = generateDataPiece(line);
            trainingPieces.push_back(dp);
        }
    }

}

TrainingData::~TrainingData() {
    //Maybe clean up all DataPieces
}

TrainingData::TrainingData(const vector<string> filename_v) {
    total_inputNum = (int) filename_v.size() - 1;
    for (string path : filename_v) {
        m_trainingDataPaths.push_back(path);
        ifstream f(path, ios::in);
        matrix input_matrix;
        while (!f.eof()) {
            vector<double> input_line;

            string line;
            getline(f, line);

            stringstream ss(line);


            double oneValue;
            while(ss >> oneValue) {
                input_line.push_back(oneValue);
            }
            input_matrix.push_back(input_line);
        }
        input_matrix.pop_back(); //Remove empty line
        input_cube.push_back(input_matrix);
    }
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

DataPiece TrainingData::generateDataPiece(string line) {
    //Todo parse data piece
    return DataPiece({1.2,1.2},{1.2, 1.2});
}

void TrainingData::shuffleTrainingData() {
    random_shuffle(trainingPieces.begin(), trainingPieces.end());
}

void TrainingData::splitTestTrainingData(float trainingProporting) {
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





void TrainingData::flushTrainingProgressToConsole(int currentEpoch, int maxEpoch) {
    cout << "\r[epoch " << currentEpoch << "/" << maxEpoch << "]" << " progress: " << getTrainingProgress() << flush;
    //TODO make fancy progress bar
}
