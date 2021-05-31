#include <gtest/gtest.h>
#include <sstream>
#include <iostream>

#include "token.h"
#include "type.h"

TEST(TypeSuite, Basic)
{
    BasicCType type(
        BasicCTypeType::CHAR,
        BasicCTypeSignedness::SIGNED,
        BasicCTypeStorage::STATIC,
        BasicCTypeQualifier::CONST
    );
    ASSERT_EQ(static_cast<std::string>(type), std::string("[static const signed char]"));

    type = BasicCType(
        BasicCTypeType::INT,
        BasicCTypeSignedness::SIGNED,
        BasicCTypeStorage::STATIC
    );
    ASSERT_EQ(static_cast<std::string>(type), std::string("[static signed int]"));

    type = BasicCType(
        BasicCTypeType::SHORT_INT,
        BasicCTypeSignedness::UNSIGNED
    );
    ASSERT_EQ(static_cast<std::string>(type), std::string("[unsigned short int]"));
}

TEST(TypeSuite, Derived)
{
    BasicCType base(BasicCTypeType::INT, BasicCTypeSignedness::SIGNED);

    DerivedCType type(
        std::make_unique<BasicCType>(base),
        DerivedCTypeType::ARRAY,
        10,
        BasicCTypeQualifier::NOT_SET
    );
    ASSERT_EQ(static_cast<std::string>(type), std::string("[[10], [signed int]]"));

    type = DerivedCType(
        std::make_unique<BasicCType>(base),
        DerivedCTypeType::POINTER,
        BasicCTypeQualifier::CONST
    );
    ASSERT_EQ(static_cast<std::string>(type), std::string("[const *, [signed int]]"));
}