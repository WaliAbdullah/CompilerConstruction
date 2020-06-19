#ifndef FIRSTFOLLOW_H
#define FIRSTFOLLOW_H

#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <vector>
#include "token.h"

using namespace std;

class Firstfollow
{

	private:
	
	
	public:
        //member functions for firstfollow.cc
        //First of statementPart	
	vector<Symbol> firstOfStatePart(); 
	//First of statement	
	vector<Symbol> firstOfStatement();
        //First of readStatement	
	vector<Symbol> firstOfReadSt();
	//First of writeStatement
	vector<Symbol> firstOfWriteSt();
        //First of assignmentStatement
	vector<Symbol> firstOfAssignSt();
	//First of procedureStatement
        vector<Symbol> firstOfProcSt();
        //First of emptyStatement	
	vector<Symbol> firstOfEmptySt();
        //First of ifStatement
	vector<Symbol> firstOfIfSt();
	//First of doStatement
	vector<Symbol> firstOfDoSt();
	//First of definition	
	vector<Symbol> firstOfDefinition();
	//First of constantDefinition
	vector<Symbol> firstOfConstDef();
	//First of variableDefinition
	vector<Symbol> firstOfVariDef(); 
	//First of procedureDefinition	
	vector<Symbol> firstOfProcDef(); 
	//First of variableList	
	vector<Symbol> firstOfVariList();
	//First of variableAccessList
	vector<Symbol> firstOfVAList();
        //First if expressionList
	vector<Symbol> firstOfExpList();
	//First if guarderCommandList
	vector<Symbol> firstOfGCList();	
	//First of indexSelector
	vector<Symbol> firstOfIndexSel();
	//First of primaryOperator
	vector<Symbol> firstOfPrimOp();
        //First of addingOperator
	vector<Symbol> firstOfAddOp();
        //First of multiplyingOperator
	vector<Symbol> firstOfMultOp();
        //First of relationalOperator
	vector<Symbol> firstOfRelOp();
	//First of term
	vector<Symbol> firstOfTerm();
	//First of factor
	vector<Symbol> firstOfFactor();
	//First of constant
	vector<Symbol> firstOfConstant();
	
	//functions for follow of a termina/non-terminal

	//follow of definitionPart
	vector<Symbol> followOfDefPart();
        //follow of variableAccessList
	vector<Symbol> followOfVaList();	
	//follow of Expression
	vector<Symbol> followOfExpression();
	//follow of statementPart	
	vector<Symbol> followOfStatePart();
	//follow of guardedCommand
	vector<Symbol> followOfGuardedCommand();
	
		
		
		
};

vector<Symbol> operator+(vector<Symbol> , vector<Symbol> );
vector<Symbol> operator-(vector<Symbol> , Symbol );	
#endif
