#include "t_skobki.h"

TSkobki::TSkobki(char skoba) : skobka(skoba)
{
	if (skoba == '(')
		open = true;
	else 
		open = false;
}
TokenType TSkobki::getType() const
{
	if(open)
		return TokenType::OPEN_SKOBKA;
	else
		return TokenType::CLOSED_SKOBKA;
}
char TSkobki::getSkobka() const
{
	return skobka;
}
bool TSkobki::isOpenSkobka() const
{
	return open;
}

