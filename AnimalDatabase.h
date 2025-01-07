#ifndef ANIMALDATABASE_H
#define ANIMALDATABASE_H

#include <iostream>
#include <fstream>
#include <new>
#include <string>
#include <chrono>
#include <vector>
#include <sstream>
#include <algorithm>

#include "Animal.h"
#include "Attribute.h"

using namespace std;

class AnimalDatabase
{
    public:
        AnimalDatabase();
        AnimalDatabase(vector<Animal*> animals);
        ~AnimalDatabase();
        vector<Animal*> getAnimals() const;
        int getTotalAttributes() const;
        void initialize(const string &classesFile, const string &predicatesFile, const string &predicateMatrixFile);
        Animal searchName(const string &name);
        vector<Animal*> searchAttributes(const vector<Attribute>& attributes);
        void print() const;

    private:
        vector<Animal*> animals;
        int totalAttributes;
};

#endif