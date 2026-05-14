#include "Polinom.h"

#include <gtest.h>

TEST(Polinoms, DefaultConstructor)
{
    Polinom p;
    std::ostringstream oss;
    oss << p;

    EXPECT_EQ(oss.str(), "0");
}

TEST(Polinoms, CopyConstructor)
{
    Polinom original(1.0, 1, 1, 0);
    Polinom copy(original);

    std::ostringstream oss1, oss2;
    oss1 << original;
    oss2 << copy;
    EXPECT_EQ(oss1.str(), oss2.str());
}

TEST(Polinoms, MoveConstructor)
{
    Polinom original(1.0, 1, 1, 0);
    Polinom moved(std::move(original));

    std::ostringstream oss;
    oss << moved;
    EXPECT_FALSE(oss.str().empty());
}

TEST(Polinoms, AssignmentOperator)
{
    Polinom p1(1.0, 1, 1, 0);
    Polinom p2;
    p2 = p1;

    std::ostringstream oss1, oss2;
    oss1 << p1;
    oss2 << p2;
    EXPECT_EQ(oss1.str(), oss2.str());
}

TEST(Polinoms, MoveAssignmentOperator)
{
    Polinom p1(1.0, 1, 1, 0);
    Polinom p2;
    p2 = std::move(p1);

    std::ostringstream oss;
    oss << p2;
    EXPECT_FALSE(oss.str().empty());
}

TEST(Polinoms, Addition)
{
    Polinom p1(1.0, 1, 2, 2);
    Polinom p2(1.0, 2, 2, 3);
    Polinom result = p1 + p2;
    std::ostringstream oss;
    oss << result;
    EXPECT_TRUE(oss.str().find("2") != std::string::npos);
    EXPECT_TRUE(oss.str().find("3") != std::string::npos);
}

TEST(Polinoms, AdditionWithZero)
{
    Polinom p1(1.0, 1, 2, 2);
    Polinom p2(0.0, 0, 0, 0);
    Polinom result = p1 + p2;
    std::ostringstream oss1, oss2;
    oss1 << p1;
    oss2 << result;
    EXPECT_EQ(oss1.str(), oss2.str());
}

TEST(Polinoms, Subtraction)
{
    Polinom p1(5.0, 0, 1, 0);
    Polinom p2(3.0, 0, 1, 0);
    Polinom result = p2 - p1;
    std::ostringstream oss;
    oss << result;
    EXPECT_TRUE(oss.str().find("2.000000y") != std::string::npos);
}

TEST(Polinoms, SubtractionToZero)
{
    Polinom p1(5.0, 0, 1, 0);
    Polinom result = p1 - p1;
    std::ostringstream oss;
    oss << result;
    EXPECT_EQ(oss.str(), "0");
}

TEST(Polinoms, SubtractionWithZero)
{
    Polinom p1(5.0, 0, 1, 0);
    Polinom p2(0.0, 0, 0, 0);
    Polinom result = p1 - p2;
    std::ostringstream oss1, oss2;
    oss1 << p1;
    oss2 << result;
    EXPECT_EQ(oss1.str(), oss2.str());
}

TEST(Polinoms, Multiplication)
{
    Polinom p1(1.0, 0, 1, 0);
    Polinom p2(2.0, 0, 1, 0);
    Polinom result = p1 * p2;
    std::ostringstream oss;
    oss << result;
    EXPECT_TRUE(oss.str().find("y^2") != std::string::npos);
}

TEST(Polinoms, MultiplicationByZero)
{
    Polinom p1(1.0, 0, 1, 0);
    Polinom p2(0.0, 0, 0, 0);
    Polinom result = p1 * p2;
    std::ostringstream oss;
    oss << result;
    EXPECT_EQ(oss.str(), "0");
}

TEST(Polinoms, MultiplicationSquare)
{
    Polinom p1(1.0, 0, 2, 0);
    Polinom result = p1 * p1;
    std::ostringstream oss;
    oss << result;
    EXPECT_TRUE(oss.str().find("y^4") != std::string::npos);
}

TEST(Polinoms, MultiplicationByConstant)
{
    Polinom p1(1.0, 0, 2, 0);
    Polinom result = p1 * 2.0;
    std::ostringstream oss;
    oss << result;
    EXPECT_TRUE(oss.str().find("2.000000y") != std::string::npos);
}

TEST(Polinoms, MultiplicationByZeroConstant)
{
    Polinom p1(1.0, 0, 2, 0);
    Polinom result = p1 * 0.0;
    std::ostringstream oss;
    oss << result;
    EXPECT_EQ(oss.str(), "0");
}

TEST(Polinoms, DivisionByConstant)
{
    Polinom p1(3.0, 0, 1, 0);
    Polinom result = p1 / 2.0;
    std::ostringstream oss;
    oss << result;
    EXPECT_TRUE(oss.str().find("1.500000y") != std::string::npos);
}

TEST(Polinoms, DivisionByZero)
{
    Polinom p1(3.0, 0, 1, 0);
    EXPECT_ANY_THROW(p1 / 0.0);
}

TEST(Polinoms, SimpleMonom)
{
    Polinom p(5.0, 2, 3, 4);
    std::ostringstream oss;
    oss << p;
    EXPECT_TRUE(oss.str().find("5") != std::string::npos);
    EXPECT_TRUE(oss.str().find("x^2") != std::string::npos);
    EXPECT_TRUE(oss.str().find("y^3") != std::string::npos);
    EXPECT_TRUE(oss.str().find("z^4") != std::string::npos);
}

TEST(Polinoms, NegativeCoefficients)
{
    Polinom p(-3.0, 0, 1, 0);
    std::ostringstream oss;
    oss << p;
    EXPECT_TRUE(oss.str().find("-3.000000y") != std::string::npos);
}

TEST(Polinoms, DecimalCoefficients)
{
    Polinom p(1.5, 1, 0, 0);
    std::ostringstream oss;
    oss << p;
    EXPECT_TRUE(oss.str().find("1.500000x") != std::string::npos);
}

TEST(Polinoms, Associativity)
{
    Polinom p1(1.0, 1, 2, 8);
    Polinom p2(2.5, 1, 2, 8);
    Polinom p3(8.5, 1, 2, 8);
    Polinom r1 = (p1 + p2) + p3;
    Polinom r2 = p1 + (p2 + p3);

    std::ostringstream oss1, oss2;
    oss1 << r1;
    oss2 << r2;
    EXPECT_EQ(oss1.str(), oss2.str());
}

TEST(Polinoms, Commutativity)
{
    Polinom p1(1.5, 1, 2, 8);
    Polinom p2(1.5, 8, 3, 12);
    Polinom r1 = p1 * p2;
    Polinom r2 = p2 * p1;

    std::ostringstream oss1, oss2;
    oss1 << r1;
    oss2 << r2;
    EXPECT_EQ(oss1.str(), oss2.str());
}

TEST(Polinoms, ZeroPolinomOperations)
{
    Polinom zero;
    Polinom p(1.5, 1, 2, 8);

    Polinom sum = zero + p;
    Polinom diff = p - zero;
    Polinom prod = zero * p;

    std::ostringstream oss_sum, oss_diff, oss_prod, oss_p;
    oss_sum << sum;
    oss_diff << diff;
    oss_prod << prod;
    oss_p << p;

    EXPECT_EQ(oss_sum.str(), oss_p.str());
    EXPECT_EQ(oss_diff.str(), oss_p.str());
    EXPECT_EQ(oss_prod.str(), "0");
}
TEST(Polinoms, Division)
{
    Polinom p1(1.0, 1, 0, 0);
    Polinom p2(1.0, 0, 1, 0);
    Polinom r1 = ((p1 + p2)/(p1 + p2));

    std::ostringstream oss;
    oss << r1;
    EXPECT_TRUE(oss.str().find("1.000000") != std::string::npos);
}
TEST(Polinoms, Division2)
{
    Polinom p1(1.0, 2, 0, 0);
    Polinom p2(1.0, 1, 0, 0);
    Polinom p3(1.0, 0, 0, 0);
    Polinom r1 = ((p1 + Polinom(2, 0, 0, 0) * p2 + p3) / (p2 + p3));

    std::ostringstream oss;
    oss << r1;
    std::cout << r1 << '\n';
    EXPECT_TRUE(oss.str().find("x + 1.000000") != std::string::npos);
}
TEST(Polinoms, Division3)
{
    //((x-1)*(x-1)*(x+1) + x + 3)/(x+1) = x^2 -2x + 2
    Polinom p2(1.0, 1, 0, 0);
    Polinom p3(1.0, 0, 0, 0);
    Polinom r1 = (((p2 - p3)*(p2 - p3)*(p2 + p3) + p2 + Polinom(3, 0, 0, 0)) / (p2 + p3));

    std::ostringstream oss;
    oss << r1;
    std::cout << r1 << '\n';
    EXPECT_TRUE(oss.str().find("x^2 - 2.000000x + 2.000000") != std::string::npos);
}
TEST(Polinoms, Division4)
{
    Polinom p2(1.0, 1, 0, 0);
    Polinom p3(1.0, 0, 0, 0);
    Polinom r1 = (p2 - p3);

    std::ostringstream oss;
    oss << r1;
    std::cout << r1 << '\n';
    EXPECT_TRUE(oss.str().find("x - 1.000000") != std::string::npos);
}