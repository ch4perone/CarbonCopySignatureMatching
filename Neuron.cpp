#include <cstdlib>
#include <cmath>
#include <iostream>
#include "Neuron.h"


double Neuron::eta = 0.1; //0.1 0.15
double Neuron::alpha = 0.6; //0.6 0.5

Neuron::Neuron(unsigned numOutputs, unsigned index) {
    for (unsigned c = 0; c < numOutputs; ++c) {
        outputsWeights.push_back(Connection());
        outputsWeights.back().weight = randomWeight();
        //outputsWeights.back().deltaWeight = 0.1;
    }

    this->index = index;
}

Neuron::Neuron(unsigned numOutputs, unsigned index, vector<double> &weight_v) {
    for (unsigned c = 0; c < numOutputs; ++c) {
        outputsWeights.push_back(Connection());
        outputsWeights.back().weight = weight_v[c];
        //outputsWeights.back().deltaWeight = 0.1;
    }

    this->index = index;
}

void Neuron::feedForward(Layer &prevLayers) {
    double sum = 0.0;

    //Sum the previous layer's output
    //including bias node

    for (int n = 0; n < prevLayers.size(); ++n) {
        sum+= prevLayers[n].getOutputValue() * prevLayers[n].outputsWeights[index].weight;
    }


    inputSum = sum;
    outputValue = Neuron::activationFunction(sum);
    //cout << " "<< sum << ">>" << outputValue;
}

void Neuron::computeSoftmaxActivation(const vector<double> &z) {
    outputValue = softmaxFunction(z, index);
}


void Neuron::calcOutputGradients(double targetValue, const vector<double> &z) {
    double delta = targetValue - outputValue; //cross entropy delta: dE / dx = ground_truth_x - x | or the other way around
    gradient = delta * Neuron::softmaxFunctionDerivative(z, index);
}

void Neuron::calcHiddenGradients(const Layer &nextLayer) {
    double dow = sumDOW(nextLayer);

    gradient = dow * Neuron::activationFunctionDerivative(inputSum);

}

double Neuron::sumDOW(const Layer &nextLayer) const {
    double sum = 0.0;

    for (int n = 0; n < nextLayer.size() - 1; ++n) {
        sum += outputsWeights[n].weight * nextLayer[n].gradient;
    }
    return sum;
}



/*
 * For the this neuron, update all previous neurons weights that are connected to this
 * according to the gradient in this neuron
 * times the actual activation in the prev. neurons (times rates) ^= sensitivity of the weight to changes
 */
void Neuron::updateInputWeights(Layer &prevLayer) {

    //The weights to be updated are in the Connection container
    //in the neurons in preceding layer
    for (int n = 0; n < prevLayer.size(); ++n) {
        Neuron &neuron = prevLayer[n];

        double oldDeltaWeight = neuron.outputsWeights[index].deltaWeight;
        double newDeltaWeight =
                //Individual input, magnified by the gradient and train rate:
                eta
                * neuron.getOutputValue()
                * gradient
                //+ Monumentum  (fraction of prev delta Weight)
                + alpha
                  * oldDeltaWeight;


        neuron.outputsWeights[index].deltaWeight = newDeltaWeight;
        neuron.outputsWeights[index].weight += newDeltaWeight;

    }


}

double Neuron::randomWeight() {
    return (double) rand() / (double) RAND_MAX - 0.5f;
}

/*
 * activation functions and derivatives
 */
double Neuron::activationFunction(double x) {
    //using tanh - output_range [-1, 1] could be any function
    return tanh(x);
}

double Neuron::activationFunctionDerivative(double x) {
    //tanh derivative
    return 1 - tanh(x) * tanh(x);
}

double Neuron::softmaxFunction(const vector<double> &z, int j) { //maybe use index, instead of passing on j.
    double sum = 0;
    for (double k : z) {
        sum += exp(k);
    }

    return exp(z[j]) / sum;
}

double Neuron::softmaxFunctionDerivative(const vector<double> &z, int j) {
    double sum = 0;
    for (double k : z) {
        sum += exp(k);
    }
    return exp(z[j]) * (sum - exp(z[j])) / (sum * sum);
}

void Neuron::setOutputValue(double value) {
    outputValue = value;
}

double Neuron::getOutputValue() const {
    return outputValue;
}

double Neuron::getInputSum() {
    return inputSum;
}



