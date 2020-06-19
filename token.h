
//This file defines data members and members function for the token.cc class.

#ifndef TOKEN_H
#define TOKEN_H

#include <fstream>
#include <string>
#include "symbol.h"

using namespace std;

	class Token
	{
	    
	private:
	    // The following structure is used for storing attribute's value and lexeme
	    struct attVal
	    {
		attVal(int v, string l):value(v),lexeme(l){}
		int value;
		string lexeme;
	    };
	    
	    attVal sval;
	    Symbol sname;
	    int idType;
	    
	public:
	    // Default constructor
	    Token();
	    
	    // Constructor which takes the value and lexeme for a token
	    Token(Symbol sym, int val, string lex );
	    
	    //get functions
	    Symbol getSymbol();
	    int getValue();
	    string getLexeme();
	    void setIDtype(int type);
		int getIDtype();
	    
	};

#endif
