#include "Question.h"

Question::Question()
{
    index = 0;
    questionText = "DefaultQuestionText";
}

Question::Question(const int index, const string &questionText, const vector<Attribute> &targetedAttributes)
{
    this->index = index;
    this->questionText = questionText;
    this->targetedAttributes = targetedAttributes;
}

Question::Question(const Question &Question)
{
    index = Question.getIndex();
    questionText = Question.getQuestionText();
    targetedAttributes = Question.getTargetedAttributes();
}

int Question::getIndex() const
{
    return index;
}

string Question::getQuestionText() const
{
    return questionText;
}

vector<Attribute> Question::getTargetedAttributes() const
{
    return targetedAttributes;
}

void Question::setIndex(const int index)
{
    this->index = index;
}

void Question::setQuestionText(const string &questionText)
{
    this->questionText = questionText;
}

void Question::setTargetedAttributes(const vector<Attribute> &targetedAttributes)
{
    this->targetedAttributes = targetedAttributes;
}

void Question::print() const
{
    cout << "Index: " << getIndex() << " Name: " << getQuestionText() << " Targeted Attributes:";
    for(Attribute attribute : targetedAttributes)
    {
        cout << " " << attribute;
    }
    cout << endl;
}