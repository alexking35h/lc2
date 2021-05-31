#ifndef _CTYPE_H_
#define _CTYPE_H_

#include <ostream>

#include "token.h"

class CTypeError
{

};

enum class BasicCTypeType
{
    CHAR, SHORT_INT, INT, NOT_SET
};

enum class BasicCTypeSignedness
{
    SIGNED, UNSIGNED, NOT_SET
};

enum class BasicCTypeStorage
{
    STATIC, NOT_SET
};

enum class BasicCTypeQualifier
{
    CONST, NOT_SET
};

class CType
{
    public:
        virtual ~CType();
        virtual operator std::string() = 0;

};

std::ostream& operator<<(std::ostream&, CType&);

class BasicCType : public CType
{
    // Basic C Type (p34: 6.2.5, 14)
    // char, signed, unsigned integer types.
 
    public:
        BasicCTypeType type;
        BasicCTypeSignedness signedness;
        BasicCTypeStorage storage;
        BasicCTypeQualifier qualifier;

        inline BasicCType(
            BasicCTypeType type,
            BasicCTypeSignedness signedness,
            BasicCTypeStorage storage = BasicCTypeStorage::NOT_SET,
            BasicCTypeQualifier qualifier = BasicCTypeQualifier::NOT_SET)
            : type(type)
            , signedness(signedness)
            , storage(storage)
            , qualifier(qualifier) { }

        virtual operator std::string() override;
};

enum class DerivedCTypeType
{
    ARRAY, POINTER, FUNCTION
};

class DerivedCType : public CType
{
    // Derived C Types (35: 6.2.5, 20)
    // Array, struct, union, function, and pointer types. Can be constructed
    // recursively.
    public:
        std::unique_ptr<CType> base;
        BasicCTypeQualifier qualifier;
        DerivedCTypeType type;
        int array_size = 0;

        DerivedCType(
            std::unique_ptr<CType> base,
            DerivedCTypeType type,
            BasicCTypeQualifier qualifier = BasicCTypeQualifier::NOT_SET)
            : base(std::move(base))
            , type(type)
            , qualifier(qualifier) { }
        
        DerivedCType(
            std::unique_ptr<CType> base,
            DerivedCTypeType type,
            int array_size,
            BasicCTypeQualifier qualifier = BasicCTypeQualifier::NOT_SET)
            : DerivedCType(std::move(base), type, qualifier)
            {
                this->array_size = array_size;
            }

        virtual operator std::string() override;
};

class VoidCType : public CType
{
    // Void C Type (p35: 6.2.5, 19)
    // Comprises an empty set of values; it is an incomplete type that
    // cannot be completed.

    public:
        virtual operator std::string() override;
};

class CTypeBuilder
{
    public:
        void add_specifier(TokenType);
        void add_qualifier(TokenType);
        void add_storage_specifier(TokenType);
};
#endif