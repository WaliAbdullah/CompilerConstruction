
#ifndef ADMINISTRATION_H
#define ADMINISTRATION_H

#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <vector>
#include "token.h"
#include "scanner.h"
#include "blocktable.h"


	// This is the maximum number of errors before the compiler bails out.
#define MAXERRORS 50
#define TERMINALS 47	


enum errorkind {ScanE, ParseE, ScopeE, TypeE};

using namespace std;

class Administration
{

  public:
	//flag for printing symbol table
	bool printSymbolTable;

	Administration(ofstream &out);

	~Administration() {}

		// Begin a new line
	void NewLine();

	int lineNo;

		// Error function for the phases
	void error(errorkind , Symbol, int);
		//fatal error
	void fatal(string);
		//function for error count
	void ErrorCount();	
		//get number of errors
		int getErrCount();
	
		// call scanner from here
	int scan();

	//either valid or invalid token 
	bool validTok(Symbol sym);

	static const string terminals[TERMINALS];
	
	void complete();

		// code generation phase
	void emit1(string);
	void emit2(string,int);
	void emit3(string,int, int);

  private:

	ofstream &outFile;
	
	bool correctline;
	
		//get token from scan() function	
	Token nextTok;

		//count the number of errors.
	int errorCount;

};

#endif
