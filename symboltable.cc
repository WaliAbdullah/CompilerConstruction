#include "symboltable.h"

const string Symboltable::keywords[KEYWORDS] = {"begin", "end", "const", "array", "integer", "Boolean", "proc", "skip", "read", "write", "call", "if", "do", "fi", "od", "false", "true"};

	Symboltable::Symboltable()
	:occupied(1)
	{
		    //enum constant for keyword BEGIN, starts at 286
		int sym = 286; 
	
		vector<Token>().swap(htable);
		    // hash table initializing
		htable.reserve(SYMTABLESIZE);
		for( int i = 0; i < SYMTABLESIZE; ++i )
		    htable.push_back(Token(NONAME,-1,"noname"));
	
		    //preloading 17 reserve keywords with sname and lexeme
		for(int i=0; i < 17; i++)
		{
			Symbol castEnum = (Symbol)sym;
			Token tok(castEnum,-1,keywords[i]);
			insert(tok);
			sym +=1;
		}
	
	
	}
	//return true if table is full otherwise false;
	bool Symboltable::full()
	{
		return occupied == SYMTABLESIZE;
	}
	//return number of occupiend cells in symbol table
	int Symboltable::getOC()
	{
		return occupied;
	}


	    //To insert ID, const or keyword in Symbol-table
	Token Symboltable::insert(string s,int type)
	{
		
		//symbol table is full 
		if(full())
		{
			cerr<<"Symbol table is full. Program exits"<<endl;
			exit(0);
		}

		int isOccupied = find(s);
		
		if(isOccupied == -1)
		{	
			Token tk(ID,position,s);
			//tk.setIDtype(type);
			htable.at(position) = tk;
			//tk = htable.at(position);
			occupied++;
			return tk;
		}

		//position is occupied finds the next empty position and insert
		else
		{
			while(1)
			{
						
				Token tempTk;
				tempTk = htable.at(position); 	
						
				//263 is NONAME symbol so position is empty insert here
				if(tempTk.getSymbol() == 263)
				{	
				
					Token tk(ID,position,s);
					//tk.setIDtype(type);
					htable.at(position) = tk;
					//tk = htable.at(position);
					occupied++;
					return tk;
				
				}
				//go to the next position			
				else
				{
							
					int sym = tempTk.getSymbol();
				
					if(sym >= 286  && s == keywords[sym - 286] ) //encoding scheme for matching lexeme with keyword
					{
						return tempTk; // for keyword
					}				
					else if( s == tempTk.getLexeme())
						return tempTk; // same identifier already stored
				
					//last position so reset
					else
					{
						if(position > 306)
						{
							position = 0;
						}
			
						else
							position++;				
				
					}
				}
			}
		}
	
	}

	    //inserting reserve keywords into symbol table here
	void Symboltable::insert(Token tok)
	{
	
	    	//checks if position is occupied or not
		int isOccupied = find(tok.getLexeme());
		Token tk(tok.getSymbol(),-1,"");
	
	    	//if Not then insert token
		if(isOccupied == -1)
		{	
			htable.at(position) = tk;
			tk = htable.at(position);
			occupied++;
		}


		//position is occupied then go to the next position
		else
		{
			position++;
			while(1)
			{
						
				Token tempTk;
				tempTk = htable.at(position); 	
				
			
			
				    //if next position is empty, insert token
		        //263 is constant for NONAME
				if(tempTk.getSymbol() == 263)
				{	
					htable.at(position) = tk;
					tk = htable.at(position);
					occupied++;
					break;	
				
				}



				else
				{
					    //check the last position
					if(position > 306)
					{
						position = 0;
					}
					else
						position++;				
				
				
				}
			}
		}
	
	}
	    //find function
	int Symboltable::find(string lex)
	{
		position = hashfn(lex);
		Token tk;
		
		tk = htable.at(position); 
	
		if(tk.getSymbol() ==263)
			return -1;
		else
			return position;					
	
	
	}

	    //hash function
	int Symboltable::hashfn(string lex)
	{
		int asciiValue = 0;
		for(int i = 0; i < lex.length(); i++)
		{
			asciiValue += int(lex.at(i)); 	
		}	
	
		return asciiValue%307;
	
	}

	    //print symbol table
	void Symboltable::print()
	{

		cout<<endl<<"<<Symbol Table>>"<<endl;	
		for(int i=0; i < htable.size(); i++)
		{
			Token tempTk;
			tempTk = htable.at(i); 	
				
	    		//263 is NONAME symbol
			if(tempTk.getSymbol() != 263)
			{
				cout<<"Symbol ID: "<<tempTk.getSymbol() <<" Lexeme: "<<tempTk.getLexeme()<<" Position: "<<i<<endl;
			}
		}
	}
