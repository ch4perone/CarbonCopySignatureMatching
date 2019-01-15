#ifndef CARBONCOPYSIGNATUREMATCHING_NEURON_H
#define CARBONCOPYSIGNATUREMATCHING_NEURON_H

#include <vector>

using namespace std;

class Neuron;
typedef vector<Neuron> Layer;

struct Connection {
    double weight;
    double deltaWeight;
};


class Neuron {

private:
    static double eta; //[0.0 .. 1.0] overall net training rate
    static double alpha; //[0.0 .. n] multiplier of the last weight change (monumentum)
    unsigned index;
    double inputSum;
    double outputValue;
    double gradient;

public:
    Neuron(unsigned numOutputs, unsigned index); //random weights
    Neuron(unsigned numOutputs, unsigned index, vector<double> &weight);

    void setOutputValue(double value);
    double getOutputValue() const;
    double getInputSum();

    //Feed forward
    void feedForward(Layer &prevLayers);
    void computeSoftmaxActivation(const vector<double> &z);

    //Back propagate
    void calcOutputGradients(double targetValue, const vector<double> &z);
    void calcHiddenGradients(const Layer &nextLayer);
    void updateInputWeights(Layer &prevLayer);

    vector<Connection> outputsWeights;

private:
    double sumDOW(const Layer &nextLayer) const;

    static double randomWeight();
    static double activationFunction(double x);
    static double activationFunctionDerivative(double x);
    static double softmaxFunction(const vector<double> &z, int j);
    static double softmaxFunctionDerivative(const vector<double> &z, int j);


};



#endif //CARBONCOPYSIGNATUREMATCHING_NEURON_H
