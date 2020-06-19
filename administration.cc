
#include "administration.h"

const string Administration::terminals[TERMINALS] = { "id","num","bad num","bad name", "bad symbol", "badchar", "newline", "no name", "end of file", ".", ",", ";", "[", "]", "&", "|", "~", "<", "=", ">", "+", "-", "*", "/", "\\", ")", "(", ":=", "[]", "->", "begin", "end", "const", "array", "integer", "Boolean", "proc", "skip", "read", "write", "call", "if", "do", "fi", "od", "false", "true"};

	//constructor
	Administration::Administration(ofstream &out)
	:lineNo(1),outFile(out)
	{
	}

	int Administration::scan()
	{
		return errorCount;
	}


	    //count number of errors
	void Administration::ErrorCount()
	{	
		errorCount++;
		if(errorCount >= MAXERRORS)
		{
			cerr<<"Too many errors. Bailing out!!\n";
			exit(0);
		}
	
	}


	    //count number of lines
	void Administration::NewLine()
	{
		lineNo++;
	}



	int Administration:: getErrCount()
	{
		return errorCount;
	}


	    //checks wheather the token is valid or not. 
	    //If valid then returns true
	bool Administration::validTok(Symbol sym)
	{
		if(sym == NONAME || sym == BADNAME || sym == BADNUMERAL || sym == BADSYMBOL)
			return false;
		else 
			return true;
	}	


	    //Error reporting	
	void Administration::error(errorkind error,Symbol sym,int flag)
	{
		ErrorCount();	
		switch(error)
		{
	    		//lexical error
			case ScanE:
				cerr<<"Lexical Error::: '"<<terminals[sym-256]<<"' occured at line "<<lineNo<<endl;	
			break;
	    		//parsing error
			case ParseE:	
			    //this portions handles error in variableName, constantName and procedureName 
			if(flag < 4)
			{		
				string type[3] = {"variable", "constant", "procedure"};
				cerr<<"Syntax Error: Expecting name of type '"<<type[flag - 1]<<"' at line "<<lineNo<<endl;
			}
	    		//matching error
			else if(flag == 4)
			{
				string before = "at";
				if (sym == SEMICOLON)
					before = "on/before";	
				cerr<<"Syntax Error::: Missing '"<<terminals[sym-256]<<"' "<<before<< " line "<<lineNo<<endl;
			}
		    	//found token that was not supposed to be there 
			else if(flag == 5)
			{
				cerr<<"Syntax Error::: Illegal token '"<<terminals[sym-256]<<"' at line "<<lineNo<<endl;
			}
			else if(flag == 6)
			{
			
				cerr<<"Syntax Error::: Constant expected at line "<<lineNo<<endl;
			}
		
			break;

	    		//Scope and type error
			case ScopeE:
			if(flag < 4)
			{
				string kind[3] = {"variable", "constant", "procedure"};
				cerr<<"Scope Error::: Expecting name of kind '"<<kind[flag - 1]<<"' at line "<<lineNo<<endl;
			}
			else if(flag == 4)
			{
				cerr<<"Scope Error::: Undefined name at line "<<lineNo<<endl;
			}
			else if(flag == 5)
			{
				cerr<<"Scope Error::: Ambiguous name at line "<<lineNo<<endl;
			}
			else if(flag == 6 || flag == 7)
			{
				string type[3] = {"Integer", "Boolean"};
				cerr<<"Type Error::: Expecting name of type '"<<type[flag - 6]<<"' at line "<<lineNo<<endl;
			}
			else if(flag == 8)
			{
				cerr<<"Size Error::: Size of variable list and expression list mismatch at line "<<lineNo<<endl;
			}
			else if(flag == 9)
			{
				cerr<<"Type Error::: Type mismatch in assignment statement at line "<<lineNo<<endl;
			}
			break;
		}	
	}	

		//code generation part

	void Administration::emit1(string str)
	{
		outFile<<str<<endl;
	}

	void Administration::emit2(string str,int procLabel)
	{
		outFile<<str<<endl;
		outFile<<procLabel<<endl;

	}

	void Administration::emit3(string str,int varLabel,int startLabel)
	{
		outFile<<str<<endl;
		outFile<<varLabel<<endl;
		outFile<<startLabel<<endl;

	}

	void Administration::fatal(string message)
	{

		cerr << message << endl;
		exit(0);
	}

		//Completion message
	void Administration::complete()
	{
		      
		cout<<endl<<"*** Compilation complete ***"<<endl;
		cout<<"Number of lines Parsed: "<<lineNo<<endl;
		outFile.close();
	}

