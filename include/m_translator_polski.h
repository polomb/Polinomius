#ifndef __M_TRANSLATOR_POLSKI__
#define __M_TRANSLATOR_POLSKI__

#include <vector>
#include <memory>
#include <unordered_map>
#include <string>
#include <stdexcept>
#include <cmath>
#include <stack>

#include "token.h"
#include "t_polinom.h"
#include "t_operation.h"
#include "t_peremennaya.h"
#include "t_skobki.h"
#include "Polinom.h"
#include "SortedMap.h"
#include "BlackRedTree.h"
#include "Hash.h"

class TranslatorPolski
{
    bool shouldPopOperator(const Token* oper1, const Token* oper2) const;
public:
    TranslatorPolski();

    std::vector<std::unique_ptr<Token>> toPolishNotation(const std::vector<std::unique_ptr<Token>>& tokens);

    template<typename ContainerType>
    Polinom calculate(const std::vector<std::unique_ptr<Token>>& polishTokens, ContainerType& perem)
    {
        std::stack<Polinom> values;

        bool isThereEquation = false;
        auto start = polishTokens.begin();
        if (polishTokens.size() >= 2)
        {
            if (polishTokens[1]->getType() == TokenType::EQUATION)
            {
                isThereEquation = true;
                start = polishTokens.begin() + 2;
            }
            if (polishTokens[0]->getType() == TokenType::TILDA)
            {
                perem.erase(dynamic_cast<const TPeremennaya*>(polishTokens[1].get())->getPeremennaya());
                Polinom result(1, 0, 0, 0);
                values.push(result);
            }
        }

        for (auto it = start; it != polishTokens.end(); ++it)
        {
            const auto& token = *it;
            TokenType type = token->getType();

            if (type == TokenType::POLINOM)
            {
                const TPolinom* poli = dynamic_cast<const TPolinom*>(token.get());
                values.push(poli->getPolinomValue());
            }
            else if (type == TokenType::OPERATION)
            {
                const TOperation* op = dynamic_cast<const TOperation*>(token.get());

                if (op->getArgument() == Arguments::TWO)
                {
                    if (values.size() < 2)
                        throw std::runtime_error("TRANSLATOR_OPERATION");

                    Polinom b = values.top();
                    values.pop();
                    Polinom a = values.top();
                    values.pop();
                    Polinom result = Polinom();

                    std::string operation = op->getOperation();

                    if (operation == "+")
                        result = a + b;
                    else if (operation == "-")
                        result = a - b;
                    else if (operation == "*")
                        result = a * b;
                    else
                        throw std::runtime_error("TRANSLATOR(BIG_ERROR): " + operation);

                    values.push(result);
                }
            }
            else if (type == TokenType::ONE_ARG_OPER)
            {
                const TOperation* op = dynamic_cast<const TOperation*>(token.get());

                if (values.empty())
                    throw std::runtime_error("TRANSLATOR(BIG_ERROR)_ONE_ARG");

                Polinom a = values.top();
                values.pop();
                Polinom result = Polinom();
                std::string operation = op->getOperation();
                if (operation == "-")
                    result = -a;
                values.push(result);
            }
            else if (type == TokenType::PEREMENNAYA)
            {
                const TPeremennaya* var = dynamic_cast<const TPeremennaya*>(token.get());
                std::string name = var->getPeremennaya();

                if (perem.find(name) != perem.end())
                {
                    values.push(perem[name]);
                }
            }
        }

        if (values.size() != 1)
            throw std::runtime_error("TRANSLATOR_Stack is not empty " +
                std::to_string(values.size()) + " values");

        if (isThereEquation)
        {
            perem[dynamic_cast<const TPeremennaya*>(polishTokens[0].get())->getPeremennaya()] = values.top();
        }

        return values.top();
    }
};

#endif

