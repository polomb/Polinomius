#include "m_translator_polski.h"

#include <string>
#include <vector>
#include <stdexcept>
#include <cmath>
#include <stack>

#include "token.h"
#include "t_polinom.h"
#include "t_operation.h"
#include "t_peremennaya.h"
#include "t_skobki.h"

TranslatorPolski::TranslatorPolski() {}

bool TranslatorPolski::shouldPopOperator(const Token* oper1, const Token* oper2) const
{
    if ((oper1->getType() != TokenType::OPERATION && oper1->getType() != TokenType::ONE_ARG_OPER) ||
        (oper2->getType() != TokenType::OPERATION && oper2->getType() != TokenType::ONE_ARG_OPER))
        return false;

    const TOperation* op1 = dynamic_cast<const TOperation*>(oper1);
    const TOperation* op2 = dynamic_cast<const TOperation*>(oper2);

    int p1 = op1->getPosledov();
    int p2 = op2->getPosledov();

    bool op1Unary = (oper1->getType() == TokenType::ONE_ARG_OPER);
    bool op2Unary = (oper2->getType() == TokenType::ONE_ARG_OPER);

    if (!op1Unary && op2Unary)
        return false;

    if (op1Unary && !op2Unary)
        return false;

    if (p1 > p2)
        return true;

    if (p1 == p2 && op2->isLeftAssociative())
        return true;

    return false;
}
std::vector<std::unique_ptr<Token>> TranslatorPolski::toPolishNotation(const std::vector<std::unique_ptr<Token>>& tokens)
{
    std::vector<std::unique_ptr<Token>> output;
    std::stack<std::unique_ptr<Token>> operatorsStack;

    auto start = tokens.begin();
    if (tokens.size() >= 2)
    {
        if (tokens[1]->getType() == TokenType::EQUATION)
        {
            start = tokens.begin() + 2;
            output.push_back(std::make_unique<TPeremennaya>\
                (dynamic_cast<const TPeremennaya*>(tokens[0].get())->getPeremennaya()));
            output.push_back(std::make_unique<TEquation>\
                (dynamic_cast<const TEquation*>(tokens[1].get())->getEquationValue()));
        }
        if (tokens[0]->getType() == TokenType::TILDA)
        {
            output.push_back(std::make_unique<TTilda>\
                (dynamic_cast<const TTilda*>(tokens[0].get())->getTildaValue()));
            output.push_back(std::make_unique<TPeremennaya>\
                (dynamic_cast<const TPeremennaya*>(tokens[1].get())->getPeremennaya()));
        }
    }
    for (auto it = start; it != tokens.end(); ++it)
    {
        const auto& token = *it;
        TokenType type = token->getType();

        if (type == TokenType::POLINOM || type == TokenType::PEREMENNAYA)
        {
            if (type == TokenType::POLINOM)
            {
                const TPolinom* num = dynamic_cast<const TPolinom*>(token.get());
                output.push_back(std::make_unique<TPolinom>(num->getPolinomValue()));
            }
            else
            {
                const TPeremennaya* var = dynamic_cast<const TPeremennaya*>(token.get());
                output.push_back(std::make_unique<TPeremennaya>(var->getPeremennaya()));
            }
            while (!operatorsStack.empty() && operatorsStack.top()->getType() == TokenType::ONE_ARG_OPER)
            {
                const TOperation* topOp = dynamic_cast<const TOperation*>(operatorsStack.top().get());
                output.push_back(std::make_unique<TOperation>(topOp->getOperation(), topOp->getArgument()));
                operatorsStack.pop();
            }
        }
        else if (type == TokenType::OPERATION || type == TokenType::ONE_ARG_OPER)
        {
            const TOperation* op = dynamic_cast<const TOperation*>(token.get());
            while (!operatorsStack.empty() &&
                operatorsStack.top()->getType() != TokenType::OPEN_SKOBKA &&
                op->getType() == TokenType::OPERATION &&
                shouldPopOperator(operatorsStack.top().get(), token.get()))
            {
                const TOperation* topOp = dynamic_cast<const TOperation*>(operatorsStack.top().get());
                output.push_back(std::make_unique<TOperation>(topOp->getOperation(), topOp->getArgument()));
                operatorsStack.pop();
            }

            operatorsStack.push(std::make_unique<TOperation>(op->getOperation(), op->getArgument()));
        }
        else if (type == TokenType::OPEN_SKOBKA)
        {
            const TSkobki* skobka = dynamic_cast<const TSkobki*>(token.get());
            operatorsStack.push(std::make_unique<TSkobki>(skobka->getSkobka()));
        }
        else if (type == TokenType::CLOSED_SKOBKA)
        {
            while (!operatorsStack.empty() && operatorsStack.top()->getType() != TokenType::OPEN_SKOBKA)
            {
                const TOperation* topOp = dynamic_cast<const TOperation*>(operatorsStack.top().get());
                if (topOp->getType() == TokenType::OPERATION || topOp->getType() == TokenType::ONE_ARG_OPER)
                    output.push_back(std::make_unique<TOperation>(topOp->getOperation(), topOp->getArgument()));
                operatorsStack.pop();
            }
            if (!operatorsStack.empty() && operatorsStack.top()->getType() == TokenType::OPEN_SKOBKA)
                operatorsStack.pop();
            while (!operatorsStack.empty() && operatorsStack.top()->getType() == TokenType::ONE_ARG_OPER)
            {
                const TOperation* topOp = dynamic_cast<const TOperation*>(operatorsStack.top().get());
                output.push_back(std::make_unique<TOperation>(topOp->getOperation(), topOp->getArgument()));
                operatorsStack.pop();
            }
        }
    }
    while (!operatorsStack.empty())
    {
        const TOperation* topOp = dynamic_cast<const TOperation*>(operatorsStack.top().get());
        if (topOp->getType() == TokenType::OPERATION || topOp->getType() == TokenType::ONE_ARG_OPER)
            output.push_back(std::make_unique<TOperation>(topOp->getOperation(), topOp->getArgument()));
        operatorsStack.pop();
    }

    return output;
}

