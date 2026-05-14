#include "t_polinom.h"

TPolinom::TPolinom(Polinom val) : value(std::move(val)) { }
TokenType TPolinom::getType() const
{
	return TokenType::POLINOM;
}
Polinom TPolinom::getPolinomValue() const
{
	return value;
}

