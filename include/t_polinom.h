#ifndef __T_POLINOM__
#define __T_POLINOM__

#include <string>

#include "token.h"
#include "Polinom.h"

class TPolinom : public Token
{
    Polinom value;
public:
    TPolinom(Polinom val);
    TokenType getType() const override;
    Polinom getPolinomValue() const;
};

#endif

