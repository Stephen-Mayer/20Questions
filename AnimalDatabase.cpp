#include "AnimalDatabase.h"

AnimalDatabase::AnimalDatabase(){}

AnimalDatabase::AnimalDatabase(vector<Animal*> animals)
{
    this->animals = animals;
}

AnimalDatabase::~AnimalDatabase()
{
    for(Animal* animal : animals)
    {
        delete animal;
    }
}

vector<Animal*> AnimalDatabase::getAnimals() const
{
    return animals;
}

int AnimalDatabase::getTotalAttributes() const
{
    return totalAttributes;
}

void AnimalDatabase::initialize(const string &classesFile, const string &predicatesFile, const string &predicateMatrixFile)
{
    ifstream nameFile(classesFile);
    if (!nameFile.is_open()) 
    {
        cerr << "Failed to open the file: " << classesFile << endl;
        return;
    }
    
    string line;
    while(getline(nameFile, line))
    {
        istringstream nameStream(line); 
        int index;
        string name;
        nameStream >> index;
        nameStream >> name;
        Animal* animal = new Animal(name);
        animals.push_back(animal);
    }

    ifstream matrixFile(predicateMatrixFile);
    if (!matrixFile.is_open()) 
    {
        cerr << "Failed to open the file: " << predicateMatrixFile << endl;
        return;
    }    

    string lineAttribute;
    string lineMatrix;
    for(size_t i = 0; i < animals.size(); i++)
    {
        getline(matrixFile, lineMatrix);     
        istringstream matrixStream(lineMatrix);
        int matrixValue;
        vector<Attribute> attributes;
        ifstream attributesFile(predicatesFile);
        if (!attributesFile.is_open()) 
        {
            cerr << "Failed to open the file: " << predicatesFile << endl;
            return;
        }
        int index;
        while(matrixStream >> matrixValue)
        {
            getline(attributesFile, lineAttribute);
            istringstream attributeStream(lineAttribute);
            string att;
            attributeStream >> index;
            attributeStream >> att;
            Attribute attribute((index - 1), att);
            if(matrixValue == 1)
            {
                attributes.push_back(attribute);
            }
        }
        totalAttributes = index;
        animals.at(i)->setAttributes(attributes);
    }
}

Animal AnimalDatabase::searchName(const string &name)
{
    Animal returnAnimal;
    for(Animal* animal : animals)
    {
        if(animal->getName() == name)
        {
            returnAnimal = *animal;
            break;
        }
    }
    return returnAnimal;
}

vector<Animal*> AnimalDatabase::searchAttributes(const vector<Attribute>& attributes)
{
    vector<Animal*> returnAnimalVector;
    for(Animal* animal : animals)
    {
        bool attributesInAnimal = true;
        vector<Attribute> animalAttributes = animal->getAttributes();
        for(Attribute attribute : attributes)
        {
            if(find(animalAttributes.begin(), animalAttributes.end(), attribute) == animalAttributes.end())
            {
                attributesInAnimal = false;
                break;
            }
        }
        if(attributesInAnimal)
        {
            returnAnimalVector.push_back(animal);
        }
    }
    return returnAnimalVector;
}

void AnimalDatabase::print() const
{
    for(Animal* animal: animals)
    {
        animal->print();
    }
    cout << endl;
}