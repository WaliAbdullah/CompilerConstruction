#include "parser.h"

//constructor
Parser::Parser(ifstream &in, ofstream &out1, ofstream &out, Scanner &sc, BlockTable &bt)
:srcFile(in),outFile1(out1), outFile(out),scanner(sc),bTable(bt),islookAheadTok(false),label(0),admin(out)
{
	agc = false;
        level=false;
        check=true;
	//First look ahead token	
	lookAheadToken();
}



/* For non-terminals procedures are defined and terminals are matched using match() function*/		
int Parser::parse()
{
	program(ENDOFFILE);
	//after parsing return no of errors occured
	return admin.getErrCount();
}
// program = block '.'
void Parser::program(Symbol sym)
{
	
	//Building stop set using vector		
	stopSet.push_back(sym);
	stopSet.push_back(DOT);
	outFile1<<"program()"<<endl;
        for(int i=0; i<3; i++)
        {
            outFile1 << "  |\n";
        }
        //variables to store reference labels
	int startLabel, varLabel; 
	startLabel = NewLabel(); 
        // To record the the length of the defined variables
	varLabel = NewLabel(); 

	admin.emit3("PROG", varLabel, startLabel);

	//non terminal bloc	
	block(startLabel,varLabel,0,stopSet);
	
	//Building stop set using vector that may appear after dot symbol
	vector<Symbol>().swap(stopSet);
	stopSet.push_back(sym); //adding symbols in stop set
	
	//this portion matches dot and if not matched the shows corresponding error message and finds the next stop symbol
	match(DOT,stopSet);
	
	admin.emit1("ENDPROG");
	//parsing complete
	admin.complete();

}

// block = 'begin' definitionPart statementPart 'end'
void Parser::block(int sLabel, int vLabel,int varLength, vector<Symbol> stops)
{       
          if(level==true)
                {
                     count=count+1;
                    count1++;
                 }
	outFile1<<"   Block " << count << " Begins\n";
        for(int i=0; i<3; i++)
        {
            outFile1 << "       |\n";
        }
                level=true; 
//tree(8,0);
                //p=&count;
	        check=false;
	//new block in block table
	if(!bTable.newBlock())
	{
		admin.fatal("Exceeded block limit");
	}
	//match 'begin' and if not a match then show error message and move on to find the next stop symbol
	match(BEGIN,ff.firstOfDefinition() + ff.firstOfStatement() + stops);
	
	//look ahead token is first of definition
	if(in(ff.firstOfDefinition()))
	{	
		vector<Symbol>().swap(stopSet);
		stopSet.push_back(END);
		
		// varLength is the value of “vLabel” and is determined in DefinitionPart
		definitionPart(varLength,ff.firstOfStatement() + stops + stopSet);


		// labels used in PROC and PROG.
		// Output assembler instruction DEFARG
                //varlength as the 3rd argument
		admin.emit3("DEFARG", vLabel, varLength);

		// first executable instruction
		// DEFADDR to enter
		// replacing labelTable[startLabel] = address for next instruction
		// The assembler keeps track of the address of the instructions.
		admin.emit2("DEFADDR", sLabel);
		statementPart(stops + stopSet);

	}

	//look ahead token is follow of definition
	else if(in(ff.followOfDefPart()))
	{
		vector<Symbol>().swap(stopSet);
		stopSet.push_back(END);

		
                // labels used in PROC and PROG.
	        // Output assembler instruction DEFARG
                //varlength as the 3rd argument
		admin.emit3("DEFARG", vLabel, varLength);

		// first executable instruction
		// DEFADDR to enter
		// replacing labelTable[startLabel] = address for next instruction
		// The assembler keeps track of the address of the instructions.
		admin.emit2("DEFADDR", sLabel);

		statementPart(stops + stopSet);
	}
	
	//match terminal symbol, if false show error message and move on to find next stop symbol
	match(END,stops);
        outFile1 << "\n";
        for(int i=0; i<6; i++)
        {
            outFile1 << "   --";
        }
        outFile1<< "\n    Block  " << count+1 << "  ends";
        
        check=true;
}

// definitionPart = {definition';'}
void Parser::definitionPart(int& varLength,vector<Symbol> stops)
{
       if(check==false)
             {
                /* for(int i=0; i<6; i++)
                 {
                     outFile << "   --";
                 }*/
             
	      outFile1<<"\n       definitionPart() " << count << "\n";
                 for(int i=0; i<3; i++)
                 {
                     outFile1 << "          |\n";
                 }
          // count1++;
	      check=true;
             }
	if(in(ff.firstOfDefinition()))
	{
		vector<Symbol>().swap(stopSet);
		stopSet.push_back(SEMICOLON);	
		definition(varLength,ff.firstOfDefinition() + stopSet + stops);
		match(SEMICOLON , stops + ff.firstOfDefinition());
		definitionPart(varLength,stops);
                 check=false;
	}
	else
		syntaxCheck(stops);
}

// definition = constantDefinition | variableDefinition | procedureDefinition
void Parser::definition(int& varLength,vector<Symbol> stops)
{
         outFile1<< "\n";
         outFile1<<"          Definition()\n";
	if(in(ff.firstOfConstDef()))
	{
		constantDefinition(stops);
	}
	else if(in(ff.firstOfVariDef()))
	{
		variableDefinition(varLength,stops);
	}
	else if(in(ff.firstOfProcDef()))
	{
		procedureDefinition(varLength,stops);
	}
}

// constantDefinition = 'const' constName '='  constant;
void Parser::constantDefinition(vector<Symbol> stops)
{
        for(int i=0; i<2; i++)
        {
            outFile1 << "          |\n";
        }
		outFile1<<"           constantDefinition()\n";
	
	vector<Symbol>().swap(stopSet);
	stopSet.push_back(EQUAL);
	
	match(CONST , ff.firstOfConstant() + stopSet + stops);
	
	//matchName is match function for matching name symbols
	int id = matchName(ID, stopSet + stops);
	
	match(EQUAL , ff.firstOfConstant() + stops);
	
	//scope and type check
	int tempValue;//for constant value
	PL_Type tempType;//for type of constant
	constant(tempValue, tempType, stops);
	

	if(id != -1)
	{
		//define the name in block table
		bool isDefined = bTable.define(id,CONSTANT,tempType,0,tempValue,0);
		if(!isDefined)
		{
			//ambiguous name error
			admin.error(ScopeE,lookAheadTok.getSymbol(),5);

		}
	}
}

// constanr = numeral | booleanSymbol | constantName
void Parser::constant(int& tempValue, PL_Type& tempType, vector<Symbol> stops)
{
	//outFile1<<"constant()"<<endl;

	if(lookAheadTok.getSymbol() == NUMERAL)
	{
		tempValue = lookAheadTok.getValue();
                outFile1<<"              constant()->" << tempValue;
		tempType = INTEGRAL;
		match(NUMERAL,stops);

	}	
	else if(lookAheadTok.getSymbol() == TRUE)
	{
		tempValue = 1;
		tempType = BOOLEAN;
		match(TRUE,stops);
	}
	else if(lookAheadTok.getSymbol() == FALSE)
	{
		tempValue = 0;
		tempType = BOOLEAN;
		match(FALSE,stops);
	}
	else if(lookAheadTok.getSymbol() == ID)
	{
		//look for the constantName in block table
		int id = matchName(ID, stops);
		if(id != -1)
		{
			TableEntry entry =  bTable.find(id,bTable.error);
			//not found
			if(bTable.error)
			{
				admin.error(ScopeE,lookAheadTok.getSymbol(),4);
			}
			//matched but typekind is not constant 
			else if(entry.kind != CONSTANT)
			{
				admin.error(ScopeE,lookAheadTok.getSymbol(),2);
			}
			//matched
			else
			{
				tempValue = entry.value;
				tempType = INTEGRAL;
			}
		}

	}
	
	else
	{
		tempValue = -1;
		tempType = UNIVERSAL;
		admin.error(ParseE,lookAheadTok.getSymbol(),6);
	}	
}


// variableDefinition = typeSymbol variableList | typeSymbol 'array' variableList'['constant']'
void Parser::variableDefinition(int& varLength,vector<Symbol> stops)
{
         for(int i=0; i<2; i++)
        {
            outFile1 << "              |\n";
        }
        
		  outFile1<<"               variableDefinition()"; //11 spaces
	vector<Symbol>().swap(stopSet);
	stopSet.push_back(ARRAY);
	
	PL_Type tempType = typeSymbol(stopSet + ff.firstOfVariList() + stops);
	
	if(in(ff.firstOfVariList()))
	{
		variableList(tempType,varLength,stops);
	}
	else if(lookAheadTok.getSymbol() == ARRAY)
	{
		int defPosition = bTable.def;
		vector<Symbol>().swap(stopSet);
		stopSet.push_back(LEFTBRACKET);
		stopSet.push_back(RIGHTBRACKET);

		match(ARRAY, stopSet + ff.firstOfVariList() + stops);
		
		int prevVarlength = varLength;
		variableList(tempType,varLength,stopSet + stops);
		
		vector<Symbol>().swap(stopSet);
		stopSet.push_back(RIGHTBRACKET);
		match(LEFTBRACKET , stopSet + ff.firstOfConstant() + stops);
		
		int tempValue;
		PL_Type tempType;
		constant(tempValue, tempType, stopSet + stops);
		match(RIGHTBRACKET , stops);

		//set array size
		if(tempType == INTEGRAL)
		{
			//set array size of the declared variable in block table
			bTable.setArraySize(defPosition,tempValue);
			varLength += (varLength - prevVarlength)*tempValue - (varLength - prevVarlength);
			//return (bTable.def - defPosition -1)*arrayVars + varLength;
		}
		else
		{
			//array size type must be integer
			admin.error(ScopeE,lookAheadTok.getSymbol(),2);
		}
	}
}

// typeSymbol = 'integer' | 'Boolean'
PL_Type Parser::typeSymbol(vector<Symbol> stops)
{
	if(lookAheadTok.getSymbol() == INT)
	{
		
		match(INT,stops);
		return INTEGRAL;
	}	
	else if(lookAheadTok.getSymbol() == BOOL)
	{
		
		match(BOOL,stops);
		return BOOLEAN;
	}
}


// variableList = variableName {','variableName}
void Parser::variableList(PL_Type tempType,int& varListSize, vector<Symbol> stops)
{
	if(lookAheadTok.getSymbol() == ID )	
	{
                outFile1 <<"\n                   varList()->" << tempType << " " << lookAheadTok.getSymbol() << " ";
		//if(lookAheadTok.getIDtype() != 1)
		//	admin.error(ParseE,lookAheadTok.getSymbol(),1);
		vector<Symbol>().swap(stopSet);
		stopSet.push_back(COMMA);

		int id = matchName(ID, stopSet + stops);

		if(id != -1)
		{
				bool isDefined = bTable.define(id, VAR, tempType, 1, -1,varListSize+3);
				varListSize++;
				if(!isDefined)
				{
					//ambiguous name error
					admin.error(ScopeE,lookAheadTok.getSymbol(),5);
				}
		}
	}	
	
	if(lookAheadTok.getSymbol() == COMMA)
	{
		match(COMMA, ff.firstOfVAList() + stops);
		variableList(tempType,varListSize,stops);
	}
	else
	{
		syntaxCheck(stops - ID);
	}
}

// procedureDefinition = 'proc' procedureName block
void Parser::procedureDefinition(int& varLength,vector<Symbol> stops)
{
         for(int i=0; i<2; i++)
        {
            outFile1 << "          |\n";
        }
		outFile1<<"           procedureDefinition()"<<endl;
	vector<Symbol>().swap(stopSet);
	stopSet.push_back(BEGIN);
	stopSet.push_back(ID);
	
	match(PROC, stopSet + stops);

	stopSet.pop_back();//pop back ID from stop set that may appear ID
	
	int id = matchName(ID, stopSet + stops);
	 
	if(id != -1)
	{
		bool isDefined = bTable.define(id,PROCEDURE,INTEGRAL,0,1,0);
		if(!isDefined)
		{
			//ambiguous name error
			admin.error(ScopeE,lookAheadTok.getSymbol(),5);
		}
	}
         //variables to store reference labels
	int procLabel,startLabel, varLabel;
	procLabel = NewLabel();
        // to record the the length of the variables defined
	varLabel = NewLabel(); 
        //record the address first instruction of program
	startLabel = NewLabel(); 
	bTable.setStartLabel(procLabel);
	admin.emit2("DEFADDR",procLabel);
	admin.emit3("PROC",varLabel,startLabel);
	block(startLabel,varLabel,0,stops);
	admin.emit1("ENDPROC");
	bTable.endBlock();
}


// statementPart = {statement';'}
void Parser::statementPart(vector<Symbol> stops)
{

        if(check==false)
                   {
                       outFile1 << "\n";
                       for(int i=0; i<2; i++)
                       {
                           outFile1 << "         |\n";
                       }
                      count1=count1++;
		   outFile1<<"\n       stmtPart()  " <<count << "\n";
                    count=count-1;
	           check=true;
                   }
	if(in(ff.firstOfStatement()))
	{	
		vector<Symbol>().swap(stopSet);	
		stopSet.push_back(SEMICOLON);
		
		statement(stopSet + stops);
		match(SEMICOLON , ff.firstOfStatement() + stops);
		statementPart(stops);
                check=false;
	}
	else if(in(ff.followOfStatePart()))
	{
		//do nothing
	}
	else
		syntaxCheck(stops);
}

// statement = emptyStatement | readStatement | writeStatement |  assignmentStatement | ifStatement | doStatement 
void Parser::statement(vector<Symbol> stops)
{
        outFile1 << "\n";
        for(int i=0; i<2; i++)
        {
            outFile1 << "             |\n";
        }
		outFile1<<"             stmt()\n";
	if(in(ff.firstOfEmptySt()))
	{
		match(SKIP,stops);
	}
	else if(in(ff.firstOfReadSt()))
	{
		readStatement(stops);if(lookAheadTok.getSymbol() == INT)
	{
		match(INT,stops);		
	}	
	else if(lookAheadTok.getSymbol() == BOOL)
	{
		match(BOOL,stops);
	}
	}
	else if(in(ff.firstOfWriteSt()))
	{
		writeStatement(stops);
	}
	else if(in(ff.firstOfProcSt()))
	{
		vector<Symbol>().swap(stopSet);
		stopSet.push_back(ID);
		
		match(CALL, stopSet + stops);

		if(lookAheadTok.getSymbol() == ID )	
		{
			//look for the procedure name in block table
			int id = matchName(ID, stops);
			TableEntry entry =  bTable.find(id,bTable.error);
			//not found
			if(bTable.error)
			{
				admin.error(ScopeE,lookAheadTok.getSymbol(),4);
			}
			//found but kind is not procedure
			else if(entry.kind != PROCEDURE)
			{
				admin.error(ScopeE,lookAheadTok.getSymbol(),3);
			}
			//entry.disp is proc label for procedure name
			admin.emit3("CALL", bTable.currentBlockLabel() -entry.bl, entry.disp);
		}
	}
	else if(in(ff.firstOfAssignSt()))
	{
		assignmentStatement(stops);
	}
	else if(in(ff.firstOfIfSt()))
	{
		ifStatement(stops);		
	}
	else if(in(ff.firstOfDoSt()))
	{
		doStatement(stops);		
	}
}

// readStatement = 'read' variableAccessList
void Parser::readStatement(vector<Symbol> stops)
{
        for(int i=0; i<2; i++)
        {
            outFile1 << "               |\n";
        }
		outFile1<<"               readStatement()";
	match(READ, ff.firstOfVAList() + stops);
	vector<PL_Type> typeList;
	vector<PL_Type>().swap(typeList);
	variableAccessList(stops,typeList);
	admin.emit2("READ",typeList.size());
}

// variableAccessList = variableAccess{','variableAccess}
void Parser::variableAccessList(vector<Symbol> stops, vector<PL_Type>& varTypeList)
{
        outFile1<<"->varAccessList()";
	vector<Symbol>().swap(stopSet);
	stopSet.push_back(COMMA);
	stopSet.push_back(ID);
	
	PL_Type tempType = variableAccess(stopSet + stops);
	varTypeList.push_back(tempType);

	if(lookAheadTok.getSymbol() == COMMA)
	{
		match(COMMA, ff.firstOfVAList() + stops);
		variableAccessList(stops, varTypeList);
	}
	else if(in(ff.followOfVaList()))
	{
		syntaxCheck(stops);		
	}
	else
	{
		syntaxCheck(stops - ID);
	}			
}



// writeStatement = 'write' expressionList  
void Parser::writeStatement(vector<Symbol> stops)
{
         for(int i=0; i<2; i++)
        {
            outFile1 << "               |\n";
        }
		outFile1<<"               writeStatement()";
	match(WRITE, ff.firstOfExpList() + stops);
	vector<PL_Type> typeList;
	vector<PL_Type>().swap(typeList);
	expressionList(stops,typeList);

	admin.emit2("WRITE",typeList.size());
}

// expressionList = expression {','expression}
void  Parser::expressionList(vector<Symbol> stops, vector<PL_Type>& typeList)
{
        outFile1<<" ->expList()";
	vector<Symbol>().swap(stopSet);
	stopSet.push_back(COMMA);

	PL_Type tempType = expression(stopSet + stops,0,NONAME);
	typeList.push_back(tempType);
	
	if(lookAheadTok.getSymbol() == COMMA)
	{
		match(COMMA, ff.firstOfExpList() + stops);
		expressionList(stops,typeList);
	}
	else
	{
		syntaxCheck(stops - ID);
	}
}

// assignmentStatement = variableAccessList ':=' expressionList
void Parser::assignmentStatement(vector<Symbol> stops)
{
         for(int i=0; i<2; i++)
        {
            outFile1 << "               |\n";
        }
		outFile1<<"                assignStmt() := ";
	vector<Symbol>().swap(stopSet);
	stopSet.push_back(ASSIGN);
	
	vector<PL_Type> varTypeList;
	vector<PL_Type>().swap(varTypeList);
	variableAccessList(stopSet + stops,varTypeList);
	match(ASSIGN, ff.firstOfExpList() + stops);

	vector<PL_Type> typeList;
	vector<PL_Type>().swap(typeList);
	expressionList(stops,typeList);

	//size of variable access list and expression list must be same
	if(varTypeList.size() != typeList.size())
	{
		admin.error(ScopeE,lookAheadTok.getSymbol(),8);
	}
	else
	{
		for(int i = 0; i < varTypeList.size(); i++)
			if(varTypeList.at(i) !=  typeList.at(i))
				admin.error(ScopeE,lookAheadTok.getSymbol(),9);
	}
	admin.emit2("ASSIGN",varTypeList.size());
}

// ifStatement = 'if' guardedCommandList 'fi' 
void Parser::ifStatement(vector<Symbol> stops)
{
        for(int i=0; i<2; i++)
        {
            outFile1 << "       |\n";
        }
		outFile1<<"               ifStmt()"<<endl;
        // label of the instructions if the the boolean expression is false
	int startLabel = NewLabel();
        // label of the instruction immediately after exiting if-else 
	int doneLabel = NewLabel(); 
	vector<Symbol>().swap(stopSet);
	stopSet.push_back(FI);
	
	match(IF, ff.firstOfGCList() + stops);
	guardedCommandList(startLabel,doneLabel,stopSet + stops);

	admin.emit2("DEFADDR", startLabel);  
	// FI command.
	admin.emit2("FI", admin.lineNo);
	 // Define the address to jump to on successful completion of the command.
	admin.emit2("DEFADDR", doneLabel); 

	match(FI,stops);	
}

// doStatement = 'do' guardedCommandList 'od'
void Parser::doStatement(vector<Symbol> stops)
{
         for(int i=0; i<2; i++)
        {
            outFile1 << "       |\n";
        }
		outFile1<<"             doStatement()"<<endl;
	match(DO, ff.firstOfGCList() + stops);

	vector<Symbol>().swap(stopSet);
	stopSet.push_back(OD);
	int startLabel = NewLabel();
	int loopLabel = NewLabel();
	// Emit the label to loop back to.
	admin.emit2("DEFADDR", loopLabel);

	guardedCommandList(startLabel,loopLabel,stopSet + stops);

	// Emit the label that exits from the loop.
	admin.emit2("DEFADDR", startLabel);
	match(OD,stops);
}

// guardedCommandList = guardedCommand{'[]'guardedCommand}
void Parser::guardedCommandList(int& startLabel, int goTo,vector<Symbol> stops)
{
	outFile1<<"-> guardedCommandList()"<<endl;

	vector<Symbol>().swap(stopSet);
	stopSet.push_back(GC1);
	stopSet.push_back(FI);
	stopSet.push_back(OD);
	
	guardedCommand(startLabel,goTo,stopSet + stops);
	
	if(lookAheadTok.getSymbol() == GC1)
	{
		match(GC1, ff.firstOfGCList() + stops);
		guardedCommandList(startLabel,goTo,stops);
	}
	else if(in(ff.firstOfStatement()))
	{
		syntaxCheck(stops);	
	}
	else
	{
		syntaxCheck(stops - ID);
	}
}

// guardedCommand = expression '->' statementPart
void Parser::guardedCommand(int& thisLabel, int goTo,vector<Symbol> stops)
{
	outFile1<<"-> guardedCommand()"<<endl;
	admin.emit2("DEFADDR", thisLabel);

	vector<Symbol>().swap(stopSet);
	stopSet.push_back(GC2);	
	stopSet.push_back(RIGHTBRACKET);
	stopSet.push_back(RIGHTP);
	
	agc = true;
	PL_Type tempType = expression(stopSet + ff.firstOfStatement() + stops,0,NONAME);
	agc = false;

	thisLabel = NewLabel();
	admin.emit2("ARROW", thisLabel);

	match(GC2, ff.firstOfStatement() + stops);
	statementPart(stops);

	admin.emit2("BAR", goTo);
}

// expression = primaryExpression{primaryOperator primaryExpression}	
PL_Type Parser::expression(vector<Symbol> stops,int flag,Symbol sym)
{
	outFile1<<"->E()";
	
	PL_Type tempType = primaryExpression(ff.firstOfPrimOp() + stops);

	//emit in postfix fashion
	if(sym == AND)
		admin.emit1("AND");
	else if(sym == OR)
		admin.emit1("OR");
	if(flag == 1)
	{
		if(tempType != BOOLEAN)
		{
			admin.error(ScopeE,lookAheadTok.getSymbol(),7);
		}
	}
	if(in(ff.firstOfPrimOp()))
	{
		
		if(tempType != BOOLEAN)
		{
			admin.error(ScopeE,lookAheadTok.getSymbol(),7);
		}
		Symbol symb = lookAheadTok.getSymbol();
		primaryOperator(ff.firstOfExpList() + stops);
		expression(stops,1,symb);
	}
	else if(agc)
	{
		syntaxCheck(stops);
		return tempType;
	}
	else
	{
		syntaxCheck(stops - ID);
		return tempType;
	}
}

// primaryOperator = '&' | '|'
void Parser::primaryOperator(vector<Symbol> stops)
{
        outFile1<<"->primaryOp()";
	if(lookAheadTok.getSymbol() == AND)
	{
		match(AND,stops);		
	}	
	else if(lookAheadTok.getSymbol() == OR)
	{
		match(OR,stops);
	}
}

// primaryExpression = simpleExpression [relationalOperator simpleExpression]
PL_Type Parser::primaryExpression(vector<Symbol> stops)
{
        outFile1<<"->primaryE()";
	PL_Type tempType = simpleExpression(ff.firstOfRelOp() + stops);
	
	if(in(ff.firstOfRelOp()))
	{
		//relational operator is present so simple expression is expected to be integer
		if(tempType != INTEGRAL)
		{
			admin.error(ScopeE,lookAheadTok.getSymbol(),6);
		}
		Symbol symb = lookAheadTok.getSymbol();
		relationalOperator(ff.firstOfRelOp() + stops);
		tempType = simpleExpression(stops);

		if(symb == LESST)
			admin.emit1("LESS");
		else if(symb == EQUAL)
			admin.emit1("EQUAL");
		else
			admin.emit1("GREATER");

		//relational operator is present so simple expression is expected to be integer
		if(tempType != INTEGRAL)
		{
			admin.error(ScopeE,lookAheadTok.getSymbol(),6);
		}
		return BOOLEAN;
	}
	else
	{
		syntaxCheck(stops - ID);
		return tempType;
	}
}	

// relationalOperator = '<' | '=' | '>'	
void Parser::relationalOperator(vector<Symbol> stops)
{
         outFile1<<"->relOp()";
	if(lookAheadTok.getSymbol() == LESST)
	{
		match(LESST,stops);		
	}	
	else if(lookAheadTok.getSymbol() == EQUAL)
	{
		match(EQUAL,stops);
	}
	else if(lookAheadTok.getSymbol() == GREATERT)
	{
		match(GREATERT,stops);
	}
	

}

// simpleExpression = ['-'] term {addingOperator term}
PL_Type Parser::simpleExpression(vector<Symbol> stops)
{
        outFile1<<"->simpleE()";
	Symbol sym = NONAME;
	if(lookAheadTok.getSymbol() == MINUS)
	{
		match(MINUS, ff.firstOfTerm() + stops);
		sym = MINUS;
	}

	PL_Type tempType;
	term(tempType,ff.firstOfAddOp() + stops,0,NONAME);

	if(sym == MINUS)
		admin.emit1("SUBTRACT");
		
	if(in(ff.firstOfAddOp()))
	{
		if(tempType != INTEGRAL)
		{
			admin.error(ScopeE,lookAheadTok.getSymbol(),6);
		}
		addopTerm(ff.firstOfAddOp() + stops);
		return tempType;
	}
	else if(agc)
	{
		syntaxCheck(stops);
		return tempType;
	}
	else
	{
		syntaxCheck(stops - ID);
		return tempType;
	}
}

// addingOperator = '+' | '-'
void Parser::addingOperator(vector<Symbol> stops)
{
         outFile1<<"->addOp()";

	if(lookAheadTok.getSymbol() == PLUS)
	{
		match(PLUS,stops);		
	}	
	else if(lookAheadTok.getSymbol() == MINUS)
	{
		match(MINUS,stops);
	}
}

//helps to execute {addingOperator term}
void Parser::addopTerm(vector<Symbol> stops)
{
	if(in(ff.firstOfAddOp()))
	{
		Symbol symb = lookAheadTok.getSymbol();
		addingOperator(stops);
		PL_Type tempType ;
		term(tempType,stops,0,NONAME);

		if(symb == PLUS)
			admin.emit1("ADD");
		else if(symb == MINUS)
			admin.emit1("SUBTRACT");

		if(tempType != INTEGRAL)
		{
			admin.error(ScopeE,lookAheadTok.getSymbol(),6);
		}
		addopTerm(stops);
	}
}

// term = factor {multiplyingOperator factor}
void Parser::term(PL_Type& tempType,vector<Symbol> stops,int flag, Symbol sym)
{
         outFile1<<"->T()";
	tempType = factor(ff.firstOfMultOp() + stops);

	if(sym == TIMES)
		admin.emit1("MULTIPLY");
	else if(sym == DIV)
		admin.emit1("DIVIDE");
	else if(sym == MOD)
		admin.emit1("MODULO");

	
	if(flag == 1)
	{
		if(tempType != INTEGRAL)
		{
			admin.error(ScopeE,lookAheadTok.getSymbol(),6);
		}
	}
	if(in(ff.firstOfMultOp()))
	{
		
		if(tempType != INTEGRAL)
		{
			admin.error(ScopeE,lookAheadTok.getSymbol(),6);
		}
		Symbol symb = lookAheadTok.getSymbol();
		multiplyingOperator(stops);


		term(tempType,stops,1,symb);


	}	
	else if(in(ff.followOfExpression()))
	{
		syntaxCheck(stops);
	}
	else if(agc)
	{
		syntaxCheck(stops);
	}
	else
	{
		syntaxCheck(stops - ID);
	}
}

// multiplyingOperator = '*' | '/' | '\'
 void Parser::multiplyingOperator(vector<Symbol> stops)
{     
         outFile1<<"->mulOp()";
	if(lookAheadTok.getSymbol() == TIMES)
	{
		match(TIMES,stops);
	}	
	else if(lookAheadTok.getSymbol() == DIV)
	{
		match(DIV,stops);
	}
	else if(lookAheadTok.getSymbol() == MOD)
	{
		match(MOD,stops);
	}
}

//factor = constant | variableAccess | '('expression')' | '~' factor	
PL_Type Parser::factor(vector<Symbol> stops)
{
          outFile1<<"->F()";
	if(lookAheadTok.getSymbol() == ID)
	{

		TableEntry entry =  bTable.find(lookAheadTok.getValue(),bTable.error);

		if (entry.kind == CONSTANT)
		{
			int tempValue;
			PL_Type tempType;
			constant(tempValue, tempType, stops);

			admin.emit2("CONSTANT",tempValue);

			return tempType;
		}
		else
		{
			PL_Type tempType = variableAccess(stops);

			admin.emit1("VALUE");

			return tempType;
		}
	}
	else if(in(ff.firstOfConstant()))
	{
		int tempValue;
		PL_Type tempType;
		constant(tempValue, tempType, stops);

		admin.emit2("CONSTANT",tempValue);

		return tempType;
	}
	else if(lookAheadTok.getSymbol() == LEFTP)
	{
		vector<Symbol>().swap(stopSet);
		stopSet.push_back(RIGHTP);
	
		match(LEFTP, ff.firstOfExpList() + stopSet + stops);
		PL_Type tempType = expression(stopSet + stops,0,NONAME);
		match(RIGHTP,stops);

		return tempType;
	}
	else if(lookAheadTok.getSymbol() == NOT)
	{
		match(NOT, ff.firstOfFactor() + stops);
		factor(stops);	

		admin.emit1("NOT");
	}
	else if(in(ff.followOfExpression()))
	{
		admin.error(ParseE,lookAheadTok.getSymbol(),5);
		return UNIVERSAL;
	}
	
}

//variableAccess = variableName [indexSelector]
PL_Type Parser::variableAccess(vector<Symbol> stops)
{
	//look for the variable Name in block table
	int id = matchName(ID, stops);
         outFile1<<"->varAccess() " <<  ID;
	if(id != -1)
	{
		TableEntry entry =  bTable.find(id,bTable.error);
		//not found
		if(bTable.error)
		{
			admin.error(ScopeE,lookAheadTok.getSymbol(),4);
		}
		//found but kind is not variable
		else if(entry.kind != VAR)
		{
			admin.error(ScopeE,lookAheadTok.getSymbol(),1);
		}
		else
		{
			admin.emit3("VARIABLE",bTable.currentBlockLabel() - entry.bl,entry.disp);
		}

		if(in(ff.firstOfIndexSel()))
		{
			if(entry.type != INTEGRAL)
			{
				admin.error(ScopeE,lookAheadTok.getSymbol(),6);
			}
			vector<Symbol>().swap(stopSet);
			stopSet.push_back(RIGHTBRACKET);

			match(LEFTBRACKET, stopSet + ff.firstOfExpList() + stops);
			PL_Type tempType = expression(stopSet + stops,0,NONAME);
			match(RIGHTBRACKET,stops);

			// INDEX instruction emits the line number
			admin.emit3("INDEX", entry.size, admin.lineNo);

			return tempType;
		}
		else if(agc)
		{
			syntaxCheck(stops);
			return entry.type;
		}
		else
		{
			syntaxCheck(stops - ID);
			return entry.type;
		}
	}
}


//this function is used to match terminal symbols of CFG
bool Parser::match(Symbol sym , vector<Symbol> stops)
{
	//if the  token is matched returns true
	if(lookAheadTok.getSymbol() == sym)
	{
		while(1)
		{
			lookAheadTok = scanner.getToken();	
							
			//newline detected			
			if(lookAheadTok.getSymbol() == NEWLINE)
			{
				admin.NewLine();
			}
			//lexical errors
			else if(lookAheadTok.getSymbol() == BADNAME || lookAheadTok.getSymbol() == BADNUMERAL 
				|| lookAheadTok.getSymbol() == BADSYMBOL)
			{
				admin.error(ScanE,lookAheadTok.getSymbol(),0);
			}
			else if(lookAheadTok.getSymbol() != NONAME)
			{
				break;
			}	
		}	
		return true;
	}
	else
	{
		//report syntax error 
		admin.error(ParseE,sym,4);
		//recover from syntax error
		syntaxError(stops);
	}
	//check whether the look ahead symbol is valid or not
	syntaxCheck(stops);
}

//this function is used to match name symbols of CFG
int Parser::matchName(Symbol sym , vector<Symbol> stops)
{
	int temp = -1;

	//it means the token is matched returns position from symbol table
	if(lookAheadTok.getSymbol() == sym)
	{
		//index position of name symbol in symbol table
		temp = lookAheadTok.getValue();
		while(1)
		{
			lookAheadTok = scanner.getToken();
			//newline detected
			if(lookAheadTok.getSymbol() == NEWLINE)
			{
				admin.NewLine();
			}
			//lexical errors
			else if(lookAheadTok.getSymbol() == BADNAME || lookAheadTok.getSymbol() == BADNUMERAL
				|| lookAheadTok.getSymbol() == BADSYMBOL)
			{
				admin.error(ScanE,lookAheadTok.getSymbol(),0);
			}
			else if(lookAheadTok.getSymbol() != NONAME)
			{
				break;
			}
		}
		return temp;
	}
	else
	{
		//report syntax error
		admin.error(ParseE,sym,4);
		//recover from syntax error
		syntaxError(stops);
	}
	//check whether the look ahead symbol is valid or not
	syntaxCheck(stops);
	return temp;
}

void Parser::lookAheadToken()
{
	//take the first valid token.
	
	while(1)
	{	
		lookAheadTok = scanner.getToken();
		
		//newline detected
		if(lookAheadTok.getSymbol() == NEWLINE)
		{
			admin.NewLine();
		}
		//lexical errors
		else if(lookAheadTok.getSymbol() == BADNAME || lookAheadTok.getSymbol() == BADNUMERAL 
				|| lookAheadTok.getSymbol() == BADSYMBOL)
		{
			admin.error(ScanE,lookAheadTok.getSymbol(),0);
		}	
		else if(lookAheadTok.getSymbol() != NONAME)
		{
			break;
		}	
	}
}


//Syntex Error recovery

void Parser::syntaxError(vector<Symbol> stops)
{
	//continue taking token until stop symbol is found
	while(!in(stops))
	{
		lookAheadTok = scanner.getToken();	
		//newline detected
		if(lookAheadTok.getSymbol() == NEWLINE)
		{
			admin.NewLine();
		}
		//lexical errors
		else if(lookAheadTok.getSymbol() == BADNAME || lookAheadTok.getSymbol() == BADNUMERAL 
				|| lookAheadTok.getSymbol() == BADSYMBOL)
		{
			admin.error(ScanE,lookAheadTok.getSymbol(),0);
		}
	}
}

//after matching any terminal symbol we check whether our look ahead token is valid or not
void Parser::syntaxCheck(vector<Symbol> stops)
{
	//grab a look ahead token
	while(1)
	{	
		//newline detected
		if(lookAheadTok.getSymbol() == NEWLINE)
		{
			admin.NewLine();
		}
		//lexical errors
		else if(lookAheadTok.getSymbol() == BADNAME || lookAheadTok.getSymbol() == BADNUMERAL 
				|| lookAheadTok.getSymbol() == BADSYMBOL)
		{
			admin.error(ScanE,lookAheadTok.getSymbol(),0);
		}
		else if(lookAheadTok.getSymbol() != NONAME)
		{
			break;
		}	
		lookAheadTok = scanner.getToken();
	}
	//look ahead symbol not in stop set
	if(!in(stops))
	{
		//So report syntax error
		admin.error(ParseE,lookAheadTok.getSymbol(),5);	
		//recover from syntax error
		syntaxError(stops);
	}
}


//check whether the look ahead symbol is in the 'set' of symbol
bool Parser::in(vector<Symbol> set)
{
	for(int i = 0; i < set.size(); i++)
	{
		if(lookAheadTok.getSymbol() == set.at(i))
		{
			return true;
		}
	}
	return false;
}


//new label generator
int Parser::NewLabel()
{
	label++;
	return label;
}


	

