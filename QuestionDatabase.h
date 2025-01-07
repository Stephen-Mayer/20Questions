#ifndef QUESTIONDATABASE_H
#define QUESTIONDATABASE_H

#include <iostream>
#include <fstream>
#include <new>
#include <string>
#include <chrono>
#include <vector>
#include <sstream>

#include "Question.h"
#include "Attribute.h"

using namespace std;

class QuestionDatabase
{
    public:
        QuestionDatabase();
        QuestionDatabase(vector<Question*> &questions);
        ~QuestionDatabase();
        vector<Question*> getQuestions() const;
        void initialize(const string &filename);
        Question searchText(const string &questionText) const;
        vector<Question*> searchAttributes(const vector<Attribute>& targetedAttributes) const;
        vector<Question*> searchAttributeIndexes(const vector<int>& attributeIndexes) const;
        void print() const;

    private:
        vector<Question*> questions;
};

#endif