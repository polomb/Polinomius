#include "t_operation.h"

TOperation::TOperation(std::string operation, Arguments argument) : oper(operation), argum(argument)
{
    posledov = 0;
    if (operation == "+" || operation == "-")
    {
        posledov = 1;
    }
    if (operation == "*" || operation == "/")
    {
        posledov = 2;
    }
    if (operation == "^")
    {
        posledov = 3;
    }
    if ((operation == "exp" || operation == "ln") && argum == Arguments::ONE)
    {
        posledov = 4;
    }
    if (operation == "-" && argum == Arguments::ONE)
    {
        posledov = 5;
    }
}
TokenType TOperation::getType() const 
{
    if(argum == Arguments::TWO)
        return TokenType::OPERATION;
    else
        return TokenType::ONE_ARG_OPER;
}
std::string TOperation::getOperation() const
{
    return oper;
}
int TOperation::getPosledov() const
{
    return posledov;
}
Arguments TOperation::getArgument() const
{
    return argum;
}
void TOperation::setArgument(Arguments argument)
{
    argum = argument;
    if (argum == Arguments::ONE)
    {
        if (oper == "-")
            posledov = 5;
        else if (oper == "ln" || oper == "exp")
            posledov = 4;
        else
            posledov = 0;
        return;
    }
    if (oper == "+" || oper == "-") posledov = 1;
    else if (oper == "*" || oper == "/") posledov = 2;
    else if (oper == "^") posledov = 3;
    else posledov = 0;
}
bool TOperation::isLeftAssociative() const
{
    return oper != "^";
}
//
//
//
TEquation::TEquation(char eq) : equal(eq)
{
    
}
TokenType TEquation::getType() const
{
    return TokenType::EQUATION;
}
char TEquation::getEquationValue() const
{
    return equal;
}
//
//
//
TTilda::TTilda(char tl) : tilda(tl)
{

}
TokenType TTilda::getType() const
{
    return TokenType::TILDA;
}
char TTilda::getTildaValue() const
{
    return tilda;
}

