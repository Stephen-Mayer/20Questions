#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include <iostream>
#include <fstream>
#include <new>
#include <string>
#include <chrono>
#include <vector>
#include <cmath>
#include <sstream>
#include <random>


#include "Matrix.h"

using namespace std;

class NeuralNetwork
{
    public:
        NeuralNetwork();
        NeuralNetwork(int inputSize, int hiddenSize, int outputSize);
        Matrix predict(const Matrix& input) const;
        void initialize(string &inputWeightsFile, string &inputBiasesFile, string &hiddenWeightsFile, string &hiddenBiasesFile);
        void train(string & predicateAnimalBinaryFile, double trainingSpeed); //WIP
        NeuralNetwork& operator = (const NeuralNetwork network);

        Matrix getInputWeights() const;

    private:
        Matrix inputWeights; //Input -> Hidden
        Matrix hiddenWeights; //Hidden -> Output layer
        Matrix inputBiases;
        Matrix hiddenBiases;

        Matrix softmax(const Matrix& matrix) const;
        double computeCrossEntropyLoss(int &indexOfAnimal, Matrix &predictions) const;
};
#endif