#include "t_peremennaya.h"

#include <gtest.h>

TEST(PeremennayaTest, DefaultConstructor)
{
    TPeremennaya var("abc123");
    EXPECT_EQ(var.getPeremennaya(), "abc123");
}

TEST(PeremennayaTest, CorrectType)
{
    TPeremennaya var("u");
    EXPECT_EQ(var.getType(), TokenType::PEREMENNAYA);
}