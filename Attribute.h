#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include <iostream>
#include <fstream>
#include <new>
#include <string>
#include <chrono>
#include <vector>

using namespace std;

class Attribute
{
    public:
        Attribute();
        Attribute(int index, string attribute);
        int getIndex() const;
        string getAttribute() const;
        void setIndex(int index);
        void setAttribute(string attribute);
        friend ostream& operator<< (ostream& os, const Attribute& attribute);
        bool operator==(const Attribute& other) const;

    private:
        int index;
        string attribute;
};

#endif