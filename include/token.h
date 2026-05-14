#ifndef __TOKEN__
#define __TOKEN__

#include <string>

enum class TokenType
{
    POLINOM,
    OPERATION,
    ONE_ARG_OPER,
    PEREMENNAYA,
    OPEN_SKOBKA,
    CLOSED_SKOBKA,
    EQUATION,
    TILDA
};

class Token
{
public:
    virtual ~Token() = default;
    virtual TokenType getType() const = 0;
};

#endif

