#ifndef CARBONCOPYSIGNATUREMATCHING_TRAININGDATA_H
#define CARBONCOPYSIGNATUREMATCHING_TRAININGDATA_H

#include <string>
#include <vector>

using namespace std;

typedef vector<vector<double> > matrix;


struct DataPiece {
    vector<double> X;
    vector<double> Y;

    DataPiece(vector<double> X, vector<double> Y) {
        this->X = X;
        this->Y = Y;
    }
};


class TrainingData
{
public:
    TrainingData(vector<string> filename_v);
    void getTopology(vector<unsigned> &topology);

    unsigned getNextInputs(vector<double> &inputVals);

    unsigned getTargetOutputs(vector<double> &targetOutputVals);
    vector<string> m_trainingDataPaths;
    int c_inputNum;
    int total_inputNum;
    //vector<ifstream*> m_trainingDataFiles;
    vector<matrix> input_cube;

    /*
     * Training Data Unit
     * generates training data from signature data points
     * handles test and training data collections
     */

    TrainingData(string path, int resolution);
    ~TrainingData();

    void shuffleTrainingData();
    void splitTestTrainingData(float trainingProporting);
    DataPiece getNextTrainingDataPiece();
    DataPiece getNextTestDataPiece();
    float getTrainingProgress();
    float getTestProgress();
    void flushTrainingProgressToConsole(int currentEpoch, int maxEpoch);

    void restartTraining();
    void restartTesting();


    vector<DataPiece> trainingPieces;
    vector<DataPiece> testPieces;

private:
    //Preprocess input line according to network resolution and generate DataPiece object
    DataPiece generateDataPiece(string line);

    int resolution;
    int trainingIndex = 0;
    int testIndex = 0;
};

#endif //CARBONCOPYSIGNATUREMATCHING_TRAININGDATA_H
