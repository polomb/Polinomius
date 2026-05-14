#ifndef __M_LEKSICH_ANALIZER__
#define __M_LEKSICH_ANALIZER__

#include <string>
#include <vector>
#include <memory>

#include "Polinom.h"
#include "token.h"

class LeksichAnalizer
{
    std::string input;
    size_t position;
    //
    void skipWhiteSpace();
    double parseNumber();
    std::unique_ptr<Token> parsePolinom();
    std::unique_ptr<Token> parsePeremennaya();
    char getCurrentChar() const;
    void goNextPos();
public:
    LeksichAnalizer(const std::string& inputStr);

    std::vector<std::unique_ptr<Token>> tokenize();
};

#endif

