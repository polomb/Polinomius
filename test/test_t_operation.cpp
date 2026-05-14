#include "t_operation.h"

#include <gtest.h>

TEST(OperationTest, DefaultConstructor)
{
    TOperation op("+", Arguments::TWO);

    EXPECT_EQ(op.getOperation(), "+");
    EXPECT_EQ(op.getArgument(), Arguments::TWO);
    EXPECT_EQ(op.getType(), TokenType::OPERATION);
}

TEST(OperationTest, OneArg_OneArgOper)
{
    TOperation op("-", Arguments::ONE);
    EXPECT_EQ(op.getType(), TokenType::ONE_ARG_OPER);
}

TEST(OperationTest, PosledovIsCorrect)
{
    TOperation op1("+", Arguments::TWO);
    EXPECT_EQ(op1.getPosledov(), 1);

    TOperation op2("*", Arguments::TWO);
    EXPECT_EQ(op2.getPosledov(), 2);

    TOperation un_minus("-", Arguments::ONE);
    EXPECT_EQ(un_minus.getPosledov(), 5);
}

TEST(OperationTest, SetArgumentChangesPosledov)
{
    TOperation op("-", Arguments::TWO);
    EXPECT_EQ(op.getPosledov(), 1);

    op.setArgument(Arguments::ONE);
    EXPECT_EQ(op.getPosledov(), 5);
}

TEST(OperationTest, Associativity)
{
    TOperation pow("^", Arguments::TWO);
    EXPECT_FALSE(pow.isLeftAssociative());

    TOperation add("+", Arguments::TWO);
    EXPECT_TRUE(add.isLeftAssociative());
}