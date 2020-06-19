#ifndef SCANNER_H
#define SCANNER_H

#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <fstream>
#include "token.h"
#include "symboltable.h"

using namespace std;

class Scanner
{

	private:
	//input file stream object
	ifstream &srcFile;
	//symbol table object
	Symboltable &symTablePtr;
	//look ahead character
	char ch,laCh;
	
	int idType;
	//returns true if parameter is whitespace, false otherwise
	bool isWhitespace(char lach);
	//returns true if parameter is special, false otherwise
	bool isSpecial(char lach);

	//returns true if parameter is numeric, false otherwise
	bool isNumEnd(char lach);

	int isNumeric(string s);

	public:
	//constructor to initialize a scanner object with input stream and symbol table
	Scanner(ifstream &in,Symboltable &st);

	//most important function
        //scan the next sequence of character and tries to recognize if matches with some pattern or not
        //if a pattern is recognized, then token is construced 
	Token getToken();

	//This for debugging purpose
	void printSymTable();		

	//recognize an ID or reserve work and return the token
    	Token recognizeName();

        //recognize a special terminal symbol and return the token
    	Token recognizeSpecial();

        //recognize a decimal whole number and return the token
   	 Token recognizeNumeral();

        //recognize and ignore the comments
    	Token recognizeComment();



};

#endif
