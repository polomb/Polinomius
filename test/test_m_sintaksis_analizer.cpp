#include "m_leksich_analizer.h"
#include "m_sintaksis_analizer.h"

#include <gtest.h>

static std::vector<std::unique_ptr<Token>> CreatingLeksich(const std::string& expression)
{
    std::unique_ptr<LeksichAnalizer> leksich;
    leksich = std::make_unique<LeksichAnalizer>(expression);
    std::vector<std::unique_ptr<Token>> tokens = leksich->tokenize();
    return tokens;
}
static std::unique_ptr<SintaksisAnalizer> CreatingSintaksis()
{
    std::unique_ptr<SintaksisAnalizer> sintaksis;
    sintaksis = std::make_unique<SintaksisAnalizer>();
    return sintaksis;
}

TEST(SintaksisAnalizerTest, Works_1)
{
    std::string expression = "1+1";
    std::unordered_map<std::string, Polinom> perem;
    //
    std::vector<std::unique_ptr<Token>> tokens = CreatingLeksich(expression);
    std::unique_ptr<SintaksisAnalizer> sintaksis = CreatingSintaksis();
    EXPECT_NO_THROW(sintaksis->isCorrect(tokens, perem));
    //
    //ADD_FAILURE();
}
TEST(SintaksisAnalizerTest, Works_2)
{
    std::string expression = "1+1+1+1*7+12/14-15+18";
    std::unordered_map<std::string, Polinom> perem;
    //
    std::vector<std::unique_ptr<Token>> tokens = CreatingLeksich(expression);
    std::unique_ptr<SintaksisAnalizer> sintaksis = CreatingSintaksis();
    EXPECT_NO_THROW(sintaksis->isCorrect(tokens, perem));
    //
    //ADD_FAILURE();
}
TEST(SintaksisAnalizerTest, Works_3)
{
    std::string expression = "(1-3)+(12*4)*((15+296))+(12*(123+15)+13)";
    std::unordered_map<std::string, Polinom> perem;
    //
    std::vector<std::unique_ptr<Token>> tokens = CreatingLeksich(expression);
    std::unique_ptr<SintaksisAnalizer> sintaksis = CreatingSintaksis();
    EXPECT_NO_THROW(sintaksis->isCorrect(tokens, perem));
    //
    //ADD_FAILURE();
}
TEST(SintaksisAnalizerTest, Works_4)
{
    std::string expression = "xxx + yyy + zzz + 123.123zyx/xy";
    std::unordered_map<std::string, Polinom> perem;
    //
    std::vector<std::unique_ptr<Token>> tokens = CreatingLeksich(expression);
    std::unique_ptr<SintaksisAnalizer> sintaksis = CreatingSintaksis();
    EXPECT_NO_THROW(sintaksis->isCorrect(tokens, perem));
    //
    //ADD_FAILURE();
}
TEST(SintaksisAnalizerTest, Drops_1)
{
    std::string expression = "()";
    std::unordered_map<std::string, Polinom> perem;
    //
    std::vector<std::unique_ptr<Token>> tokens = CreatingLeksich(expression);
    std::unique_ptr<SintaksisAnalizer> sintaksis = CreatingSintaksis();
    EXPECT_ANY_THROW(sintaksis->isCorrect(tokens, perem));
    //
    //ADD_FAILURE();
}
TEST(SintaksisAnalizerTest, Drops_2)
{
    std::string expression = ")(";
    std::unordered_map<std::string, Polinom> perem;
    //
    std::vector<std::unique_ptr<Token>> tokens = CreatingLeksich(expression);
    std::unique_ptr<SintaksisAnalizer> sintaksis = CreatingSintaksis();
    EXPECT_ANY_THROW(sintaksis->isCorrect(tokens, perem));
    //
    //ADD_FAILURE();
}
TEST(SintaksisAnalizerTest, Drops_3)
{
    std::string expression = "+1";
    std::unordered_map<std::string, Polinom> perem;
    //
    std::vector<std::unique_ptr<Token>> tokens = CreatingLeksich(expression);
    std::unique_ptr<SintaksisAnalizer> sintaksis = CreatingSintaksis();
    EXPECT_ANY_THROW(sintaksis->isCorrect(tokens, perem));
    //
    //ADD_FAILURE();
}
TEST(SintaksisAnalizerTest, Drops_4)
{
    std::string expression = "1+";
    std::unordered_map<std::string, Polinom> perem;
    //
    std::vector<std::unique_ptr<Token>> tokens = CreatingLeksich(expression);
    std::unique_ptr<SintaksisAnalizer> sintaksis = CreatingSintaksis();
    EXPECT_ANY_THROW(sintaksis->isCorrect(tokens, perem));
    //
    //ADD_FAILURE();
}
TEST(SintaksisAnalizerTest, Drops_5)
{
    std::string expression = "ln()";
    std::unordered_map<std::string, Polinom> perem;
    //
    std::vector<std::unique_ptr<Token>> tokens = CreatingLeksich(expression);
    std::unique_ptr<SintaksisAnalizer> sintaksis = CreatingSintaksis();
    EXPECT_ANY_THROW(sintaksis->isCorrect(tokens, perem));
    //
    //ADD_FAILURE();
}
TEST(SintaksisAnalizerTest, Drops_6)
{
    std::string expression = "exp()";
    std::unordered_map<std::string, Polinom> perem;
    //
    std::vector<std::unique_ptr<Token>> tokens = CreatingLeksich(expression);
    std::unique_ptr<SintaksisAnalizer> sintaksis = CreatingSintaksis();
    EXPECT_ANY_THROW(sintaksis->isCorrect(tokens, perem));
    //
    //ADD_FAILURE();
}
TEST(SintaksisAnalizerTest, Drops_7)
{
    std::string expression = "-";
    std::unordered_map<std::string, Polinom> perem;
    //
    std::vector<std::unique_ptr<Token>> tokens = CreatingLeksich(expression);
    std::unique_ptr<SintaksisAnalizer> sintaksis = CreatingSintaksis();
    EXPECT_ANY_THROW(sintaksis->isCorrect(tokens, perem));
    //
    //ADD_FAILURE();
}
TEST(SintaksisAnalizerTest, Drops_8)
{
    std::string expression = "(5+14(";
    std::unordered_map<std::string, Polinom> perem;
    //
    std::vector<std::unique_ptr<Token>> tokens = CreatingLeksich(expression);
    std::unique_ptr<SintaksisAnalizer> sintaksis = CreatingSintaksis();
    EXPECT_ANY_THROW(sintaksis->isCorrect(tokens, perem));
    //
    //ADD_FAILURE();
}
TEST(SintaksisAnalizerTest, Drops_9)
{
    std::string expression = "123.12.3";
    std::unordered_map<std::string, Polinom> perem;
    //
    std::vector<std::unique_ptr<Token>> tokens = CreatingLeksich(expression);
    std::unique_ptr<SintaksisAnalizer> sintaksis = CreatingSintaksis();
    EXPECT_ANY_THROW(sintaksis->isCorrect(tokens, perem));
    //
    //ADD_FAILURE();
}