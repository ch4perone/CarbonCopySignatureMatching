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
public:
    Neuron(unsigned numOutputs, unsigned index); //random weight
    Neuron(unsigned numOutputs, unsigned index, vector<double> &weight);

    void setOutputVal(double val) {m_outputVal = val;}
    double getOutputValue() const { return m_outputVal;}
    void feedForward(Layer &prevLayers);
    void calcOutputGradients(double targetVal);
    void calcHiddenGradients(const Layer &nextLayer);
    void updateInputWeights(Layer &prevLayer);

    vector<Connection> m_outputsWeights;
private:
    static double eta; //[0.0 .. 1.0] overall net training rate
    static double alpha; //[0.0 .. n] multiplier of the last weight change (monumentum)

    static double randomWeight();//TODO return rand() / (double) RAND_MAX;}
    static double activationFunction(double x);
    static double activationFunctionDerivative(double x);
    double sumDOW(const Layer &nextLayer) const;
    unsigned m_index;
    double m_outputVal;
    double m_gradient;

};



#endif //CARBONCOPYSIGNATUREMATCHING_NEURON_H
