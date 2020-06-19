
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <cstring>
#include "symbol.h"	
#include "scanner.h"
#include "administration.h"
#include "parser.h"
#include "symboltable.h"
#include "blocktable.h"

#include "Assembler.h"
#include "interp.h"


int main(int argc, char *argv[]) {
    
		//source file object	
	ifstream srcFile;
		//output file object
	ofstream outFile;
		//print symbol table upon user input
	bool printSymTab = false;
	

	if(argc < 2)
	{
		cout<<"Missing input file name."<<endl;
		return 1;
	}
	
	//create a symbol table object
	Symboltable st;

	    //create a Scanner object
	Scanner scanner(srcFile,st);
	
	srcFile.open(argv[1]);
	outFile.open("ParserOutFile");

	if (!srcFile)
	{
		 cerr << "Input file " << argv[1] << " could not be opened" << endl;
		 return 1;
	}
	

		// To store intermediate code, we have defined the file format here.
	char intermediateCode[1024];
	strcpy(intermediateCode,"PL_interFile_");
	strcat(intermediateCode, argv[1]);
	strcat(intermediateCode, ".asm");
	ofstream assemOutputFile;

	assemOutputFile.open(intermediateCode, ios::out);
	 if (!assemOutputFile)
	 {
		 cerr << "Intermediate file " << intermediateCode << " could not be written" << endl;
		 return 1;
	 }


	BlockTable bTable;
		//Parser calss object	
	Parser parser(srcFile,  outFile, assemOutputFile, scanner, bTable);
		//Strat Parsing  
	int countError = parser.parse();
	
	srcFile.close();
	assemOutputFile.close();
	outFile.close();
	
	if (countError == 0) // will execute if there will be no error in the input PL program
	{
		ifstream assemInputFile;
		assemInputFile.open(intermediateCode, ios::in);

		if (!assemInputFile)
		{
		    cerr << "Intermediate file " << intermediateCode << " could not be read"<< endl;
		    return 1;
		}
			//machine code with resolved forward references are always stored in this file
		ofstream outputfile("MachineCode");
		if (!outputfile)
		{
		    cerr << "Output file " << argv[3] << " could not be opened"<< endl;
		    return 1;
		}

		Assembler assembler(assemInputFile, outputfile);
		assembler.firstPass();
		assemInputFile.seekg(0);
		assembler.secondPass();
		assemInputFile.close();
		outputfile.close();
		Interpreter interpreter("MachineCode", false);
	}
}
