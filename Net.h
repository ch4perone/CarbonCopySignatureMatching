#ifndef CARBONCOPYSIGNATUREMATCHING_NET_H
#define CARBONCOPYSIGNATUREMATCHING_NET_H

#include <sstream>
#include <vector>
#include "Neuron.h"

using namespace std;

class Net {
private:
    vector<Layer> m_layers;
    double m_error;
    double m_recentAverageError;
    double m_recentAverageSmoothingFactor;
    vector<unsigned> topology;

public:
    //initialize and compose (new) random weighted network
    Net(int resolution, int numClasses, int width, int depth);

    //initialize (new) random weighted network
    Net(vector<unsigned> &topology);

    //initialize network with preexisting weights
    Net(vector<unsigned> &topology, vector<vector<vector <double> > > &weight_cube);

    //load network from file
    explicit Net(string source);

    void feedForward(const vector<double> &inputValues);

    void backPropagate(const vector<double> &targetValues);
    void getOutput(vector<double> &outputValues) const;
    stringstream getNetStructure() const;
    void printNetworkStructureVisualization();
    double getRecentAverageError() { return m_recentAverageError;}
    vector<unsigned> getTopology();
private:
    static vector<double> softmax(const vector<double> &z);
    static double softmaxValue(const vector<double> &z, int j);
    double crossEntropyLoss(const vector<double> &targetValues, const vector<double> &estimatedValues);
    vector<double> deltaCrossEntropy(vector<double> &targetValues, vector<double> &estimatedValues);
    bool loadWeightsFromSource(string &path, vector<unsigned> &topology, vector<vector<vector<double>>> &weight_cube);

};


#endif //CARBONCOPYSIGNATUREMATCHING_NET_H
