#include "main.h"
int main(int argc, char* argv[])
{

    GameInterface game(85, 64, 50);
    string inputWeightsFile = argv[1];
    string inputBiasesFile = argv[2];
    string hiddenWeightsFile = argv[3];
    string hiddenBiasesFile = argv[4];
    string classesFile = argv[5];
    string predicatesFile = argv[6];
    string classesPredicatesMatrixFile = argv[7];
    string questionsFile = argv[8];
    game.loadGame(classesFile, predicatesFile, classesPredicatesMatrixFile, questionsFile, inputWeightsFile, inputBiasesFile, hiddenWeightsFile, hiddenBiasesFile);
    game.handleGame();
}