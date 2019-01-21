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
        layers.emplace_back();
        int numOutputs = indexLayer == topology.size() -1 ? 0 : topology[indexLayer+1];

        for (int indexNeuron = 0; indexNeuron <= topology[indexLayer]; ++indexNeuron) {
            layers.back().push_back(Neuron(numOutputs, indexNeuron));
        }
        layers.back().back().setOutputValue(1.0);
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
        layers.push_back(Layer());
        unsigned numOutputs = indexLayer == topology.size() -1 ? 0 : topology[indexLayer+1];

        for (int indexNeuron = 0; indexNeuron <= topology[indexLayer]; ++indexNeuron) {
            layers.back().push_back(Neuron(numOutputs, static_cast<unsigned int>(indexNeuron), weight_cube[indexLayer][indexNeuron]));
        }
        layers.back().back().setOutputValue(1.0);
    }
}

Net::Net(vector<unsigned> &topology) {
    this->topology = topology;
    auto numLayers = static_cast<unsigned int>(topology.size());

    for (unsigned indexLayer = 0; indexLayer < numLayers; ++indexLayer) {
        layers.push_back(Layer());
        unsigned numOutputs = indexLayer == topology.size() -1 ? 0 : topology[indexLayer+1];

        for (int indexNeuron = 0; indexNeuron <= topology[indexLayer]; ++indexNeuron) {
            layers.back().push_back(Neuron(numOutputs, indexNeuron));
        }
        layers.back().back().setOutputValue(1.0);
    }
}

Net::Net(vector<unsigned> &topology, vector<vector<vector<double> > > &weight_cube) {
    this->topology = topology;
    unsigned numLayers = (unsigned int) topology.size();

    for (unsigned indexLayer = 0; indexLayer < numLayers; ++indexLayer) {
        layers.push_back(Layer());
        unsigned numOutputs = indexLayer == topology.size() -1 ? 0 : topology[indexLayer+1];

        for (int indexNeuron = 0; indexNeuron <= topology[indexLayer]; ++indexNeuron) {
        layers.back().push_back(Neuron(numOutputs, indexNeuron, weight_cube[indexLayer][indexNeuron]));
        }
        layers.back().back().setOutputValue(1.0);
    }
}

void Net::feedForward(const vector<double> &inputValues) {
    assert(inputValues.size() == layers[0].size() - 1);

    //Set input values as output of first layer
    for (int i = 0; i < inputValues.size(); ++i) {
        layers[0][i].setOutputValue(inputValues[i]);
    }

    //Forward propagate
    for (int indexLayer = 1; indexLayer < layers.size(); ++indexLayer) {
        Layer &prevLayers = layers[indexLayer - 1];
        for (int n = 0; n < layers[indexLayer].size() - 1; ++n) {
            layers[indexLayer][n].feedForward(prevLayers);
        }
    }

    //Compute softmax activation in last layer
    vector<double> z; //vector of neuron input (z) before activation (a)
    Layer &outputLayer= layers.back();
    for (int i = 0; i < outputLayer.size() - 1; ++i) {
        z.push_back(outputLayer[i].getInputSum());
    }
    for (int j = 0; j < outputLayer.size() - 1; ++j) {
        outputLayer[j].computeSoftmaxActivation(z);
    }


}

void Net::backPropagate(const vector<double> &targetValues) {
    //Calculate overall net rms_error (RMS of output neuron errors)
    Layer &outputLayer = layers.back();
    rms_error  = 0.0;

    vector<double> estimatedValues;
    for (int n = 0; n < outputLayer.size() - 1; ++n) {
        estimatedValues.push_back(outputLayer[n].getOutputValue());
        //double delta = targetValues[n] - outputLayer[n].getOutputValue();
        //rms_error += delta * delta;
    }

    //rms_error /= outputLayer.size() - 1;
    //rms_error = sqrt(rms_error); //RMS
    //recentAverageError = (recentAverageError * recentAverageSmoothingFactor + rms_error)/(recentAverageSmoothingFactor + 1);


    //Calculate the output layer gradients from softmax
    vector<double> z; //vector of neuron input (z) before activation (a)
    for (int i = 0; i < outputLayer.size() - 1; ++i) {
        z.push_back(outputLayer[i].getInputSum());
    }
    for (unsigned n = 0; n < outputLayer.size() - 1; ++n) {
        outputLayer[n].calcOutputGradients(targetValues[n], z);
    }

    //Calculate gradients on hidden layers
    for (unsigned indexLayer = layers.size() - 2; indexLayer > 0; --indexLayer) {
        Layer &hiddenLayer = layers[indexLayer];
        Layer &nextLayer = layers[indexLayer + 1];


        for (unsigned n = 0; n < hiddenLayer.size(); ++n) {
            hiddenLayer[n].calcHiddenGradients(nextLayer);
        }
    }

    //For all layers from output to first hidden Layer
    //Update connection weights
    for (int indexLayer = layers.size() - 1; indexLayer > 0; --indexLayer) {

        Layer &layer = layers[indexLayer];
        Layer &prevLayer = layers[indexLayer - 1];

        for (auto &neuron : layer) {
            neuron.updateInputWeights(prevLayer);
        }
    }

}

void Net::getOutput(vector<double> &outputValues) const {
    outputValues.clear();

    for (int n = 0; n < layers.back().size() - 1; ++n) {
        outputValues.push_back(layers.back()[n].getOutputValue());
    }
}

stringstream Net::getNetStructure() const {
    stringstream ss_topology, ss_neurons;
    ss_topology << "topology:";
    for(int i = 0; i < layers.size(); ++i) {
        ss_topology << " " << layers[i].size() - 1;
        if (i == layers.size() - 1)
            break;
        for (int j = 0; j < layers[i].size(); ++j) {
            ss_neurons << "[" << i << "," << j << "]";
            const Neuron *c_neuron = &layers[i][j];
            for(Connection c : c_neuron->outputsWeights) {
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
    for (int i = 0; i < topology.size(); ++i) {
        string num = to_string(topology[i]);
        nums += num + "----";
        dots += ".";
        for (int i =0; i < num.length() - 1; ++i) {
            dots+=" ";
        }
        dots += "    ";
    }
    dots+="\n";

    nums = nums.substr(0, nums.length() - 4) + "\n";

    cout << "Network structure:" << endl;
    cout << dots << dots << dots << nums << dots << dots << dots << endl;
}

bool Net::loadWeightsFromSource(string &path, vector<unsigned> &topology, vector<vector <vector<double> > > &weight_cube) {
    ifstream f(path, ios::in);

    //get topology
    string line;
    string label;

    while(!f.eof()) {
        getline(f, line);
        if (line[0] != char('#') ) {
            cout << line << endl;
            break;
        }
    }

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

double Net::crossEntropyLoss(const vector<double> &targetValues, const vector<double> &estimatedValues) {
    double loss = 0;

    for (int i = 0; i < estimatedValues.size(); ++i) {
        loss+=targetValues[i] * log(estimatedValues[i]);
    }
    return -loss;
}

double Net::getLoss(const vector<double> &targetValues) {
    vector<double> estimatedValues;
    getOutput(estimatedValues);
    return crossEntropyLoss(targetValues, estimatedValues);

}

bool Net::saveNetworkToFile(string path, int argc, char** argv, string trainingMetaInfo) {
    stringstream ss = getNetStructure();
    ofstream f(path);
    if (!f.good()) {
        cout << "Error: output file " << path << "not good" << endl;
        return false;
    }

    string cmd = "";
    for (int i = 0; i < argc; ++i) {
        cmd+= argv[i];
        cmd+= " ";
    }

    time_t t = time(0);   // get time now
    struct tm * now = localtime( & t );
    string date = to_string(now->tm_mon + 1) + "/" + to_string(now->tm_mday) + "/" + to_string(now->tm_year + 1900);
    f << "#Neural network weight configuration\n#\n#Constructed by Carbon Copy Signature Matching - training application\n#Executed with: " << cmd << "\n#Latest update: " << date << "\n#" << endl;
    f << trainingMetaInfo << endl;
    f << ss.str();
    f.close();
    return true;
}

