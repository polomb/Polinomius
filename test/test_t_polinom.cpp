#include "t_polinom.h"

#include <gtest.h>

TEST(NumberTest, DefaultConstructor)
{
    Polinom poli(123.456, 0, 0, 0);
    TPolinom num(poli);
    EXPECT_EQ(num.getPolinomValue(), poli);
}
TEST(NumberTest, CorrectType)
{
    Polinom poli(123.456, 0, 0, 0);
    TPolinom num(poli);
    EXPECT_EQ(num.getType(), TokenType::POLINOM);
}