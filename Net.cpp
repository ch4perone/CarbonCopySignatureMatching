#include <cassert>
#include <cmath>
#include "Net.h"


Net::Net(vector<unsigned> &topology, vector<vector<vector<double> > > &weight_cube) {

    unsigned numLayers = (unsigned int) topology.size();

    for (unsigned indexLayer = 0; indexLayer < numLayers; ++indexLayer) {
        m_layers.push_back(Layer());
        unsigned numOutputs = indexLayer == topology.size() -1 ? 0 : topology[indexLayer+1];

        for (int indexNeuron = 0; indexNeuron <= topology[indexLayer]; ++indexNeuron) {
        m_layers.back().push_back(Neuron(numOutputs, indexNeuron, weight_cube[indexLayer][indexNeuron]));
        }
        m_layers.back().back().setOutputVal(1.0);
    }
}

void Net::feedForward(const vector<double> &inputVals) {
    assert(inputVals.size() == m_layers[0].size() - 1);

    for (int i = 0; i < inputVals.size(); ++i) {
        m_layers[0][i].setOutputVal(inputVals[i]);
    }

    //Forward propagate
    for (int indexLayer = 1; indexLayer < m_layers.size(); ++indexLayer) {
        Layer &prevLayers = m_layers[indexLayer - 1];
        for (int n = 0; n < m_layers[indexLayer].size() - 1; ++n) {
            m_layers[indexLayer][n].feedForward(prevLayers);
        }
    }
}

void Net::backProp(const vector<double> &targetVals) {
    //Calculate overall net error (RMS of output neuron errors)
    Layer &outputLayer = m_layers.back();
    m_error  = 0.0;

    for (int n = 0; n < outputLayer.size() - 1; ++n) {
        double delta = targetVals[n] - outputLayer[n].getOutputVal();
        m_error += delta * delta;
    }

    m_error /= outputLayer.size() - 1;
    m_error = (double) sqrt(m_error); //RMS

    //Give an insight on average error
    m_recentAverageError = (m_recentAverageError * m_recentAverageSmoothingFactor + m_error)/(m_recentAverageSmoothingFactor + 1);


    //Calculate the output layer gradients
    for (unsigned n = 0; n < outputLayer.size() - 1; ++n) {
        outputLayer[n].calcOutputGradients(targetVals[n]);
    }

    //Calculate gradients on hidden layers
    for (unsigned indexLayer = m_layers.size() - 2; indexLayer > 0; --indexLayer) {
        Layer &hiddenLayer = m_layers[indexLayer];
        Layer &nextLayer = m_layers[indexLayer + 1];


        for (unsigned n = 0; n < hiddenLayer.size(); ++n) {
            hiddenLayer[n].calcHiddenGradients(nextLayer);
        }
    }

    //For all layers from output to first hidden Layer
    //Update conection weight

    for (int indexLayer = m_layers.size() - 1; indexLayer > 0; --indexLayer) {

        Layer &layer = m_layers[indexLayer];
        Layer &prevLayer = m_layers[indexLayer - 1];

        for (int n = 0; n < layer.size(); ++n) {
            layer[n].updateInputWeights(prevLayer);
        }
    }

}

void Net::getResults(vector<double> &resultVals) const {
    resultVals.clear();

    //cout << "*********************************" << endl;
    for (int n = 0; n < m_layers.back().size() - 1; ++n) {
        resultVals.push_back(m_layers.back()[n].getOutputVal());
        //cout << m_layers.back()[n].getOutputVal() << endl;

    }

}

stringstream Net::getNetStructure() const {
    stringstream ss_topology, ss_neurons;
    ss_topology << "topology:";
    for(int i = 0; i < m_layers.size(); ++i) {
        ss_topology << " " << m_layers[i].size() - 1;
        if (i == m_layers.size() - 1)
            break;
        for (int j = 0; j < m_layers[i].size(); ++j) {
            ss_neurons << "[" << i << "," << j << "]";
            const Neuron *c_neuron = &m_layers[i][j];
            for(Connection c : c_neuron->m_outputsWeights) {
                ss_neurons << " " << c.weight;
            }
            ss_neurons << endl;
        }
    }
    ss_topology << endl << ss_neurons.str();
    return ss_topology;
}

