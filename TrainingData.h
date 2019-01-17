#ifndef CARBONCOPYSIGNATUREMATCHING_TRAININGDATA_H
#define CARBONCOPYSIGNATUREMATCHING_TRAININGDATA_H

#include <string>
#include <vector>

using namespace std;

typedef vector<vector<double> > matrix;


struct DataPiece {
    vector<double> X;
    vector<double> Y;
    vector<pair<int, int>> originalTrack;

    DataPiece(vector<double> X, vector<pair<int, int>> originalTrack) {
        this->X = X;
        this->originalTrack = originalTrack;
    }

    DataPiece(vector<double> X, vector<double> Y, vector<pair<int, int>> originalTrack) {
        this->X = X;
        this->Y = Y;
        this->originalTrack = originalTrack;
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

    /*
     * Training Data Unit
     * generates training data from signature data points
     * handles test and training data collections
     */

    TrainingData(string path, int resolution);
    explicit TrainingData(int resolution); //Empty training data object //TODO refactor

    //shuffle training data vector
    void shuffleTrainingData();

    //augment data using image processing techniques
    void augmentTrainingData(int times);

    //assigning last X data pieces for testing
    void splitTestTrainingData(float trainingProportion, bool printNotification);

    //Iterator
    DataPiece getNextTrainingDataPiece();
    DataPiece getNextTestDataPiece();
    float getTrainingProgress();
    float getTestProgress();
    int getTrainingSampleSize();
    int getTestSampleSize();

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
    void rotateTrack(vector<pair<int, int>> &track, double degree);
    void sheerTrack(vector<pair<int, int>> &track);

    vector<string> getFilesInDirectory(string path, string extension);
    bool hasFileEnding(string &file, string &ending);

};

#endif //CARBONCOPYSIGNATUREMATCHING_TRAININGDATA_H
