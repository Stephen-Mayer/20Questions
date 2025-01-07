#ifndef ANIMAL_H
#define ANIMAL_H

#include <iostream>
#include <fstream>
#include <new>
#include <string>
#include <chrono>
#include <vector>

#include "Attribute.h"

using namespace std;

class Animal
{
    public:
        Animal();
        Animal(const string &name);
        Animal(const string &name, const vector<Attribute> &attributes);
        Animal(const Animal &animal);
        string getName() const;
        vector<Attribute> getAttributes() const;
        void setName(const string &name);
        void setAttributes(const vector<Attribute> &attributes);
        void print() const;

    private:
        string name;
        vector<Attribute> attributes; 
};

#endif