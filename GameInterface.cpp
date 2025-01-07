#include "GameInterface.h"

GameInterface::GameInterface(){}

GameInterface::GameInterface(int inputLayerSize, int hiddenLayerSize, int ouputLayerSize) : neuralNetwork(inputLayerSize, hiddenLayerSize, ouputLayerSize){}

void GameInterface::loadGame(string &classesFile, string &predicatesFile, string &predicateMatrixBinaryFile, string &questionsFile, string& inputLayerWeightsFile, string &hiddenLayerWeightsFile, string& inputBiasesFile, string &hiddenBiasesFile)
{
    cout << "Loading Game..." << endl;
    cout << "Training neural network..." << endl;
    cout << "This will take up to one minute. Thank you for your patience." << endl;
    cout << endl;
    neuralNetwork.initialize(inputLayerWeightsFile, inputBiasesFile, hiddenLayerWeightsFile, hiddenBiasesFile);
    questionBank.initialize(questionsFile);
    animalBank.initialize(classesFile, predicatesFile, predicateMatrixBinaryFile);
    int sizeOfInputLayer = animalBank.getTotalAttributes();
    Matrix userResponses(sizeOfInputLayer, 1, .3);
    this->userResponses = userResponses;
    processExclusiveAttributes(predicateMatrixBinaryFile);
    processSharedAttributes(predicateMatrixBinaryFile);
    neuralNetwork.train(predicateMatrixBinaryFile, .01);
}

void GameInterface::processExclusiveAttributes(string& predicateMatrixBinaryFile)
{
    ifstream matrixFile(predicateMatrixBinaryFile);
    if (!matrixFile.is_open()) {
        cerr << "Failed to open the file: " << predicateMatrixBinaryFile << endl;
        return;
    }

    vector<vector<int>> matrix;
    string line;
    while (getline(matrixFile, line)) 
    {
        istringstream rowStream(line);
        vector<int> row;
        int value;
        while (rowStream >> value) 
        {
            row.push_back(value);
        }
        matrix.push_back(row);
    }
    matrixFile.close();

    int numOfAttributes = matrix[0].size();
    vector<vector<int>> allExclusiveAttributes(numOfAttributes);

    for (int observedAttributeIndex = 0; observedAttributeIndex < numOfAttributes; ++observedAttributeIndex) 
    {
        vector<int> exclusiveAttributes;
        for (int i = 0; i < numOfAttributes; i++) 
        {
            if (i != observedAttributeIndex) 
            {
                exclusiveAttributes.push_back(i);
            }
        }

        for (const auto& row : matrix) 
        {
            if (row[observedAttributeIndex] == 1) 
            {
                for (int otherAttributeIndex = 0; otherAttributeIndex < numOfAttributes; ++otherAttributeIndex) 
                {
                    if (row[otherAttributeIndex] == 1) 
                    {
                        auto it = find(exclusiveAttributes.begin(), exclusiveAttributes.end(), otherAttributeIndex);
                        if (it != exclusiveAttributes.end()) 
                        {
                            exclusiveAttributes.erase(it);
                        }
                    }
                }
            }
        }
        allExclusiveAttributes[observedAttributeIndex] = vector<int>(exclusiveAttributes.begin(), exclusiveAttributes.end());
    }
    this->allExclusiveAttributes = allExclusiveAttributes;  
}

void GameInterface::processSharedAttributes(string & predicateMatrixBinaryFile)
{
    ifstream matrixFile(predicateMatrixBinaryFile);
    if (!matrixFile.is_open()) {
        cerr << "Failed to open the file: " << predicateMatrixBinaryFile << endl;
        return;
    }

    vector<vector<int>> matrix;
    string line;
    while (getline(matrixFile, line)) 
    {
        istringstream rowStream(line);
        vector<int> row;
        int value;
        while (rowStream >> value) 
        {
            row.push_back(value);
        }
        matrix.push_back(row);
    }
    matrixFile.close();

    int numOfAttributes = matrix[0].size();
    vector<vector<int>> allSharedAttributes(numOfAttributes);

    for (int observedAttributeIndex = 0; observedAttributeIndex < numOfAttributes; ++observedAttributeIndex) 
    {
        vector<int> sharedAttributes;
        for (int i = 0; i < numOfAttributes; i++) 
        {
            if (i != observedAttributeIndex) 
            {
                sharedAttributes.push_back(i);
            }
        }

        for (const auto& row : matrix) 
        {
            if (row[observedAttributeIndex] == 1) 
            {
                for (int otherAttributeIndex = 0; otherAttributeIndex < numOfAttributes; ++otherAttributeIndex) 
                {
                    if (row[otherAttributeIndex] == 0) 
                    {
                        auto it = find(sharedAttributes.begin(), sharedAttributes.end(), otherAttributeIndex);
                        if (it != sharedAttributes.end()) 
                        {
                            sharedAttributes.erase(it);
                        }
                    }
                }
            }
        }
        allSharedAttributes[observedAttributeIndex] = vector<int>(sharedAttributes.begin(), sharedAttributes.end());
    }
    this->allSharedAttributes = allSharedAttributes;
}

void GameInterface::handleGame()
{
    bool gameActive = true;
    while(gameActive)
    {
        restartGame();

        cout << "Welcome to 20 Questions. Please think of a mammal." << endl << "You can check the list of valid mammals and their attributes with the command '/animals'" << endl;
        cout << endl;

        cout << "Question 1: Starting question" << endl;
        bool fastStart = askRandomMultiAttributeQuestion();
        cout << endl;
        cout << endl;

        for(int i = 0; i < 19; i++)
        {
            cout << "Question " << i + 2 << ": ";
            Matrix outputMatrix = neuralNetwork.predict(userResponses);
            for(int index : guessedAnimals)
            {
                outputMatrix[index][0] = 0.0;
            }
            int guess = outputMatrix.getRowIndexOfMaxValue();

            if(i == 18)
            {
                cout << "Making guess" << endl;
                bool success = askGuess(guess);
                if(success)
                {
                    cout << "I won!! Thank you for playing!";
                }
                else
                {
                    cout << "Oops, you won! Thank you for playing!";
                }
            }
            else if(i > 10 && outputMatrix[guess][0] > .7)
            {
                cout << "Making guess" << endl;
                bool success = askGuess(guess);
                if(success)
                { 
                    cout << "I won!! Thank you for playing!"; 
                    break;
                }
                else
                {
                    guessedAnimals.push_back(guess);
                }
            }
            else if (i > 14)
            {
                cout << "Smart Question: small range of possible mammals" << endl;
                bool success = askLeastFrequentlyOcurringAttributeQuestion(19 - i);
                if(success)
                {
                    cout << "I won!! Thank you for playing!"; 
                    break;
                }
            }
            else if(i > 4 || (i <= 4 && fastStart))
            {
                cout << "Smart Question: wide range of possible mammals" << endl;
                bool success = askMostFrequentlyOcurringAttributeQuestion(8);
                if(success)
                { 
                    cout << "I won!! Thank you for playing!"; 
                    break;
                }
            }
            else
            {
                cout << "Random Question" << endl;
                bool success = askRandomQuestion();
                if(success)
                { 
                    cout << "I won!! Thank you for playing!"; 
                    break;
                }
            }
            cout << endl;
        }

        cout << endl;
        cout << "Would you like to play again?" << endl;
        bool playAgain = promptUser();
        cout << endl;
        gameActive = playAgain;
    }    
}

bool GameInterface::askRandomQuestion()
{
    bool foundAnimalByExhaustion = false;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, userResponses.getRows() - 1);

    int randomNumber = dis(gen);

    bool foundQuestion = false;
    if(questionIndexesAsked.size() == questionBank.getQuestions().size())
    {
        Matrix outputMatrix = neuralNetwork.predict(userResponses);
        for(int index : guessedAnimals)
        {
            outputMatrix[index][0] = 0.0;
        }
        int guess = outputMatrix.getRowIndexOfMaxValue();
        bool success = askGuess(guess);
        if(success)
        { 
            foundAnimalByExhaustion = true;
        }
        else
        {
            guessedAnimals.push_back(guess);
        }
    }

    while(!foundQuestion)
    {
        if(find(questionIndexesAsked.begin(), questionIndexesAsked.end(), randomNumber) == questionIndexesAsked.end())
        {
            foundQuestion = true;
        }
        else
        {
            randomNumber = ((randomNumber + 1) % questionBank.getQuestions().size());
        }
    }
    cout << questionBank.getQuestions()[randomNumber]->getQuestionText() << endl;
    bool storeAttribute = promptUser();
    addQuestionToInput(storeAttribute, randomNumber);
    return foundAnimalByExhaustion;
}

bool GameInterface::askRandomMultiAttributeQuestion()
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, questionBank.getQuestions().size() - 1);

    int randomNumber = dis(gen);

    bool foundQuestion = false;
    while(!foundQuestion)
    {
        if(find(questionIndexesAsked.begin(), questionIndexesAsked.end(), randomNumber) == questionIndexesAsked.end() && questionBank.getQuestions()[randomNumber]->getTargetedAttributes().size() != 1)
        {
            foundQuestion = true;            
        }
        else
        {
            randomNumber = dis(gen);
        }
    }
    cout << questionBank.getQuestions()[randomNumber]->getQuestionText() << endl;
    bool storeAttribute = promptUser();
    addQuestionToInput(storeAttribute, randomNumber);
    return storeAttribute;
}

bool GameInterface::askMostFrequentlyOcurringAttributeQuestion(const int targetNumber)
{
    /*
    1) Calculate the top 5 animals that the neural network produces (Done)
    2) Form a list of attributes of those animals as well as frequencies (Done)
    3) Take the highest frequency attribute that has not already been asked about and ask a question on that attribute
        a)Determine what the highest frequency attribute is
        b)If that attribute has been asked about go to the next highest frequency attribute
        c)Repeat until one is found that has not been asked about
        d)Ask that question and update the indexesAskedAbout vector
    */
    bool foundAnimalByExhaustion = false;

    Matrix outputMatrix = neuralNetwork.predict(userResponses);
    for(int index : guessedAnimals)
    {
        outputMatrix[index][0] = 0.0;
    }

    vector<int> topTargetedAnimalIndexes;
    for(int i = 0; i < outputMatrix.getRows(); i++)
    {
        double probability = outputMatrix[i][0];
        if(topTargetedAnimalIndexes.size() == 0)
        {
            topTargetedAnimalIndexes.push_back(i);
        }
        else
        {
            for(int j = 0; j < topTargetedAnimalIndexes.size(); j++)
            {
                int compIndex = topTargetedAnimalIndexes[j];
                double compProbability = outputMatrix[0][compIndex];
                if(probability > compProbability)
                {
                    topTargetedAnimalIndexes.insert(topTargetedAnimalIndexes.begin() + j, i);
                    break;
                }
            }
            if(topTargetedAnimalIndexes.size() > targetNumber)
            {
                topTargetedAnimalIndexes.pop_back();
            }
        }
    }
    vector<int> frequencies;
    vector<Attribute> attributes;
    for(int i = 0; i < targetNumber; i++)
    {
        int index = topTargetedAnimalIndexes[i];
        Animal animal = *(animalBank.getAnimals()[index]);
        vector<Attribute> attributesOfAnimal = animal.getAttributes();
        for(int j = 0; j < attributesOfAnimal.size(); j++)
        {
            Attribute attributeOfAnimal = attributesOfAnimal[j];
            if(find(attributes.begin(), attributes.end(), attributeOfAnimal) == attributes.end())
            {
                attributes.push_back(attributeOfAnimal);
                frequencies.push_back(1);
            }
            else
            {
                for(int k = 0; k < attributes.size(); k++)
                {
                    if(attributes[k] == attributeOfAnimal)
                    {
                        frequencies[k]++;
                        break;
                    }
                }
            }
        }
    }
    int index = 0;
    bool duplicateQuestion = false;    
    do
    {
        duplicateQuestion = false;    
        if(!frequencies.empty())
        {
            int highestFrequency = 0;
            for(int i = 0; i < frequencies.size(); i++)
            {
                if(frequencies[i] > highestFrequency)
                {
                    highestFrequency = frequencies[i];
                    index = i;
                }
            }

            vector<Attribute> attribute;
            attribute.push_back(attributes[index]);
            vector<Question*> questions = questionBank.searchAttributes(attribute);
            for(Question* question : questions)
            {
                for(int questionIndexAsked : questionIndexesAsked)
                {
                    bool alreadyErased = false;
                    if(question->getIndex() == questionIndexAsked)
                    {
                        duplicateQuestion = true;
                        if(!alreadyErased)
                        {
                            frequencies.erase(frequencies.begin() + index);
                            attributes.erase(attributes.begin() + index);
                            alreadyErased = true;
                        }
                    }
                }
            }
        }
        else
        {
            foundAnimalByExhaustion = askRandomQuestion();
            return foundAnimalByExhaustion;
        }
    }
    while(duplicateQuestion);

    vector<Attribute> attribute;
    attribute.push_back(attributes[index]);
    vector<Question*> questions = questionBank.searchAttributes(attribute);
    Question* questionToBeAsked = questions[0];
    int questionIndex = questionToBeAsked->getIndex();

    cout << questionBank.getQuestions()[questionIndex]->getQuestionText() << endl;

    bool attributesToStore = promptUser();
    addQuestionToInput(attributesToStore, questionIndex);
    return foundAnimalByExhaustion;
}

bool GameInterface::askLeastFrequentlyOcurringAttributeQuestion(const int targetNumber)
{
    bool foundAnimalByExhaustion = false;
    Matrix outputMatrix = neuralNetwork.predict(userResponses);
    for(int index : guessedAnimals)
    {
        outputMatrix[index][0] = 0.0;
    }

    vector<int> topTargetedAnimalIndexes;
    for(int i = 0; i < outputMatrix.getRows(); i++)
    {
        double probability = outputMatrix[i][0];
        if(topTargetedAnimalIndexes.size() == 0)
        {
            topTargetedAnimalIndexes.push_back(i);
        }
        else
        {
            for(int j = 0; j < topTargetedAnimalIndexes.size(); j++)
            {
                int compIndex = topTargetedAnimalIndexes[j];
                double compProbability = outputMatrix[0][compIndex];
                if(probability > compProbability)
                {
                    topTargetedAnimalIndexes.insert(topTargetedAnimalIndexes.begin() + j, i);
                    break;
                }
            }
            if(topTargetedAnimalIndexes.size() > targetNumber)
            {
                topTargetedAnimalIndexes.pop_back();
            }
        }
    }

    vector<int> frequencies;
    vector<Attribute> attributes;
    for(int i = 0; i < targetNumber; i++)
    {
        int index = topTargetedAnimalIndexes[i];
        Animal animal = *(animalBank.getAnimals()[index]);
        vector<Attribute> attributesOfAnimal = animal.getAttributes();
        for(int j = 0; j < attributesOfAnimal.size(); j++)
        {
            Attribute attributeOfAnimal = attributesOfAnimal[j];
            if(find(attributes.begin(), attributes.end(), attributeOfAnimal) == attributes.end())
            {
                attributes.push_back(attributeOfAnimal);
                frequencies.push_back(1);
            }
            else
            {
                for(int k = 0; k < attributes.size(); k++)
                {
                    if(attributes[k] == attributeOfAnimal)
                    {
                        frequencies[k]++;
                        break;
                    }
                }
            }
        }
    }
    int index = 0;
    bool duplicateQuestion;    
    do
    {  
        duplicateQuestion = false;    
        if(!frequencies.empty())
        {
            int lowestFrequency = 100;
            for(int i = 0; i < frequencies.size(); i++)
            {
                if(frequencies[i] < lowestFrequency)
                {
                    lowestFrequency = frequencies[i];
                    index = i;
                }
            }

            vector<Attribute> attribute;
            attribute.push_back(attributes[index]);
            vector<Question*> questions = questionBank.searchAttributes(attribute);
            for(Question* question : questions)
            {
                for(int questionIndexAsked : questionIndexesAsked)
                {
                    bool alreadyErased = false;
                    if(question->getIndex() == questionIndexAsked)
                    {
                        duplicateQuestion = true;
                        if(!alreadyErased)
                        {
                            frequencies.erase(frequencies.begin() + index);
                            attributes.erase(attributes.begin() + index);
                            alreadyErased = true;
                        }
                        break;
                    }
                }
            }
        }
        else
        {
            foundAnimalByExhaustion = askRandomQuestion();
            return foundAnimalByExhaustion;
        }
    }
    while(duplicateQuestion);

    vector<Attribute> attribute;
    attribute.push_back(attributes[index]);
    vector<Question*> questions = questionBank.searchAttributes(attribute);

    Question* questionToBeAsked = questions[0];

    int questionIndex = questionToBeAsked->getIndex();

    cout << questionBank.getQuestions()[questionIndex]->getQuestionText() << endl;

    bool attributesToStore = promptUser();
    addQuestionToInput(attributesToStore, questionIndex);
    return foundAnimalByExhaustion;
}

bool GameInterface::askGuess(int index)
{
        Animal animal = guessAnimal(index);
        cout << "Is your mammal a " << animal.getName() << "?" << endl;
        bool success = promptUser();
        cout << endl;
        return success;
}

void GameInterface::restartGame()
{
    int sizeOfInputLayer = animalBank.getTotalAttributes();
    Matrix userResponses(sizeOfInputLayer, 1, .5);
    this->userResponses = userResponses;
    questionIndexesAsked.clear();
    guessedAnimals.clear();
}

bool GameInterface::promptUser() const
{
    bool userResponse;
    while(true)
    {
        cout << "Please indicate 'yes' or 'no'" << endl;
        string response;
        cin >> response;
        if(response == "/animals")
        {
            cout << endl;
            animalBank.print();
        }
        else if(response == "yes" || response == "YES" || response == "y" || response == "Y")
        {
            userResponse = true;
            break;
        }
        else if(response == "no" || response == "NO" || response == "n" || response == "N")
        {
            userResponse = false;
            break;
        }
        cout << "Please give a valid response..." << endl;
    }
    return userResponse;
}

void GameInterface::addQuestionToInput(bool yes, int questionIndex)
{
    questionIndexesAsked.push_back(questionIndex);
    vector<Question*> similarQuestions = getSimilarQuestions(*(questionBank.getQuestions()[questionIndex]));
    vector<Question*> oppositeQuestions = getOppositeQuestions(*(questionBank.getQuestions()[questionIndex]));
    for(Question* similarQuestion : similarQuestions)
    {
        if(find(questionIndexesAsked.begin(), questionIndexesAsked.end(), similarQuestion->getIndex()) == questionIndexesAsked.end())
        {
            questionIndexesAsked.push_back(similarQuestion->getIndex());
        }
    }
    for(Question* oppositeQuestion : oppositeQuestions)
    {
        if(find(questionIndexesAsked.begin(), questionIndexesAsked.end(), oppositeQuestion->getIndex()) == questionIndexesAsked.end())
        {
            questionIndexesAsked.push_back(oppositeQuestion->getIndex());
        }
    }

    vector<Attribute> questionAttributes = questionBank.getQuestions()[questionIndex]->getTargetedAttributes();
    if(yes)
    {
        for(Attribute attribute : questionAttributes)
        {
            int index = attribute.getIndex();
            vector<int> ruledOutAttributesIndexes = allExclusiveAttributes[index];
            vector<int> provenAttributesIndexes = allSharedAttributes[index];
            
            for(int i : ruledOutAttributesIndexes)
            {
                auto it = find(provenAttributesIndexes.begin(), provenAttributesIndexes.end(), i);
                if(it != provenAttributesIndexes.end())
                {
                    ruledOutAttributesIndexes.erase(it);
                    provenAttributesIndexes.erase(it);
                }
            }
            for(int i : ruledOutAttributesIndexes)
            {
                userResponses[i][0] = 0.0;
            }
            userResponses[index][0] = 1.0;
            for(int i : provenAttributesIndexes)
            {
                userResponses[i][0] = 1.0;
            }
        }
    }
    else
    {
        for(Attribute attribute : questionAttributes)
        {
            int index = attribute.getIndex();
            vector<int> ruledOutAttributesIndexes = allExclusiveAttributes[index];
            vector<int> provenAttributesIndexes = allSharedAttributes[index];

            for(int i : ruledOutAttributesIndexes)
            {
                double adjustedInputValue = userResponses[i][0] + .05;
                if(adjustedInputValue > 1.0)
                {
                    userResponses[i][0] = 1.0;
                }
                else
                {
                    userResponses[i][0] = adjustedInputValue;
                }
            }
            userResponses[index][0] = 0.0;
            for(int i : provenAttributesIndexes)
            {
                double adjustedInputValue = userResponses[i][0] - .05;
                if(adjustedInputValue < 0)
                {
                    userResponses[i][0] = 0;
                }
                else
                {
                    userResponses[i][0] = adjustedInputValue;
                }
            }
        }
    }
}

Animal GameInterface::guessAnimal(int index) const
{
        return *(animalBank.getAnimals()[index]);
}

vector<Question*> GameInterface::getSimilarQuestions(const Question question) const
{
    vector<Attribute> attributesOfQuestion = question.getTargetedAttributes();
    vector<Question*> similarQuestions;
    if(attributesOfQuestion.size() < 2)
    {
        for(Attribute attribute : attributesOfQuestion)
        {
            int indexOfAttribute = attribute.getIndex();
            vector<int> similarAttributeIndexes = allSharedAttributes[indexOfAttribute];
            vector<Question*> similarQuestionsToAttribute = questionBank.searchAttributeIndexes(similarAttributeIndexes);
            for(Question* similarQuestion : similarQuestionsToAttribute)
            {
                similarQuestions.push_back(similarQuestion);
            }
        }
    }
    return similarQuestions;
}

vector<Question*> GameInterface::getOppositeQuestions(const Question question) const
{
    vector<Attribute> attributesOfQuestion = question.getTargetedAttributes();
    vector<Question*> oppositeQuestions;

    if(attributesOfQuestion.size() < 2)
    {
        for(Attribute attribute : attributesOfQuestion)
        {
            int indexOfAttribute = attribute.getIndex();
            vector<int> oppositeAttributeIndexes = allExclusiveAttributes[indexOfAttribute];
            vector<Question*> oppositeQuestionsToAttribute = questionBank.searchAttributeIndexes(oppositeAttributeIndexes);
            for(Question* oppositeQuestion : oppositeQuestionsToAttribute)
            {
                oppositeQuestions.push_back(oppositeQuestion);
            }
        }
    }
    return oppositeQuestions;
}