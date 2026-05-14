#ifndef __M_SINTAKSIS_ANALIZER__
#define __M_SINTAKSIS_ANALIZER__

#include <memory>
#include <unordered_map>
#include <string>//For string and +
#include <vector>//For vector and +
#include <stdexcept>
#include <cctype> //For isdigit and isalpha
#include <stack> //For stack and +
#include <algorithm> //For find

#include "token.h"
#include "t_polinom.h"
#include "t_operation.h"
#include "t_peremennaya.h"
#include "t_skobki.h"
#include "Polinom.h"
#include "SortedMap.h"
#include "BlackRedTree.h"
#include "Hash.h"

class SintaksisAnalizer
{
public:
    SintaksisAnalizer();
    //
    template<typename ContainerType>
    bool isCorrect(const std::vector<std::unique_ptr<Token>>& tokens, ContainerType& perem)
    {
        if (tokens.size() == 1)
        {
            TokenType cur = tokens[0]->getType();
            if (cur == TokenType::POLINOM || cur == TokenType::PEREMENNAYA)
            {
                if (cur == TokenType::PEREMENNAYA)
                {
                    const TPeremennaya* var = dynamic_cast<const TPeremennaya*>(tokens[0].get());
                    if (perem.find(var->getPeremennaya()) != perem.end())
                    {

                    }
                    else
                        throw std::runtime_error("Sintaksis_There is no such perem");
                }
            }
            else
            {
                throw std::runtime_error("Sintaksis_One token can be only POLINOM or peremennaya");
            }
        }
        else
        {
            TokenType perv = tokens[0]->getType();
            if (perv == TokenType::POLINOM || perv == TokenType::PEREMENNAYA || \
                perv == TokenType::ONE_ARG_OPER || perv == TokenType::OPEN_SKOBKA || \
                perv == TokenType::TILDA)
            { }
            else
            {
                throw std::runtime_error("Sintaksis_First elem should be correct");
            }
            TokenType last = tokens.back()->getType();
            if (last == TokenType::OPERATION &&
                dynamic_cast<TOperation*>(tokens.back().get())->getArgument() == Arguments::TWO)
            {
                throw std::runtime_error("Sintaksis_Last elem should be correct");
            }
            for (size_t i = 1; i < tokens.size(); i++) // i от 1 -> prev always exists
            {
                TokenType prev = tokens[i - 1]->getType();
                TokenType cur = tokens[i]->getType();
                if (prev == TokenType::POLINOM) //POLI: OPER, )
                {
                    if (cur == TokenType::OPERATION || cur == TokenType::CLOSED_SKOBKA)
                    {

                    }
                    else
                        throw std::runtime_error("Sintaksis_POLINOM");
                }
                else if (prev == TokenType::OPERATION) //OPER: POLI, PEREM, (, ), ONE_ARG_OP
                {
                    if (cur == TokenType::POLINOM || cur == TokenType::PEREMENNAYA\
                        || cur == TokenType::OPEN_SKOBKA || cur == TokenType::ONE_ARG_OPER)
                    {

                    }
                    else
                        throw std::runtime_error("Sintaksis_OPERATION");
                }
                else if (prev == TokenType::ONE_ARG_OPER)//ONE_ARG_OP: POLI, PEREM, (, ONE_ARG_OP
                {
                    if (cur == TokenType::POLINOM || cur == TokenType::PEREMENNAYA\
                        || cur == TokenType::OPEN_SKOBKA || cur == TokenType::ONE_ARG_OPER)
                    {

                    }
                    else
                        throw std::runtime_error("Sintaksis_ONE_ARG_OPER");
                }
                else if (prev == TokenType::PEREMENNAYA) //PEREM: OPER, ), EQUATION
                {
                    const TPeremennaya* var = dynamic_cast<const TPeremennaya*>(tokens[i - 1].get()); // i-1 -> prev
                    if (i - 1 != 0)
                    {
                        if (!perem[var->getPeremennaya()])
                            throw std::runtime_error("Sintaksis_There is no such perem");
                    }
                    //
                    if (cur == TokenType::OPERATION || cur == TokenType::CLOSED_SKOBKA || \
                        cur == TokenType::EQUATION)
                    {
                        if (cur == TokenType::EQUATION)
                        {
                            if (tokens.size() <= 2)
                                throw std::runtime_error("Sintaksis_PEREMENNAYA_2");
                        }
                    }
                    else
                        throw std::runtime_error("Sintaksis_PEREMENNAYA_1");
                }
                else if (prev == TokenType::OPEN_SKOBKA) //(: POLI, PEREM, ONE_ARG_OP, (
                {
                    if (cur == TokenType::POLINOM || cur == TokenType::PEREMENNAYA\
                        || cur == TokenType::ONE_ARG_OPER || cur == TokenType::OPEN_SKOBKA)
                    {

                    }
                    else
                        throw std::runtime_error("Sintaksis_OPEN_SKOBKA");
                }
                else if (prev == TokenType::CLOSED_SKOBKA)//): OPER, )
                {
                    if (cur == TokenType::OPERATION || cur == TokenType::CLOSED_SKOBKA)
                    {

                    }
                    else
                        throw std::runtime_error("Sintaksis_CLOSED_SKOBKA");
                }
                else if (prev == TokenType::EQUATION)//= -> expected to be on [1], and on [0] expected oper
                {
                    if (i - 1 == 1) // i - 1 <- place for prev
                    {
                        if (cur == TokenType::POLINOM || cur == TokenType::PEREMENNAYA\
                            || cur == TokenType::OPEN_SKOBKA || cur == TokenType::ONE_ARG_OPER)
                        {
                            // =: POLI, PEREM, (, ONE_ARG_OP
                        }
                        else
                            throw std::runtime_error("Sintaksis_EQUATION_3");
                        if (tokens[0]->getType() == TokenType::PEREMENNAYA)
                        {
                            // Should be oper = ... If not -> error
                        }
                        else
                            throw std::runtime_error("Sintaksis_EQUATION_2");
                    }
                    else
                        throw std::runtime_error("Sintaksis_EQUATION_1");
                }
                else if (prev == TokenType::TILDA)//= -> expected to be on [0], and on [1] expected perem
                {
                    if (i - 1 == 0) // i - 1 <- place for prev
                    {
                        if (cur == TokenType::PEREMENNAYA)
                        {
                            // ~: PEREM
                        }
                        else
                            throw std::runtime_error("Sintaksis_TILDA_3");
                        if (tokens.size() == 2)
                        {
                            // ~PEREM => nothing else
                        }
                        else
                            throw std::runtime_error("Sintaksis_TILDA_2");
                    }
                    else
                        throw std::runtime_error("Sintaksis_TILDA_1");
                }
            }
        }
        //
        //SKOBKI at right positions
        std::stack<TokenType> stack;
        for (const auto& token : tokens)
        {
            if (token->getType() == TokenType::OPEN_SKOBKA)
            {
                stack.push(TokenType::OPEN_SKOBKA);
            }
            else if (token->getType() == TokenType::CLOSED_SKOBKA)
            {
                if (stack.empty() || stack.top() != TokenType::OPEN_SKOBKA)
                {
                    throw std::runtime_error("Sintaksis_SKOBKI_NESOGLASOVANNI");
                }
                stack.pop();
            }
        }
        return true;
    }
};

#endif

