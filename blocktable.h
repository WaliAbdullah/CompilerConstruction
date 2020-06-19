#ifndef BLOCKTABLE_H
#define BLOCKTABLE_H

#include <vector>
#include <iostream>
#include <string>
#include <cstdlib>
#include "type.h"
#define MAXBLOCKS 10
#define MAXDEFINITIONS 30

using namespace std;

typedef struct
{
	int id;
	PL_Kind kind;
	PL_Type type;
	int size;
	int value;
	int bl;  //block level
	int disp;// displacement
}TableEntry;

class BlockTable
{
	private:
	//keeps track of the current block
	int blockLevel;
	//vector of block table	
	typedef vector<TableEntry> table;
	typedef vector<table> block;
	//vector of vector
	block myBlock;
	

	public:
	//default constructor
	BlockTable();
	~BlockTable(){};
	//error is false if name matches on the block table, false otherwise
	bool error;
	//returns true if the id we are looking for is found
	//in the current block. false otherwise
	bool search(int);
	//returns true if the current block doesn't contain same ID more than once, false otherwise
	bool define(int,PL_Kind,PL_Type,int,int,int);
	//True if ID is found in the current block, false otherwise
	TableEntry find(int,bool&);
	//new block
	bool newBlock();
	//Defining array size
	void setArraySize(int,int);
	//index for name definitions
	int def;
	//set start label
	void setStartLabel(int);
	int currentBlockLabel();
	void endBlock();
		
};

#endif


