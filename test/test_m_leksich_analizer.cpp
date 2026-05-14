#include "m_leksich_analizer.h"

#include <gtest.h>

static std::vector<std::unique_ptr<Token>> CreatingLeksich(const std::string& expression)
{
    std::unique_ptr<LeksichAnalizer> leksich;
    leksich = std::make_unique<LeksichAnalizer>(expression);
    std::vector<std::unique_ptr<Token>> tokens = leksich->tokenize();
    return tokens;
}

TEST(LeksichAnalizerTest, CanMakeNumberTokens)
{
    std::string expression = "123.123";
    std::vector<std::unique_ptr<Token>> tokens = CreatingLeksich(expression);
    EXPECT_EQ(TokenType::POLINOM, tokens[0]->getType());
    //ADD_FAILURE();
}
TEST(LeksichAnalizerTest, CanMakeVariousNumberTokens)
{
    std::string expression = ".123 12.";
    std::vector<std::unique_ptr<Token>> tokens = CreatingLeksich(expression);
    EXPECT_EQ(TokenType::POLINOM, tokens[0]->getType());
    EXPECT_EQ(TokenType::POLINOM, tokens[1]->getType());
    //ADD_FAILURE();
}
TEST(LeksichAnalizerTest, CanMakeAllTwo_Arg_OperationTokens)
{
    std::string expression = "+ - * /";
    std::vector<std::unique_ptr<Token>> tokens = CreatingLeksich(expression);
    EXPECT_EQ(TokenType::OPERATION, tokens[0]->getType());
    EXPECT_EQ(TokenType::OPERATION, tokens[1]->getType());
    EXPECT_EQ(TokenType::OPERATION, tokens[2]->getType());
    EXPECT_EQ(TokenType::OPERATION, tokens[3]->getType());
    //ADD_FAILURE();
}
TEST(LeksichAnalizerTest, CanMakeAllOne_Arg_OperationTokens)
{
    std::string expression = "- 5";
    std::vector<std::unique_ptr<Token>> tokens = CreatingLeksich(expression);
    EXPECT_EQ(TokenType::ONE_ARG_OPER, tokens[0]->getType());
    //ADD_FAILURE();
}
TEST(LeksichAnalizerTest, CanMakePeremTokens)
{
    std::string expression = "perem_1";
    std::vector<std::unique_ptr<Token>> tokens = CreatingLeksich(expression);
    EXPECT_EQ(TokenType::PEREMENNAYA, tokens[0]->getType());
    //ADD_FAILURE();
}
TEST(LeksichAnalizerTest, CanMakeOpen_SkobkaTokens)
{
    std::string expression = "(";
    std::vector<std::unique_ptr<Token>> tokens = CreatingLeksich(expression);
    EXPECT_EQ(TokenType::OPEN_SKOBKA, tokens[0]->getType());
    //ADD_FAILURE();
}
TEST(LeksichAnalizerTest, CanMakeClosed_SkobkaTokens)
{
    std::string expression = ")";
    std::vector<std::unique_ptr<Token>> tokens = CreatingLeksich(expression);
    EXPECT_EQ(TokenType::CLOSED_SKOBKA, tokens[0]->getType());
    //ADD_FAILURE();
}
TEST(LeksichAnalizerTest, CanMakeEquationTokens)
{
    std::string expression = "=";
    std::vector<std::unique_ptr<Token>> tokens = CreatingLeksich(expression);
    EXPECT_EQ(TokenType::EQUATION, tokens[0]->getType());
    //ADD_FAILURE();
}