#ifndef CARBONCOPYSIGNATUREMATCHING_NET_H
#define CARBONCOPYSIGNATUREMATCHING_NET_H

#include <sstream>
#include <vector>
#include "Neuron.h"

using namespace std;

class Net {
private:
    vector<Layer> m_layers; //m_layers[layerNum][neuronNum]
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

    void feedForward(const vector<double> &inputVals);

    void backPropagate(const vector<double> &targetVals);
    void getResults(vector<double> &resultVals) const;
    stringstream getNetStructure() const;
    void printNetworkStructureVisualization();
    double getRecentAverageError() { return m_recentAverageError;}
    vector<unsigned> getTopology();
private:
    bool loadWeightsFromSource(string &path, vector<unsigned> &topology, vector<vector<vector<double>>> &weight_cube);

};


#endif //CARBONCOPYSIGNATUREMATCHING_NET_H
