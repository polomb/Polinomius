#ifndef __T_SKOBKI__
#define __T_SKOBKI__

#include <string>

#include "token.h"

class TSkobki : public Token
{
    char skobka;
    bool open;
public:
    TSkobki(char skoba);
    TokenType getType() const override;
    char getSkobka() const;
    bool isOpenSkobka() const;
};

#endif

