#ifndef CARBONCOPYSIGNATUREMATCHING_TRAININGDATA_H
#define CARBONCOPYSIGNATUREMATCHING_TRAININGDATA_H

#include <string>
#include <vector>

using namespace std;

typedef vector<vector<double> > matrix;


struct DataPiece {
    vector<double> X;
    vector<double> Y;

    explicit DataPiece(vector<double> X) {
        this->X = X;
    }

    DataPiece(vector<double> X, vector<double> Y) {
        this->X = X;
        this->Y = Y;
    }

    void setY(vector<double> Y) {
        this->Y = Y;
    }

    void printContent() {
        cout.precision(1);
        cout << "X: ";
        for (double x : X) {
            cout << x << " ";
        }
        cout << ">> Y: ";
        for (double y : Y) {
            cout << y << " ";
        }
        cout << endl;
    }
};


class TrainingData
{
private:
    int resolution;
    int trainingIndex = 0;
    int testIndex = 0;

    vector<string> files;
    vector<DataPiece> trainingPieces;
    vector<DataPiece> testPieces;
public:

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

    explicit TrainingData(int resolution); //Empty training data object //TODO refactor


    ~TrainingData();
    void shuffleTrainingData();
    void splitTestTrainingData(float trainingProportion);
    DataPiece getNextTrainingDataPiece();
    DataPiece getNextTestDataPiece();
    float getTrainingProgress();
    float getTestProgress();

    void flushTrainingProgressToConsole(int currentEpoch, int maxEpoch, double recentLoss);
    string getMetaInfo(int finalEpoch);

    void restartTraining();
    void restartTesting();
    vector<string> getFiles();

    //Preprocess input line according to network resolution and generate DataPiece object
    DataPiece generateDataPiece(vector<pair<int, int>> &track);

private:
    pair<double, double> normalizedDirectionVector(pair<int, int> coordinate1, pair<int, int> coordinate2);
    void interpolateTrack(vector<pair<int, int>> &track);
    vector<string> getFilesInDirectory(string path, string extension);
    bool hasFileEnding(string &file, string &ending);

};

#endif //CARBONCOPYSIGNATUREMATCHING_TRAININGDATA_H
