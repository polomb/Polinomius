#include "t_skobki.h"

#include <gtest.h>

TEST(SkobkiTest, OpenSkobka)
{
    TSkobki sk('(');
    EXPECT_TRUE(sk.isOpenSkobka());
    EXPECT_EQ(sk.getSkobka(), '(');
    EXPECT_EQ(sk.getType(), TokenType::OPEN_SKOBKA);
}

TEST(SkobkiTest, ClosedSkobka)
{
    TSkobki sk(')');
    EXPECT_FALSE(sk.isOpenSkobka());
    EXPECT_EQ(sk.getSkobka(), ')');
    EXPECT_EQ(sk.getType(), TokenType::CLOSED_SKOBKA);
}