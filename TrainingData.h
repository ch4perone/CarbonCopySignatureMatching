#ifndef CARBONCOPYSIGNATUREMATCHING_TRAININGDATA_H
#define CARBONCOPYSIGNATUREMATCHING_TRAININGDATA_H

#include <string>
#include <vector>

using namespace std;

typedef vector<vector<double> > matrix;


struct DataPiece {
    vector<double> X;
    vector<double> Y;
    vector<pair<double, double>> originalTrack;
    bool isAugmented = false;

    DataPiece(vector<double> X, vector<pair<double, double>> originalTrack) {
        this->X = X;
        this->originalTrack = originalTrack;
    }

    DataPiece(vector<double> X, vector<double> Y, vector<pair<double, double>> originalTrack) {
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

    int trainingInitSize;
    bool augmented = false;
    static double maxAugmentingRotation;
    static double maxAugmentingShearFactor;

public:

    /*
     * Training Data Unit
     * generates training data from signature data points
     * handles test and training data collections
     */

    TrainingData(string path, int resolution);

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
    string getMetaInfo(int finalEpoch, double finalTestLoss);

    void restartTraining();
    void restartTesting();
    vector<string> getFiles();

    /*
     * Static: Pre-processing of pixel track according to network resolution and generation DataPiece object
     */

    static DataPiece generateDataPiece(vector<pair<double, double>> &track, int resolution);
    static DataPiece augmentDataPiece(DataPiece ori, int resolution);

private:
    static pair<double, double> normalizedDirectionVector(pair<double, double> coordinate1, pair<double, double> coordinate2);
    static void interpolateTrack(vector<pair<double, double>> &track);
    static void rotateTrack(vector<pair<double, double>> &track, double angle);
    static void shearTrack(vector<pair<double, double>> &track, pair<double, double> &dir);

    static void rotateVector(pair<double, double> &v, double angle);

    vector<string> getFilesInDirectory(string path, string extension);
    bool hasFileEnding(string &file, string &ending);

};

#endif //CARBONCOPYSIGNATUREMATCHING_TRAININGDATA_H
