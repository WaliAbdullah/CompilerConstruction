#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <vector>
#include <iostream>
#include <string>
#include <cstdlib>
#include "token.h"
#include "symbol.h"
#define KEYWORDS 17
#define SYMTABLESIZE 307

using namespace std;

	class Symboltable
	{

		private:
		vector<Token> htable;
		int occupied;// number of cells occupied
		int hashfn(string lex); // hash function
		int position;//position got from hashfn
		
	
		public:
		// create a table with SYMTABLESIZE 307 entries.
		Symboltable();
		// destructor
		~Symboltable(){}
		// return the position of the lexeme in the table if found, -1 if not.
		int find(string lex);
		// Insert a token with lexeme s in the symbol table; if already in the table return its
		// location (index); otherwise create a token of appropriate type; insert the token and
		// return its location; modified in the later phases
		Token insert(string s,int type);
		void insert(Token tok);//this insert is used for pre loading symbol table with keywords
		// Return true if the table is full; false otherwise
		bool full();
		// Return the number of occupied cells (used in load factor calculartion.
		int getOC();
		//print symbol table
		void print();
		static const string keywords[KEYWORDS]; 	
	
	};

#endif


