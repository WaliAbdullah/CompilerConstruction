

#include "firstfollow.h"

	//Fidning the first of a procedure rule
	//First of definition
	vector<Symbol> Firstfollow::firstOfDefinition()
	{
		vector<Symbol> fOfConstDef = firstOfConstDef();
		vector<Symbol> fOfVariDef = firstOfVariDef();
		vector<Symbol> fOfProcDef = firstOfProcDef();

		vector<Symbol> fOfDefSet = fOfConstDef + fOfVariDef + fOfProcDef;
	
		return  fOfDefSet;		
	}

	//First of variableDefinition
	vector<Symbol> Firstfollow::firstOfVariDef()
	{
	

		vector<Symbol> fOfVariDef;
		fOfVariDef.push_back(INT);
		fOfVariDef.push_back(BOOL);
	
		return fOfVariDef;	
	} 

	//First of procedureDefinition	
	vector<Symbol> Firstfollow::firstOfProcDef()
	{
		vector<Symbol> fOfProcDef;
		fOfProcDef.push_back(PROC); 
		return fOfProcDef;
	}


	//First of constantDefinition
	vector<Symbol> Firstfollow::firstOfConstDef()
	{
	
		vector<Symbol> fOfContDef;
		fOfContDef.push_back(CONST); 
	
		return fOfContDef;	
	}

	//First of statementPart
	vector<Symbol> Firstfollow::firstOfStatePart()
	{
	
	}

	//First of statement
	vector<Symbol> Firstfollow::firstOfStatement()
	{
		vector<Symbol> fOfEmptySt = firstOfEmptySt();
		vector<Symbol> fOfReadSt = firstOfReadSt();
		vector<Symbol> fOfWriteSt = firstOfWriteSt();
		vector<Symbol> fOfAssignSt = firstOfAssignSt();
		vector<Symbol> fOfProcSt = firstOfProcSt();
		vector<Symbol> fOfIfSt = firstOfIfSt();
		vector<Symbol> fOfDoSt = firstOfDoSt();
	
		return  fOfEmptySt + fOfReadSt + fOfWriteSt + fOfAssignSt + fOfProcSt + fOfIfSt + fOfDoSt ;		
	
	}

	//First of readStatement
	vector<Symbol> Firstfollow::firstOfReadSt()
	{
		vector<Symbol> fOfReadSt;
		fOfReadSt.push_back(READ); 
	
		return fOfReadSt;
	}

	//First of writeStatement
	vector<Symbol> Firstfollow::firstOfWriteSt()
	{
		vector<Symbol> fOfWriteSt;
		fOfWriteSt.push_back(WRITE); 
	
		return fOfWriteSt;	
	}

	//First of assignmentStatement
	vector<Symbol> Firstfollow::firstOfAssignSt()
	{
		vector<Symbol> inVAList = firstOfVAList();
	
		return 	inVAList;
	}

	//First of procedureStatement
	vector<Symbol> Firstfollow::firstOfProcSt()
	{
		vector<Symbol> fOfProcSt;
		fOfProcSt.push_back(CALL); 
	
		return fOfProcSt;
	}

        //First of emptyStatement
	vector<Symbol> Firstfollow::firstOfEmptySt()
	{
		vector<Symbol> fOfEmptySt;
		fOfEmptySt.push_back(SKIP); 
	
		return fOfEmptySt;
	}

	//First of variableList	
	vector<Symbol> Firstfollow::firstOfVariList()
	{
		vector<Symbol> fOfVariList;
		fOfVariList.push_back(ID); 
	
		return fOfVariList;		
	}
        //First of variableAccessList
	vector<Symbol> Firstfollow::firstOfVAList()
	{
		vector<Symbol> fOfVaList;
		fOfVaList.push_back(ID); 
	
		return fOfVaList;
	}

	//First of ifStatement
	vector<Symbol> Firstfollow::firstOfIfSt()
	{
	
		vector<Symbol> fOfIfSt;
		fOfIfSt.push_back(IF); 
	
		return fOfIfSt;
	}

	//First of doStatement
	vector<Symbol> Firstfollow::firstOfDoSt()
	{
		vector<Symbol> fOfDoSt;
		fOfDoSt.push_back(DO); 
	
		return fOfDoSt;
	}

	//First of primaryOperator
	vector<Symbol> Firstfollow::firstOfPrimOp()
	{
		vector<Symbol> fOfPrimOp;
		fOfPrimOp.push_back(AND);
		fOfPrimOp.push_back(OR); 
	
		return fOfPrimOp;
	}

	//First of addingOperator
	vector<Symbol> Firstfollow::firstOfAddOp()
	{

		vector<Symbol> fOfAddOp;
		fOfAddOp.push_back(PLUS);
		fOfAddOp.push_back(MINUS);
	
		return fOfAddOp; 
	}

	//First of multiplyingOperator
	vector<Symbol> Firstfollow::firstOfMultOp()
	{
		vector<Symbol> fOfMultOp;
		fOfMultOp.push_back(TIMES);
		fOfMultOp.push_back(DIV);
		fOfMultOp.push_back(MOD);

		return fOfMultOp; 	
	}

	//First of relationalOperator
	vector<Symbol> Firstfollow::firstOfRelOp()
	{

		vector<Symbol> fOfRelOp;
		fOfRelOp.push_back(LESST);
		fOfRelOp.push_back(EQUAL);
		fOfRelOp.push_back(GREATERT);
	
		return fOfRelOp;
	}

	//First of constant
	vector<Symbol> Firstfollow::firstOfConstant()
	{

		vector<Symbol> fOfConst;
		fOfConst.push_back(ID);
		fOfConst.push_back(NUMERAL);
		fOfConst.push_back(TRUE);
		fOfConst.push_back(FALSE);
	
		return fOfConst;
	}

	//First of indexSelector
	vector<Symbol> Firstfollow::firstOfIndexSel()
	{
	
		vector<Symbol> fOfIndSel;
		fOfIndSel.push_back(LEFTBRACKET); 
	
		return fOfIndSel;
	}

	//First of term
	vector<Symbol> Firstfollow::firstOfTerm()
	{
		vector<Symbol> inFactor = firstOfFactor();
	
		return inFactor;
	}

	//First of factor
	vector<Symbol> Firstfollow::firstOfFactor()
	{
		vector<Symbol> fOfName;
		fOfName.push_back(ID);

		vector<Symbol> fOfLeftp;
		fOfLeftp.push_back(LEFTP);

		vector<Symbol> fOfConst = firstOfConstant();

		vector<Symbol> fOfTilt;
		fOfTilt.push_back(NOT);
		
		return fOfName + fOfConst + fOfLeftp + fOfTilt;
	}

	//First if guardedCommandList
	vector<Symbol> Firstfollow::firstOfGCList()
	{
		vector<Symbol> fOfExpList;
		fOfExpList.push_back(MINUS);
	
		vector<Symbol> fOfTerm = firstOfTerm();
	
		return fOfExpList + fOfTerm; 
	}

	//First if expressionList
	vector<Symbol> Firstfollow::firstOfExpList()
	{
		vector<Symbol> fOfExpList;
		fOfExpList.push_back(MINUS);
	
		vector<Symbol> fOfTerm = firstOfTerm();
	
		return fOfExpList + fOfTerm; 
	}



	//Finding the follow of a procedure rule 

	//follow of statementPart
	vector<Symbol> Firstfollow::followOfStatePart()
	{
		vector<Symbol> folOfGc = followOfGuardedCommand();
		vector<Symbol> fOfEnd;
		fOfEnd.push_back(END);
	
		return folOfGc + fOfEnd ;
	}


	//follow of definitionPart
	vector<Symbol> Firstfollow::followOfDefPart()
	{
		vector<Symbol> fOfStatement = firstOfStatement();
		vector<Symbol> fOfEnd;
		fOfEnd.push_back(END);
	
		return fOfEnd + fOfStatement ;
	}

	//follow of expressoion
	vector<Symbol> Firstfollow::followOfExpression()
	{
		vector<Symbol> folOfExpression;
		folOfExpression.push_back(GC2);
		folOfExpression.push_back(RIGHTP);
		folOfExpression.push_back(RIGHTBRACKET);
	
		return folOfExpression;
	}

	//follow of variableAccessList
	vector<Symbol> Firstfollow::followOfVaList()
	{
		vector<Symbol> folOfVaList;
		folOfVaList.push_back(ASSIGN);
	
		return folOfVaList;
	}

	//follow of guardedCommand
	vector<Symbol> Firstfollow::followOfGuardedCommand()
	{
		vector<Symbol> folOfGc;
		folOfGc.push_back(FI);
		folOfGc.push_back(OD);
		folOfGc.push_back(GC1);
	
		return folOfGc;
	}

	// minus operation for a vectors(which is set of symbols) and a symbol
	//- operator overloading for vector
	vector<Symbol> operator-(vector<Symbol> set, Symbol sym)
	{
		for(int i = 0; i < set.size(); i++)
		{
			if (sym == set.at(i))
				set.at(i) = NONAME;
		}
		return set;

	}	


	// union operation for 2 set of symbols
	//+ operator overloading for vector
	vector<Symbol> operator+(vector<Symbol> set1, vector<Symbol> set2)
	{
		vector<Symbol> mergedSet;

		mergedSet.reserve(set1.size() + set2.size());
		mergedSet.insert(mergedSet.end(), set1.begin(), set1.end());
		mergedSet.insert(mergedSet.end(), set2.begin(), set2.end());

		return mergedSet;

	}


