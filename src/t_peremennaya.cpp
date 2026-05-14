#include "t_peremennaya.h"

TPeremennaya::TPeremennaya(const std::string& ident) : name(ident) { }
TokenType TPeremennaya::getType() const
{
	return TokenType::PEREMENNAYA;
}
std::string TPeremennaya::getPeremennaya() const
{
	return name;
}

