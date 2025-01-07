#include "QuestionDatabase.h"

QuestionDatabase::QuestionDatabase(){}

QuestionDatabase::QuestionDatabase(vector<Question*> &questions)
{
    this->questions = questions;
}

QuestionDatabase::~QuestionDatabase()
{
    for(Question* question : questions)
    {
        delete question;
    }
}

vector<Question*> QuestionDatabase::getQuestions() const
{
    return questions;
}

void QuestionDatabase::initialize(const string &filename)
{
    ifstream questionFile(filename);
    if (!questionFile.is_open()) 
    {
        cerr << "Failed to open the file: " << filename << endl;
        return;
    }
    string line;
    int questionIndex = 0;
    while(getline(questionFile, line))
    {
        istringstream questionStream(line);
        string questionText;
        getline(questionStream, questionText, ':');
        vector<Attribute> targetedAttributes;
        int index;
        string targetedAttribute;
        while(questionStream >> index >> targetedAttribute)
        {
            Attribute attribute((index - 1), targetedAttribute);
            targetedAttributes.push_back(attribute);
        }
        Question* question = new Question(questionIndex, questionText, targetedAttributes);
        questions.push_back(question);
        questionIndex++;
    }
}

Question QuestionDatabase::searchText(const string &questionText) const
{
    Question returnQuestion;
    for(Question* question : questions)
    {
        if(question->getQuestionText() == questionText)
        {
            returnQuestion = *question;
            break;
        }
    }
    return returnQuestion;
}

vector<Question*> QuestionDatabase::searchAttributes(const vector<Attribute>& targetedAttributes) const
{
    vector<Question*> returnQuestionVector;
    for(Question* question : questions)
    {
        bool targetedAttributesInQuestion = true;
        vector<Attribute> questionTargetedAttributes = question->getTargetedAttributes();
        for(Attribute targetedAttribute : targetedAttributes)
        {
            if(find(questionTargetedAttributes.begin(), questionTargetedAttributes.end(), targetedAttribute) == questionTargetedAttributes.end())
            {
                targetedAttributesInQuestion = false;
                break;
            }
        }
        if(targetedAttributesInQuestion)
        {
            returnQuestionVector.push_back(question);
        }
    }
    return returnQuestionVector;
}

vector<Question*> QuestionDatabase::searchAttributeIndexes(const vector<int>& attributeIndexes) const
{
    vector<Question*> returnQuestionVector;
    for(Question* question : questions)
    {
        vector<Attribute> questionTargetedAttributes = question->getTargetedAttributes();
        vector<int> questionTargetedAttributesIndexes;
        for(Attribute targetedAttribute : questionTargetedAttributes)
        {
            questionTargetedAttributesIndexes.push_back(targetedAttribute.getIndex());
        }
        
        for(int i : attributeIndexes)
        {
            if(i == questionTargetedAttributesIndexes[0] && find(returnQuestionVector.begin(), returnQuestionVector.end(), question) == returnQuestionVector.end())
            {
                returnQuestionVector.push_back(question);
            }
        }
    }
    return returnQuestionVector;
}

void QuestionDatabase::print() const
{
    for(Question* question: questions)
    {
        question->print();
    }
    cout << endl;
}