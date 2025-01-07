#include "Attribute.h"
Attribute::Attribute()
{
    index = -1;
    attribute = "None";
}

Attribute::Attribute(int index, string attribute)
{
    this->index = index;
    this->attribute = attribute;
}

int Attribute::getIndex() const
{
    return index;
}

string Attribute::getAttribute() const
{
    return attribute;
}

void Attribute::setIndex(int index)
{
    this->index = index;
}

void Attribute::setAttribute(string attribute)
{
    this->attribute = attribute;
}

ostream& operator<<(ostream& os, const Attribute& attribute)
{
    os << attribute.getAttribute();
    return os;
}

bool Attribute::operator==(const Attribute& other) const
{
    return index == other.index;
}
