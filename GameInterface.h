#ifndef GAMEINTERFACE_H
#define GAMEINTERFACE_H

#include <iostream>
#include <fstream>
#include <new>
#include <string>
#include <chrono>
#include <vector>
#include <random>
#include <sstream>

#include "Matrix.h"
#include "Attribute.h"
#include "Question.h"
#include "NeuralNetwork.h"
#include "Animal.h"
#include "QuestionDatabase.h"
#include "AnimalDatabase.h"

using namespace std;

class GameInterface
{
    public:
        GameInterface();
        GameInterface(int inputLayerSize, int hiddenLayerSize, int outputLayerSize);
        void loadGame(string &classesFile, string &predicatesFile, string &predicateMatrixBinaryFile, string &questionsFile, string& inputLayerWeightsFile, string &hiddenLayerWeightsFile, string& inputBiasesFile, string &hiddenBiasesFile);
        void processExclusiveAttributes(string &predicateMatrixBinaryFile);
        void processSharedAttributes(string &predicateMatrixBinaryFile);
        void handleGame();
        bool askRandomQuestion();
        bool askRandomMultiAttributeQuestion();
        bool askMostFrequentlyOcurringAttributeQuestion(const int targetNumber);
        bool askLeastFrequentlyOcurringAttributeQuestion(const int targetNumber);
        bool askGuess(int index);
        void restartGame();
        bool promptUser() const;
        void addQuestionToInput(bool yes, int questionIndex);
        Animal guessAnimal(int index) const;
        vector<Question*> getSimilarQuestions(const Question question) const;
        vector<Question*> getOppositeQuestions(const Question question) const;

    private:
        vector<int> guessedAnimals;
        vector<int> questionIndexesAsked;
        vector<vector<int>> allExclusiveAttributes;
        vector<vector<int>> allSharedAttributes;
        Matrix userResponses;
        NeuralNetwork neuralNetwork;
        QuestionDatabase questionBank;
        AnimalDatabase animalBank;
};
#endif