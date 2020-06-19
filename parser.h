#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <vector>
#include "token.h"
#include "scanner.h"
#include "blocktable.h"
#include "type.h"
#include "firstfollow.h"
#include "administration.h"	


// This is the maximum number of errors before the compiler bails out.
#define MAXERRORS 50
//enum errorkind {ScanE, ParseE, ScopeE, TypeE};
using namespace std;

class Parser
{

	private:
	//input stream of source file
	ifstream &srcFile;
	//out stream of output file	
	ofstream &outFile;
	ofstream &outFile1;
	//Scanner object reference
	Scanner &scanner;
	//BlockTable object reference
	BlockTable &bTable;
	// object to call method of first and follow of the non terminals
	Firstfollow ff;
	int lineNo;
	// report error only if correct line is true
	bool correctline;
	//count the number of errors.
	int errorCount;
	//get token from scan() function	
	Token lookAheadTok;
	//keep track wheather nextTok is currect or look ahead token
	bool islookAheadTok;
	//Stop symbols holder
	vector<Symbol> stopSet;
	//Assign and gc2 flag
	bool agc;
        bool check;
        int count=0;
        int count1=0;
        bool level;
	//Administration class object for line count and error display	
	Administration admin;
	
	public:
	// Set up input and output files for scanning
	Parser(ifstream &in, ofstream &out1, ofstream &out, Scanner &sc, BlockTable &bt);
	~Parser() {}
	//functions for implementing CFG
	// program = block '.'
	void program(Symbol);
	// block = 'begin' definitionPart statementPart 'end'
	void block(int,int,int,vector<Symbol>);
	// definitionPart = {definition';'}
	void definitionPart(int&,vector<Symbol>);
	// definition = constantDefinition | variableDefinition | procedureDefinition
	void definition(int&,vector<Symbol>);
	// constantDefinition = 'const' constName '='  constant;
	void constantDefinition(vector<Symbol>);
	// variableDefinition = typeSymbol variableList | typeSymbol 'array' variableList'['constant']'
	void variableDefinition(int&,vector<Symbol>);
	// procedureDefinition = 'proc' procedureName block
	void procedureDefinition(int&,vector<Symbol>);
	// typeSymbol = 'integer' | 'Boolean'
	PL_Type typeSymbol(vector<Symbol>);
	// variableList = variableName {',' variableName}
	void variableList(PL_Type,int&,vector<Symbol>);
	// statementPart = {statement';'}
	void statementPart(vector<Symbol>);
	// statement = emptyStatement | readStatement | writeStatement |  assignmentStatement | ifStatement | doStatement 
	void statement(vector<Symbol>);
	// readStatement = 'read' variableAccessList
	void readStatement(vector<Symbol>);
	// variableAccessList = variableAccess{','variableAccess}
	//vector<PL_Type> variableAccessList(vector<Symbol>,vector<PL_Type> );
	void variableAccessList(vector<Symbol>, vector<PL_Type>& );
	// writeStatement = 'write' expressionList  
	void writeStatement(vector<Symbol>);
	// expressionList = expression {','expression}
	//vector<PL_Type> expressionList(vector<Symbol>, vector<PL_Type> );
	void expressionList(vector<Symbol>,vector<PL_Type>&);
	// assignmentStatement = variableAccessList ':=' expressionList
	void assignmentStatement(vector<Symbol>);
	// ifStatement = 'if' guardedCommandList 'fi' 
	void ifStatement(vector<Symbol>);
	// doStatement = 'do' guardedCommandList 'od'
	void doStatement(vector<Symbol>);
	// guardedCommandList = guardedCommand{'[]'guardedCommand}
	void guardedCommandList(int&,int,vector<Symbol>);
	// guardedCommand = expression '->' statementPart
	void guardedCommand(int&,int,vector<Symbol>);
	// expression = primaryExpression{primaryOperator primaryExpression}	
	PL_Type expression(vector<Symbol>,int,Symbol);
	// primaryOperator = '&' | '|'
	void primaryOperator(vector<Symbol>);	
	// primaryExpression = simpleExpression [relationalOperator simpleExpression]
	PL_Type primaryExpression(vector<Symbol>);
	// relationalOperator = '<' | '=' | '>'	
	void relationalOperator(vector<Symbol>);
	// simpleExpression = ['-'] term {addingOperator term}
	PL_Type simpleExpression(vector<Symbol>);
	// addingOperator = '+' | '-'
	void addingOperator(vector<Symbol>);
	//helps to execute {addingOperator term}
	void addopTerm(vector<Symbol>);
	// term = factor {multiplyingOperator factor}
	void term(PL_Type&,vector<Symbol>,int,Symbol);
	// multiplyingOperator = '*' | '/' | '\'
	void multiplyingOperator(vector<Symbol>);
	// factor = constant | variableAccess | '('expression')' | '~' factor	
	PL_Type factor(vector<Symbol>);
	// variableAccess = variableName [indexSelector]
	PL_Type variableAccess(vector<Symbol>);
	// constanr = numeral | booleanSymbol | constantName
	void constant(int&, PL_Type&, vector<Symbol>);
	
	//this function is used to match terminal symbols of CFG
	bool match(Symbol,vector<Symbol>);
	//this function is used to match name symbols of CFG
	int matchName(Symbol,vector<Symbol>);

	//any symbol is in a set of symbol
	bool in(vector<Symbol> );
	// Begin a new line of input
	void NewLine();
	
	//syntax error recovery function
	void syntaxError(vector<Symbol>);
	void syntaxCheck(vector<Symbol>);
	//keep track whether token is current token or look ahead token
	void lookAheadToken();
	//Parsing starts here
	int parse();

	//new label and new label genereator
	int label;
	int NewLabel();
	
	
		
};
vector<PL_Type> operator+(vector<PL_Type> , PL_Type);

#endif

