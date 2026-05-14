#ifndef __T_PEREMENNAYA__
#define __T_PEREMENNAYA__

#include <string>

#include "token.h"

class TPeremennaya : public Token
{
    std::string name;
public:
    TPeremennaya(const std::string& ident);
    TokenType getType() const override;
    std::string getPeremennaya() const;
};

#endif

