#include "scanner.h"


	string lex="";

	//constructor
	Scanner::Scanner(ifstream &in, Symboltable &st)
	:srcFile(in), symTablePtr(st),idType(1)
	{
	
	}

	Token Scanner::recognizeName()
	{
			do
			  {	
				
				laCh = srcFile.peek();
					//check the next character		
				if(isalnum(laCh) || laCh == '_')
				{
				
					lex += laCh;
					srcFile.get();				
				}	
			

					//vald id or keyword detected		
				else if(isWhitespace(laCh) || isSpecial(laCh) || ch == EOF)
				{	
						//check if its an ID or reserve keyword
					Token keyOrId = symTablePtr.insert(lex,idType);
				
					return keyOrId;

				}
			

				else
				{
					Token tk(BADNAME,-1,"");
						// as error detected scanner continues to read chars untill it reaches end of line
					while(1)
					{	
						char inCh = srcFile.peek();
						if(isWhitespace(inCh) || isSpecial(inCh) || inCh == EOF)
							break;
						else
							srcFile.get();				
					}
				
					return tk;
				}
			
			 }
			  while(1);

	}

	Token Scanner::recognizeSpecial()
	{

			if(ch == '.')
			{
				Token tk(DOT,-1,"");
				return tk;
			}
			else if(ch == ',')
			{
				Token tk(COMMA,-1,"");
				return tk;
			}
			else if(ch == ';')
			{
				Token tk(SEMICOLON,-1,"");
				return tk;
			}
				//checking bracket or guarded command  
			else if(ch == '[')
			{
				//next char is also ']', it's a guarded command
				if(srcFile.peek() == ']')
				{
					srcFile.get();
					Token tk(GC1,-1,"");
					return tk;
				}
				//array 
				else
				{
					Token tk(LEFTBRACKET,-1,"");
					return tk;
				}
			}
			else if(ch == ']')
			{
				Token tk(RIGHTBRACKET,-1,"");
				return tk;
			}
			else if(ch == '&')
			{
				Token tk(AND,-1,"");
				return tk;
			}
			else if(ch == '|')
			{
				Token tk(OR,-1,"");
				return tk;
			}
			else if(ch == '~')
			{
				Token tk(NOT,-1,"");
				return tk;
			}
			else if(ch == ']')
			{
				Token tk(RIGHTBRACKET,-1,"");
				return tk;
			}
			else if(ch == '<')
			{
			
					Token tk(LESST,-1,"");
					return tk;
			
			}
			else if(ch == '>')
			{
			
					Token tk(GREATERT,-1,"");
					return tk;
			
			}
			else if(ch == '=')
			{
				Token tk(EQUAL,-1,"");
				return tk;
			}
			else if(ch == '+')
			{
				Token tk(PLUS,-1,"");
				return tk;
			}
				//checking minus or guarded command  	
			else if(ch == '-')
			{
				//next char is '>' then it's GC	
				if(srcFile.peek() == '>')
				{
					srcFile.get();
					Token tk(GC2,-1,"");
					return tk;
				}
					//minus symbol
				else
				{
					Token tk(MINUS,-1,"");
					return tk;
				}
			}
			else if(ch == '*')
			{
				Token tk(TIMES,-1,"");
				return tk;
			}
			else if(ch == '/')
			{
				Token tk(DIV,-1,"");
				return tk;
			}
			else if(ch == '\\')
			{
				Token tk(MOD,-1,"");
				return tk;
			}
			else if(ch == '(')
			{
				Token tk(LEFTP,-1,"");
				return tk;
			}
			else if(ch == ')')
			{
				Token tk(RIGHTP,-1,"");
				return tk;
			}
		
			else if(ch == ':')
			{
		
				if(srcFile.peek() == '=')
				{
					srcFile.get();
					Token tk(ASSIGN,-1,"");
					return tk;
				}
		
				else
				{
					Token tk(BADSYMBOL,-1,"");
					return tk;
				}
			}	

	}


	Token Scanner::recognizeNumeral()
	{

			//value for numerals
			int value = 0;
			do
			{
				laCh = srcFile.peek();
				//if next char is a valid numeral
				if(isdigit(laCh))
				{
				
					//value = (10*value) + (srcFile.get() - '0'); 
					lex += srcFile.get();
				}
			
				//to check end of number
				else if(isWhitespace(laCh) || isNumEnd(laCh))
				{	
					//check wheather the numeral is valid or not
					value = isNumeric(lex);				
					//valid numeral
					if(value != -1)				
					{
						Token tk(NUMERAL,value,"");
						return tk;
					}
					//Bad numeral
					else
					{
						Token tk(BADNUMERAL,-1,"");
						// scanner continues reading untill it reaches end of line
						while(1)
						{
							char inCh = srcFile.peek();
							if(isWhitespace(inCh) || isNumEnd(inCh) || inCh == EOF)
								break;		
							else
								srcFile.get();
						}
						return tk;		
					}
				}
				else
				{
					//if Bad numeral detected immediately after the first valid character
					Token tk(BADNUMERAL,-1,"");
				
					while(1)
					{
						char inCh = srcFile.peek();
						if(isWhitespace(inCh) || isNumEnd(inCh) || inCh == EOF)
							break;		
						else
							srcFile.get();
					}
					return tk;
				}

			}while(1);

	}

	Token Scanner::recognizeComment()
	{

			//look ahead the next char			
			laCh = srcFile.peek();	
			//continue reading until the next char is newline				
			while(srcFile.peek() != '\n' )
			{
				srcFile.get();
			}
		
			//NONAME token returned
			Token tk(NONAME,-1,lex);
				return tk;

	}

	//token generator
	Token Scanner::getToken()
	{	
		//read a character from source file
		ch = srcFile.get();
		//temorary token object. returns either valid or invalid token
		Token tok;
	

	
		//end of file
		if(ch == EOF)
		{
			Token tk(ENDOFFILE,-1,"");
			return tk;
		}
	
		//ignore comments in source file
		else if(ch == '$')
		{	
			return recognizeComment();
			
		}
		//recognize identifier or keyword	
		if (isalpha(ch))
		{
			lex = ch;
			return recognizeName();
			
		}
		//finite machine for detecting numeral
		else if(isdigit(ch))
		{
		
			lex = ch;
			return recognizeNumeral();
		}
		//new line char detected
		else if(ch == '\n')
		{
			Token tk(NEWLINE,-1,"");
			return tk;
		}

	 	//recognize special symbols
		else if(isSpecial(ch))
		{
			return recognizeSpecial();
			
				
		}

		//return NONAME
		else if(isWhitespace(ch))
		{
			return tok;
		}
		//otherwise, everything else falls into this category: bad symbol
		else
		{
			Token tk(BADSYMBOL,-1,"");
			while(1)
			{	
				char inCh = srcFile.peek();
				if(isWhitespace(inCh) || isSpecial(inCh) || inCh == EOF)
					break;
				else
					srcFile.get();
							
			}
			return tk;
		}
	
		return tok;
	}



	//helps to detect white spaces
	bool Scanner::isWhitespace(char lach)
	{
		if(lach == ' ' || lach == '\t' || lach == '\n')
		{	
			while(srcFile.peek() == ' ' || srcFile.peek() == '\t')
			{
				srcFile.get();				
			}		
			return true;
		}
		else 
			return false;
	}

	//helps to detect special symbols
	bool Scanner::isSpecial(char lach)
	{
		if(lach == '.' || lach == ',' || lach == ';' || lach == '[' || lach == ']' || lach == '&' || lach == '|'
	 || lach == '~' || lach == '<' || lach == '=' || lach == '>' || lach == '+' || lach == '-' || lach == '*' || lach == '/'
	 || lach == '\\' || lach == '(' || lach == ')' || lach == ':' || lach == '$' )
			return true;
		else 
			return false;
	}

	//helps to detect end of numerals as end of numeral is different form end of id
	bool Scanner::isNumEnd(char lach)
	{
		if(lach == ',' || lach == ';' || lach == ']' || lach == '<' || lach == '=' || lach == '>' || lach == '+' || lach == '-' || lach == '*' || lach == '/' || lach == '\\' || lach == ')' ||  lach == '$' )
			return true;
		else 
			return false;
	}

	//check wheather the numeral is good or bad numeral
	int Scanner::isNumeric(string s)
	{
		int value = 0;	
		if(s.length() > 10)
			return -1;
		else
		{
			for(int i = 0; i < s.length(); i++)
			{
				value = (10*value) + (s[i] - '0'); 
			}
			if(value>=0 && value<=2147483647)
				return value;
			else
				return -1;  	
		}
		
	}	

	//show symbol table in terminal
	void Scanner::printSymTable()
	{
		symTablePtr.print();
	}	
