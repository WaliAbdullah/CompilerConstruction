
// This files contains the symbol-name for tokens
//Some of symbols defined might be needed in the next phase

#ifndef SYMBOL_H
#define SYMBOL_H

//Symbols for tokens
enum Symbol
{
    //Starting from 256 to avoid conflict with ASCII
    //Operators and Special characters are defined here

	ID=256, NUMERAL, BADNUMERAL, BADNAME, //256-259 
	BADSYMBOL, BADCHAR, NEWLINE, NONAME, //260-263
	ENDOFFILE, DOT, COMMA, SEMICOLON, //264-267
	LEFTBRACKET,RIGHTBRACKET, AND, OR, //268-271
	NOT,LESST, EQUAL, GREATERT, //272-275
	PLUS, MINUS, //276-277
	TIMES, DIV, MOD,LEFTP, //278-281
	RIGHTP, ASSIGN,	GC1,GC2, //282-285 

    //Reserve words for PL is defined here
	BEGIN, END, CONST, ARRAY, //286-289 
	INT, BOOL, PROC, SKIP, //290-293
	READ, WRITE, CALL, IF, //294-297
	DO, FI, OD, FALSE, TRUE //298-302
	   
};


#endif
