#include "m_leksich_analizer.h"

#include <string>
#include <vector>
#include <stdexcept>
#include <cctype> //For isdigit and isalpha

#include "token.h"
#include "t_polinom.h"
#include "t_operation.h"
#include "t_peremennaya.h"
#include "t_skobki.h"

LeksichAnalizer::LeksichAnalizer(const std::string& inputStr)\
    : input(inputStr), position(0) {}
//
void LeksichAnalizer::goNextPos()
{
	if (position >= input.length())
		return;
	position++;
}
//
void LeksichAnalizer::skipWhiteSpace()
{
	while (position < input.length() && (input[position] == ' ' || input[position] == '\n'))
	{
		goNextPos();
	}
}
//
char LeksichAnalizer::getCurrentChar() const
{
	return input[position];
}
//
double LeksichAnalizer::parseNumber()
{
	size_t start = position;
	bool dot = false;

    while (position < input.length())
    {
        char c = input[position];
        if (std::isdigit(c))
        {
            goNextPos();
        }
        else if (c == '.' && !dot)
        {
            dot = true;
            goNextPos();
        }
        else
        {
            break;
        }
    }
    std::string numStr = input.substr(start, position - start);
    if(dot && numStr.size() == 1)
        throw std::runtime_error("LEKSICH_No_number_given");
    if (!dot)
    {
        //int
        int value = 0;
        for (char c : numStr)
        {
            value = value * 10 + (c - '0');
        }
        return value;
    }
    else
    {
        //double
        size_t dotPos = numStr.find('.');
        std::string intPart = numStr.substr(0, dotPos);
        std::string fracPart = numStr.substr(dotPos + 1);

        int integer = 0;
        for (size_t i = 0; i < intPart.length(); i++)
        {
            char c = intPart[i];
            integer = integer * 10 + (c - '0');
        }

        double fraction = 0.0;
        for (size_t i = 0; i < fracPart.length(); i++)
        {
            char c = fracPart[i];
            fraction = fraction * 10 + (c - '0');
        }
        for (size_t i = 0; i < fracPart.length(); i++)
        {
            fraction /= 10.0;
        }

        double value = integer + fraction;
        return value;
    }
}
std::unique_ptr<Token> LeksichAnalizer::parsePolinom()
{
    double coef; uint32_t degX = 0; uint32_t degY = 0; uint32_t degZ = 0;
    size_t start = position;
    if (std::isdigit(input[position]) || input[position] == '.')
    {
        coef = this->parseNumber();
    }
    else
    {
        coef = 1.0;
    }
    while (position < input.length())
    {
        if (input[position] == 'x' || input[position] == 'y' || input[position] == 'z')
        {
            char flag = input[position] - 'x';
            goNextPos();
            if (input[position] == '^')
            {
                goNextPos();
                if (std::isdigit(input[position]) || input[position] == '.')
                {
                    if (flag == 0 && degX == 0)
                    {
                        degX = this->parseNumber();
                    }
                    else if (flag == 0)
                    {
                        degX += this->parseNumber();
                    }
                    if (flag == 1 && degY == 0)
                    {
                        degY = this->parseNumber();
                    }
                    else if (flag == 1)
                    {
                        degY += this->parseNumber();
                    }
                    if (flag == 2 && degZ == 0)
                    {
                        degZ = this->parseNumber();
                    }
                    else if (flag == 2)
                    {
                        degZ += this->parseNumber();
                    }
                }
                else
                {
                    throw std::runtime_error("LEKSICH_polimon^NOT_NUM");
                }
            }
            else
            {
                if (flag == 0 && degX == 0)
                {
                    degX = 1;
                }
                else if (flag == 0)
                {
                    degX += 1;
                }
                if (flag == 1 && degY == 0)
                {
                    degY = 1;
                }
                else if (flag == 1)
                {
                    degY += 1;
                }
                if (flag == 2 && degZ == 0)
                {
                    degZ = 1;
                }
                else if (flag == 2)
                {
                    degZ += 1;
                }
            }
        }
        else
        {
            break;
        }
    }
    Polinom poly(coef, degX, degY, degZ);
    return std::make_unique<TPolinom>(poly);
}
//
std::unique_ptr<Token> LeksichAnalizer::parsePeremennaya()
{
    size_t start = position;
    if (position < input.length() && std::isalpha(input[position]))
    {
        goNextPos();
        while (position < input.length() && (std::isalpha(input[position]) ||\
                std::isdigit(input[position]) || input[position] == '_'))
        {
            goNextPos();
        }
    }
    std::string identifier = input.substr(start, position - start);
    return std::make_unique<TPeremennaya>(identifier);
}
//
std::vector<std::unique_ptr<Token>> LeksichAnalizer::tokenize()
{
    std::vector<std::unique_ptr<Token>> tokens;
    position = 0;
    while (position < input.length())
    {
        skipWhiteSpace();
        //
        if (position >= input.length()) 
            break;
        //
        char current = getCurrentChar();
        std::unique_ptr<Token> token = nullptr;
        //
        if (std::isdigit(current) || current == '.' || current == 'x' || current == 'y' || current == 'z')
        {
            token = parsePolinom();
        }
        else if (std::isalpha(current))
        {
            token = parsePeremennaya();
        }
        else if (current == '+' || current == '-' || current == '*' || current == '/')
        {
            token = std::make_unique<TOperation>(std::string(1, current), Arguments::TWO);
            goNextPos();
        }
        else if (current == '(' || current == ')')
        {
            token = std::make_unique<TSkobki>(current);
            goNextPos();
        }
        else if(current == '=')
        {
            token = std::make_unique<TEquation>(current);
            goNextPos();
        }
        else if (current == '~')
        {
            token = std::make_unique<TTilda>(current);
            goNextPos();
        }
        else
        {
            throw std::runtime_error("LEKSICH_Unknown character '" + std::string(1, current)\
                + "' at position " + std::to_string(position+1));
        }
        //
        if (token)
        {
            tokens.push_back(std::move(token));
        }
    }
    if (tokens.size() == 1) { }
    else if (tokens.size() == 2)
    {
        TokenType fir = tokens[0]->getType();
        TokenType sec = tokens[1]->getType();
        if (fir == TokenType::OPERATION && (sec == TokenType::POLINOM || sec == TokenType::PEREMENNAYA))
        {
            TOperation* operation = dynamic_cast<TOperation*>(tokens[0].get());
            if (operation->getOperation() == "-")
            {
                operation->setArgument(Arguments::ONE);
            }
        }
    }
    else
    {
        TokenType fir = tokens[0]->getType();
        TokenType sec = tokens[1]->getType();
        if (fir == TokenType::OPERATION && (sec == TokenType::POLINOM || sec == TokenType::PEREMENNAYA))
        {
            TOperation* operation = dynamic_cast<TOperation*>(tokens[0].get());
            if (operation->getOperation() == "-")
            {
                operation->setArgument(Arguments::ONE);
            }
        }
        for (size_t i = 2; i < tokens.size(); i++)
        {
            TokenType fir = tokens[i-2]->getType();
            TokenType sec = tokens[i-1]->getType();
            TokenType thi = tokens[i]->getType();
            if ((thi == TokenType::POLINOM || thi == TokenType::PEREMENNAYA ||thi == TokenType::OPEN_SKOBKA || thi == TokenType::ONE_ARG_OPER)\
                && (sec == TokenType::OPERATION) && \
                (fir != TokenType::POLINOM && fir != TokenType::PEREMENNAYA && fir != TokenType::CLOSED_SKOBKA))
            {
                if (dynamic_cast<TOperation*>(tokens[i - 1].get())->getOperation() == "-")
                {
                    dynamic_cast<TOperation*>(tokens[i - 1].get())->setArgument(Arguments::ONE);
                }
            }
        }
    }
    return tokens;
}

