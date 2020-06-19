

#include "token.h"



	//default constructor
	Token::Token()
	:sname(NONAME), sval(attVal(-1,string("NONAME")))
	{
	}


	//constructor initializer for token with symbol, value and lexeme
	Token::Token(Symbol s, int v, string l )
	:sname(s),sval(attVal(v,l))
	{
	}

	//getter function for token lexeme
	string Token::getLexeme()
	{
	    return sval.lexeme;
	}

	//getter function for numeral value of token
	int Token::getValue()
	{
		return sval.value;
	}


	//getter function for token symbol
	Symbol Token::getSymbol()
	{
	    return sname;
	}

	//identifier type setter(procedure/variable/constant)
	void Token::setIDtype(int type)
	{
		idType = type;	
	}

	int Token::getIDtype()
	{
		return idType;
	}


