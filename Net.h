#ifndef CARBONCOPYSIGNATUREMATCHING_NET_H
#define CARBONCOPYSIGNATUREMATCHING_NET_H

#include <sstream>
#include <vector>
#include "Neuron.h"

using namespace std;

class Net {
public:
    Net(vector<unsigned> &topology);
    Net(vector<unsigned> &topology, vector<vector<vector <double> > > &weight_cube);
    void feedForward(const vector<double> &inputVals);
    void backProp(const vector<double> &targetVals);
    void getResults(vector<double> &resultVals) const;
    stringstream getNetStructure() const;
    double getRecentAverageError() { return m_recentAverageError;}
private:
    vector<Layer> m_layers; //m_layers[layerNum][neuronNum]
    double m_error;
    double m_recentAverageError;
    double m_recentAverageSmoothingFactor;
};

Net::Net(vector<unsigned> &topology) {
    unsigned numLayers = (unsigned int) topology.size();

    for (unsigned indexLayer = 0; indexLayer < numLayers; ++indexLayer) {
        m_layers.push_back(Layer());
        unsigned numOutputs = indexLayer == topology.size() -1 ? 0 : topology[indexLayer+1];

        for (int indexNeuron = 0; indexNeuron <= topology[indexLayer]; ++indexNeuron) {
            m_layers.back().push_back(Neuron(numOutputs, indexNeuron));
        }
        m_layers.back().back().setOutputVal(1.0);
    }
}

#endif //CARBONCOPYSIGNATUREMATCHING_NET_H
