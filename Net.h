#ifndef CARBONCOPYSIGNATUREMATCHING_NET_H
#define CARBONCOPYSIGNATUREMATCHING_NET_H

#include <sstream>
#include <vector>
#include "Neuron.h"

using namespace std;

class Net {
private:
    vector<Layer> layers;
    double rms_error;
    double recentAverageError;
    double recentAverageSmoothingFactor;
    vector<unsigned> topology;
    vector<string> outputNames;

public:
    //initialize and compose (new) random weighted network
    Net(int resolution, int numClasses, int width, int depth);

    //initialize (new) random weighted network
    explicit Net(vector<unsigned> &topology);

    //load network from file
    explicit Net(string source);

    //initialize network with topology and weights
    Net(vector<unsigned> &topology, vector<vector<vector <double> > > &weight_cube);

    void feedForward(const vector<double> &inputValues);
    void backPropagate(const vector<double> &targetValues);
    void getOutput(vector<double> &outputValues) const;
    double getLoss(const vector<double> &targetValues);

    bool saveNetworkToFile(string path, int argc, char** argv, string trainingMetaInfo);

    void printNetworkStructureVisualization();
    vector<unsigned> getTopology();
    vector<string> getOutputNames();

private:
    stringstream getNetStructure() const;
    double crossEntropyLoss(const vector<double> &targetValues, const vector<double> &estimatedValues);
    bool loadWeightsFromSource(string &path, vector<unsigned> &topology, vector<vector<vector<double>>> &weight_cube);

};


#endif //CARBONCOPYSIGNATUREMATCHING_NET_H
