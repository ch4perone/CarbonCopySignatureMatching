#include <cassert>
#include <cmath>
#include <iostream>
#include <fstream>
#include "Net.h"


Net::Net(int resolution, int numClasses, int width, int depth) {
    vector<unsigned> topology(depth + 2, width);
    topology[0] = resolution * 2;
    topology[topology.size() - 1] = numClasses;
    this->topology = topology;

    auto numLayers = static_cast<unsigned int>(topology.size());

    for (int indexLayer = 0; indexLayer < numLayers; ++indexLayer) {
        m_layers.emplace_back();
        int numOutputs = indexLayer == topology.size() -1 ? 0 : topology[indexLayer+1];

        for (int indexNeuron = 0; indexNeuron <= topology[indexLayer]; ++indexNeuron) {
            m_layers.back().push_back(Neuron(numOutputs, indexNeuron));
        }
        m_layers.back().back().setOutputVal(1.0);
    }

}

Net::Net(string source) {

    vector<unsigned> topology;
    vector<vector <vector<double> > > weight_cube;
    if(!loadWeightsFromSource(source, topology, weight_cube)) {
        cout << "Error: Loading network from source " << source << endl;
        exit(1);
    }


    this->topology = topology;
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

Net::Net(vector<unsigned> &topology) {
    this->topology = topology;
    auto numLayers = static_cast<unsigned int>(topology.size());

    for (unsigned indexLayer = 0; indexLayer < numLayers; ++indexLayer) {
        m_layers.push_back(Layer());
        unsigned numOutputs = indexLayer == topology.size() -1 ? 0 : topology[indexLayer+1];

        for (int indexNeuron = 0; indexNeuron <= topology[indexLayer]; ++indexNeuron) {
            m_layers.back().push_back(Neuron(numOutputs, indexNeuron));
        }
        m_layers.back().back().setOutputVal(1.0);
    }
}

Net::Net(vector<unsigned> &topology, vector<vector<vector<double> > > &weight_cube) {
    this->topology = topology;
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

void Net::backPropagate(const vector<double> &targetVals) {
    //Calculate overall net error (RMS of output neuron errors)
    Layer &outputLayer = m_layers.back();
    m_error  = 0.0;

    for (int n = 0; n < outputLayer.size() - 1; ++n) {
        double delta = targetVals[n] - outputLayer[n].getOutputVal();
        m_error += delta * delta;
    }

    m_error /= outputLayer.size() - 1;
    m_error = (double) sqrt(m_error); //RMS
    cout << m_error << endl;
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

void Net::printNetworkStructureVisualization() {
    string dots = "\t";
    string nums = "\t";
    for (int j = 0; j < topology.size(); ++j) {
        dots += ".    ";
    }
    dots+="\n";

    for (unsigned i : topology) {
        nums += to_string(i) + "----";
    }
    nums = nums.substr(0, nums.length() - 4) + "\n";

    cout << "Network structure:" << endl;
    cout << dots << dots << dots << nums << dots << dots << dots << endl;
}

bool Net::loadWeightsFromSource(string &path, vector<unsigned> &topology, vector<vector <vector<double> > > &weight_cube) {
    ifstream f(path, ios::in);



    //get topology
    string line;
    string label;

    getline(f, line);
    stringstream ss1(line);

    ss1 >> label;

    if(f.eof() || label.compare("topology:") != 0)
        return false;
    while(!ss1.eof()) {
        double n;
        ss1 >> n;
        topology.push_back(n);
    }

    //get weights for every neuron
    int i,j;
    vector<vector<double> > weight_layer_matrix;
    for(int i = 0, j = 0; i < topology.size() - 1; ++j) {
        getline(f, line);
        stringstream ss2(line);

        ss2 >> label;

        string expected_label = "[";
        expected_label+=to_string(i);
        expected_label+=",";
        expected_label+=to_string(j);
        expected_label+="]";

        if(f.eof() || label.compare(expected_label) != 0) {
            return false;
        //abort();
        }


        double c_weight;
        vector<double> weights_of_c_neuron;
        while(ss2 >> c_weight) {
            weights_of_c_neuron.push_back(c_weight);
        }

        if(weights_of_c_neuron.size() != topology[i+1])
            return false;

        weight_layer_matrix.push_back(weights_of_c_neuron);

        if(j == topology[i]) {
            weight_cube.push_back(weight_layer_matrix);
            weight_layer_matrix.clear();
            j = -1;
            ++i;
        }
    }
    f.close();

    return true;

    //net = new Net(topology, weight_cube);

}

vector<unsigned> Net::getTopology() {
    return topology;
}
