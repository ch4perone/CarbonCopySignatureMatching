#include <cstdlib>
#include <cmath>
#include "Neuron.h"


double Neuron::eta = 0.1; //0.1 0.15
double Neuron::alpha = 0.6; //0.6 0.5

Neuron::Neuron(unsigned numOutputs, unsigned index) {
    for (unsigned c = 0; c < numOutputs; ++c) {
        m_outputsWeights.push_back(Connection());
        m_outputsWeights.back().weight = randomWeight();
        //m_outputsWeights.back().deltaWeight = 0.1;
    }

    m_index = index;
}

Neuron::Neuron(unsigned numOutputs, unsigned index, vector<double> &weight_v) {
    for (unsigned c = 0; c < numOutputs; ++c) {
        m_outputsWeights.push_back(Connection());
        m_outputsWeights.back().weight = weight_v[c];
        //m_outputsWeights.back().deltaWeight = 0.1;
    }

    m_index = index;
}

void Neuron::feedForward(Layer &prevLayers) {
    double sum = 0.0;

    //Sum the previous layer's output
    //including bias node

    for (int n = 0; n < prevLayers.size(); ++n) {
        sum+= prevLayers[n].getOutputValue() * prevLayers[n].m_outputsWeights[m_index].weight;
    }


    m_outputVal = Neuron::activationFunction(sum);
    //cout << " "<< sum << ">>" << m_outputVal;
}

double Neuron::activationFunction(double x) {
    //using tanh - output_range [-1, 1] could be any function
    return (double) tanh(x);
}

double Neuron::activationFunctionDerivative(double x) {
    //tanh derivative (approximately)
    return 1- x * x;
}

void Neuron::calcOutputGradients(double targetVal) {
    double delta = targetVal - m_outputVal;
    m_gradient = delta * Neuron::activationFunctionDerivative(m_outputVal);
}

void Neuron::calcHiddenGradients(const Layer &nextLayer) {
    double dow = sumDOW(nextLayer);

    m_gradient = dow * Neuron::activationFunctionDerivative(m_outputVal);

}

double Neuron::sumDOW(const Layer &nextLayer) const {
    double sum = 0.0;

    for (int n = 0; n < nextLayer.size() - 1; ++n) {
        sum += m_outputsWeights[n].weight * nextLayer[n].m_gradient;
    }
    return sum;
}

void Neuron::updateInputWeights(Layer &prevLayer) {

    //The weights to be updated are in the Connection container
    //in the neurons in preceding layer
    for (int n = 0; n < prevLayer.size(); ++n) {
        Neuron &neuron = prevLayer[n];

        double oldDeltaWeight = neuron.m_outputsWeights[m_index].deltaWeight;
        double newDeltaWeight =
                //Individual input, magnified by the gradient and train rate:
                eta
                * neuron.getOutputValue()
                * m_gradient
                //+ Monumentum  (fraction of prev delta Weight)
                + alpha
                  * oldDeltaWeight;


        neuron.m_outputsWeights[m_index].deltaWeight = newDeltaWeight;
        neuron.m_outputsWeights[m_index].weight += newDeltaWeight;

    }


}

double Neuron::randomWeight() {
    double x = 2 * (double) rand() / (double) RAND_MAX * 0.98 -
               1; //used to be: (double) rand() / (double) RAND_MAX !!!!MAGIC!!!

    //thin out higher numbers
    if (x > 0.5)
        x = (double) rand() / (double) RAND_MAX;
    if (x > 0.8)
        x = (double) rand() / (double) RAND_MAX;

    return x;

}