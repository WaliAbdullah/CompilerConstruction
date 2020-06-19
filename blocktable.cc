#include "blocktable.h"

	BlockTable::BlockTable()
	:myBlock(MAXBLOCKS,table(MAXDEFINITIONS)),blockLevel(-1),def(0)
	{
	
		for(int i = 0; i < MAXBLOCKS ; i++)
			for(int j = 0; j < MAXDEFINITIONS; j++)
				myBlock[i][j].id = -1;
	}

	//returns true if the id is found in the current block , false otherwise
	bool BlockTable::search(int idToLook)
	{
		for(int i = 0; i <= def; i++)
		{
			if(myBlock[blockLevel][i].id == idToLook)
				return true;
			else if(myBlock[blockLevel][i].id == -1)
				return false;
		}
		return false;
	}

	//returns true if current block doesn't contain same ID more than once, otherwise false
	bool BlockTable::define(int nID, PL_Kind nKind, PL_Type nType, int nSize, int nValue, int nDisp)
	{
		if(!search(nID))
		{
			//define attributes
			myBlock[blockLevel][def].id = nID;
			myBlock[blockLevel][def].kind = nKind;
			myBlock[blockLevel][def].type = nType;
			myBlock[blockLevel][def].size = nSize;
			myBlock[blockLevel][def].value = nValue;
			myBlock[blockLevel][def].bl = blockLevel;
			myBlock[blockLevel][def].disp = nDisp;

			def++;

			return true;
		}
		else
			return false;

	}

	//returns false if the ID is in the block, search all blocks, if not return true
	TableEntry BlockTable::find(int idToLook, bool &err)
	{
		for(int i = blockLevel; i >= 0; i--)
			for(int j = 0; j < MAXDEFINITIONS; j++)
			{
				if(myBlock[i][j].id == -1)
					break;
				else if(myBlock[i][j].id == idToLook)
				{
					err = false;
					return myBlock[i][j];
				}
			}

		//not found
		err = true;
		TableEntry dummyEntry;
		dummyEntry.id = -1;
		return dummyEntry;

	}
	//new block in stack
	bool BlockTable::newBlock()
	{
		def = 0;
		blockLevel++;
		if(blockLevel > 9)
		{
			return false;
			//cerr<<"Symbol table is full. Program exits"<<endl;
			//exit(0);
		}
		return true;
	}

	void BlockTable::setStartLabel(int sl)
	{
		myBlock[blockLevel][def-1].disp = sl;
	}
	//setSize for Arrays
	void BlockTable::setArraySize(int defPosition, int size)
	{
		for(int i = defPosition; i < def; i++)
		{
			myBlock[blockLevel][i].size = size;
		}
	}

	int BlockTable::currentBlockLabel()
	{
		return blockLevel;
	}
	void BlockTable::endBlock()
	{
		blockLevel--;
	}
