#include "NeuralNetwork.h"

NeuralNetwork::NeuralNetwork(){}

NeuralNetwork::NeuralNetwork(int inputSize, int hiddenSize, int outputSize)
{
    Matrix inputWeights(hiddenSize, inputSize);
    this->inputWeights = inputWeights;

    Matrix hiddenWeights(outputSize, hiddenSize);
    this->hiddenWeights = hiddenWeights;

    Matrix inputBiases(hiddenSize, 1);
    this->inputBiases = inputBiases;

    Matrix hiddenBiases(outputSize, 1);
    this->hiddenBiases = hiddenBiases;
}

Matrix NeuralNetwork::predict(const Matrix& input) const
{
    Matrix hiddenLayerInput = inputWeights * input;
    hiddenLayerInput = hiddenLayerInput + inputBiases;
    Matrix hiddenLayerOutput = hiddenLayerInput.reluActivation();
    Matrix outputLayerInput = hiddenWeights * hiddenLayerOutput;
    outputLayerInput = outputLayerInput + hiddenBiases;

    return softmax(outputLayerInput);
}

void NeuralNetwork::initialize(string &inputWeightsFile, string &inputBiasesFile, string &hiddenWeightsFile, string &hiddenBiasesFile)//Does not work when initialized off of default constructor
{
    ifstream inputWeightsMatrixFile(inputWeightsFile);
    if (!inputWeightsMatrixFile.is_open()) 
    {
        cerr << "Failed to open the file: " << inputWeightsFile << endl;
        return;
    }

    ifstream inputBiasesVectorFile(inputBiasesFile);
    if (!inputBiasesVectorFile.is_open()) 
    {
        cerr << "Failed to open the file: " << inputBiasesFile << endl;
        return;
    }

    ifstream hiddenWeightsMatrixFile(hiddenWeightsFile);
    if (!hiddenWeightsMatrixFile.is_open()) 
    {
        cerr << "Failed to open the file: " << hiddenWeightsFile << endl;
        return;
    }

    ifstream hiddenBiasesVectorFile(hiddenBiasesFile);
    if (!hiddenBiasesVectorFile.is_open()) 
    {
        cerr << "Failed to open the file: " << hiddenBiasesFile << endl;
        return;
    }

    string line;
    for(int i = 0; i < inputWeights.getRows(); i++)
    {
        getline(inputWeightsMatrixFile, line);
        istringstream inputWeightsStream(line);
        for(int j = 0; j < inputWeights.getCols(); j++)
        {
            double value;
            inputWeightsStream >> value;
            inputWeights[i][j] = value;
        }
    }

    for(int i = 0; i < hiddenWeights.getRows(); i++)
    {
        getline(hiddenWeightsMatrixFile, line);
        istringstream hiddenWeightsStream(line);
        for(int j = 0; j < hiddenWeights.getCols(); j++)
        {
            double value;
            hiddenWeightsStream >> value;
            hiddenWeights[i][j] = value;
        }
    }

    double value;
    for(int i = 0; i < inputBiases.getRows(); i++)
    {
        getline(inputBiasesVectorFile, line);
        istringstream inputBiasesStream(line);
        inputBiasesStream >> value;
        inputBiases[i][0] = value;
    }


    for(int i = 0; i < hiddenBiases.getRows(); i++)
    {
        getline(hiddenBiasesVectorFile, line);
        istringstream hiddenBiasesStream(line);
        hiddenBiasesStream >> value;
        hiddenBiases[i][0] = value;
    }
}

void NeuralNetwork::train(string &predicateAnimalBinaryFile, double trainingSpeed)
{
    string line;
    double value;
    double loss = 6;
    int entropyCounter = 0;
    while(loss > 5 && entropyCounter < 500)
    {
        if(entropyCounter == 100)
        {
            cout << "Progress 1/5" << endl;
        }
        else if(entropyCounter == 200)
        {
            cout << "Progress 2/5" << endl;
        }
        else if(entropyCounter == 300)
        {
            cout << "Progress 3/5" << endl;
        }
        else if(entropyCounter == 400)
        {
            cout << "Progress 4/5" << endl;
        }
        entropyCounter++;
        ifstream matrixFile(predicateAnimalBinaryFile);
        if (!matrixFile.is_open()) 
        {
            cout << "Error" << endl;
            cerr << "Failed to open the file: " << predicateAnimalBinaryFile << endl;
            return;
        }    
        loss = 0;
        int index = 0;
        while(getline(matrixFile, line))
        {
            istringstream attributeStream(line);
            vector<vector<double>> inputVector;
            while(attributeStream >> value)
            {
                random_device rd;
                mt19937 gen(rd());
                uniform_int_distribution<> dis(1, 85);
                int randomNumber = dis(gen);

                if(randomNumber == 50 && value == 1)
                {
                    value = 0;
                }
                else if(randomNumber == 50 && value == 0)
                {
                    value = 1;
                }
                vector<double> row = {value};
                inputVector.push_back(row);
            }
            Matrix inputMatrix(inputVector);
            Matrix prediction = predict(inputMatrix);

            Matrix expected(50, 1);
            expected[index][0] = 1;

            Matrix deltaHidden = prediction - expected;

            Matrix hiddenLayerInput = inputWeights * inputMatrix;
            hiddenLayerInput = hiddenLayerInput + inputBiases;
            Matrix hiddenLayerOutput = hiddenLayerInput.reluActivation();

            Matrix gradientHiddenWeights = deltaHidden * hiddenLayerOutput.transposeMatrix();
            Matrix gradientHiddenBias = deltaHidden;

            Matrix deltaInput = (hiddenWeights.transposeMatrix() * deltaHidden).elementwiseMultiplication(hiddenLayerOutput);

            Matrix gradientInputWeights = deltaInput * inputMatrix.transposeMatrix();
            Matrix gradientInputBias = deltaInput;

            Matrix newHiddenWeights = hiddenWeights - gradientHiddenWeights.scalarMultiplication(trainingSpeed);
            Matrix newHiddenBiases = hiddenBiases - gradientHiddenBias.scalarMultiplication(trainingSpeed);
            Matrix newInputWeights = inputWeights - gradientInputWeights.scalarMultiplication(trainingSpeed);
            Matrix newInputBiases = inputBiases - gradientInputBias.scalarMultiplication(trainingSpeed);     

            hiddenWeights.setMatrixEuqal(newHiddenWeights);
            hiddenBiases.setMatrixEuqal(newHiddenBiases);
            inputWeights.setMatrixEuqal(newInputWeights);
            inputBiases.setMatrixEuqal(newInputBiases);         

            loss = loss + computeCrossEntropyLoss(index, prediction);
            index++;
        }
    }
    /*
    cout << "New Hidden Biases:" << endl;
    hiddenBiases.print();
    cout << endl;     

    cout << "New Input Biases:" << endl;
    inputBiases.print();
    cout << endl;        

    cout << "New Hidden Weights:" << endl;
    hiddenWeights.print();
    cout << endl;    

    cout << "New Input Weights:" << endl;
    hiddenWeights.print();
    cout << endl;              
    */  
}

Matrix NeuralNetwork::softmax(const Matrix& matrix) const
{
    Matrix result = matrix;
    double maxVal = matrix[0][0];
    for (int i = 1; i < matrix.getRows(); i++) 
    {
        if (matrix[i][0] > maxVal) 
        {
            maxVal = matrix[i][0];
        }
    }
    double sum = 0.0;
    for (int i = 0; i < matrix.getRows(); i++) 
    {
        result[i][0] = exp(matrix[i][0] - maxVal);
        sum += result[i][0];
    }
    for (int i = 0; i < matrix.getRows(); i++) 
    {
        result[i][0] /= sum;
    }
    return result;
}

double NeuralNetwork::computeCrossEntropyLoss(int &indexOfAnimal, Matrix &predictions) const
{
    if (indexOfAnimal < 0 || indexOfAnimal >= predictions.getRows()) 
    {
        throw invalid_argument("Invalid true class index");
    }

    double predictedProb = predictions[indexOfAnimal][0];
    return -log(predictedProb + 1e-8);
}