#include "Animal.h"

Animal::Animal()
{
    name = "None";
}

Animal::Animal(const string &name)
{
    this->name = name;
}

Animal::Animal(const string &name, const vector<Attribute> &attributes)
{
    this->name = name;
    this->attributes = attributes;
}

Animal::Animal(const Animal &animal)
{
    name = animal.getName();
    attributes = animal.getAttributes();
}

string Animal::getName() const
{
    return name;
}

vector<Attribute> Animal::getAttributes() const
{
    return attributes;
}

void Animal::setName(const string &name)
{
    this->name = name;
}

void Animal::setAttributes(const vector<Attribute> &attributes)
{
    this->attributes = attributes;
}

void Animal::print() const
{
    cout << "Name: " << getName() << " Attributes: ";
    for(Attribute attribute : attributes)
    {
        cout << attribute << ", ";
    }
    cout << endl;
}