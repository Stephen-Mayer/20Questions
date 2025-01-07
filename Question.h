#ifndef QUESTION_H
#define QUESTION_H

#include <iostream>
#include <fstream>
#include <new>
#include <string>
#include <chrono>
#include <vector>

#include "Attribute.h"

using namespace std;

class Question
{
    public:
        Question();
        Question(const int index, const string &questionText, const vector<Attribute> &targetedAttributes);
        Question(const Question &question);
        int getIndex() const;
        string getQuestionText() const;
        vector<Attribute> getTargetedAttributes() const;
        void setIndex(const int index);
        void setQuestionText(const string &questionText);
        void setTargetedAttributes(const vector<Attribute> &targetedAttributes);
        void print() const;

    private:
        int index;
        string questionText;
        vector<Attribute> targetedAttributes; 
};

#endif