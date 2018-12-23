#include <sstream>
#include <fstream>
#include "TrainingData.h"


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
