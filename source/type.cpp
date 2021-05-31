#include <sstream>

#include "type.h"

CType::~CType() {
}

BasicCType::operator std::string()
{
    std::stringstream str;
    str << "[";
    switch(storage)
    {
        case BasicCTypeStorage::STATIC:
            str << "static ";
            break;
    }
    switch(qualifier)
    {
        case BasicCTypeQualifier::CONST:
            str << "const ";
            break;
    }
    switch(signedness)
    {
        case BasicCTypeSignedness::SIGNED:
            str << "signed ";
            break;
        case BasicCTypeSignedness::UNSIGNED:
            str << "unsigned ";
            break;
    }
    switch(type)
    {
        case BasicCTypeType::CHAR:
            str << "char";
            break;
        case BasicCTypeType::SHORT_INT:
            str << "short int";
            break;
        case BasicCTypeType::INT:
            str << "int";
            break;
    }
    str << "]";
    return str.str();
}

DerivedCType::operator std::string()
{
    std::stringstream str;
    str << "[";
    switch(qualifier)
    {
        case BasicCTypeQualifier::CONST:
            str << "const ";
            break;
    }
    switch(type)
    {
        case DerivedCTypeType::ARRAY:
            str << "[" << array_size << "], ";
            break;
        case DerivedCTypeType::POINTER:
            str << "*, ";
            break;
        case DerivedCTypeType::FUNCTION:
            str << "(), ";
            break;
    }
    str << *base << "]";
    return str.str();
}

std::ostream& operator<<(std::ostream& os, CType& ctype)
{
    os << static_cast<std::string>(ctype);
    return os;
}