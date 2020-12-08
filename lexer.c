// Group Number - 25
// Name : Aayush Atul Verma         ID : 2017A7PS0061P
// Name : Aditya Ramaswamy          ID : 2017A7PS0130P
// Name : Jithin Kallukalam Sojan   ID : 2017A7PS0163P

#include<string.h>
#include<ctype.h>
#include<math.h>
#include "lexerDef.h"
#include "Lexer-Accessory.h"


/*This function takes in an identifier as a parameter by reference and checks if it is a keyword. It returns -1 if an identifier was not passed, 0 if the identifier was not a keyword and 1 if the identifier was a keyword.*/
int Lookup(Token *identifier)
{
	if(identifier->lex!=ID)
	{
		return -1;
	}
	else
	{
		if(!strcmp(identifier->value.identifier,"integer"))
		{
			identifier->value.key=INTEGER;
			identifier->lex=KEYWORD;
		}
		else if(!strcmp(identifier->value.identifier,"real"))
		{
			identifier->value.key=REAL;
			identifier->lex=KEYWORD;
		}
		else if(!strcmp(identifier->value.identifier,"boolean"))
		{
			identifier->value.key=BOOLEAN;
			identifier->lex=KEYWORD;
		}
		else if(!strcmp(identifier->value.identifier,"of"))
		{
			identifier->value.key=OF;
			identifier->lex=KEYWORD;
		}
		else if(!strcmp(identifier->value.identifier,"array"))
		{
			identifier->value.key=ARRAY;
			identifier->lex=KEYWORD;
		}
		else if(!strcmp(identifier->value.identifier,"start"))
		{
			identifier->value.key=START;
			identifier->lex=KEYWORD;
		}
		else if(!strcmp(identifier->value.identifier,"end"))
		{
			identifier->value.key=END;
			identifier->lex=KEYWORD;
		}
		else if(!strcmp(identifier->value.identifier,"declare"))
		{
			identifier->value.key=DECLARE;
			identifier->lex=KEYWORD;
		}
		else if(!strcmp(identifier->value.identifier,"module"))
		{
			identifier->value.key=MODULE;
			identifier->lex=KEYWORD;
		}
		else if(!strcmp(identifier->value.identifier,"driver"))
		{
			identifier->value.key=DRIVER;
			identifier->lex=KEYWORD;
		}
		else if(!strcmp(identifier->value.identifier,"program"))
		{
			identifier->value.key=PROGRAM;
			identifier->lex=KEYWORD;
		}
		else if(!strcmp(identifier->value.identifier,"record"))
		{
			identifier->value.key=RECORD;
			identifier->lex=KEYWORD;
		}
		else if(!strcmp(identifier->value.identifier,"tagged"))
		{
			identifier->value.key=TAGGED;
			identifier->lex=KEYWORD;
		}
		else if(!strcmp(identifier->value.identifier,"union"))
		{
			identifier->value.key=UNION;
			identifier->lex=KEYWORD;
		}
		else if(!strcmp(identifier->value.identifier,"get_value"))
		{
			identifier->value.key=GET_VALUE;
			identifier->lex=KEYWORD;
		}
		else if(!strcmp(identifier->value.identifier,"print"))
		{
			identifier->value.key=PRINT;
			identifier->lex=KEYWORD;
		}
		else if(!strcmp(identifier->value.identifier,"use"))
		{
			identifier->value.key=USE;
			identifier->lex=KEYWORD;
		}
		else if(!strcmp(identifier->value.identifier,"with"))
		{
			identifier->value.key=WITH;
			identifier->lex=KEYWORD;
		}
		else if(!strcmp(identifier->value.identifier,"parameters"))
		{
			identifier->value.key=PARAMETERS;
			identifier->lex=KEYWORD;
		}
		else if(!strcmp(identifier->value.identifier,"true"))
		{
			identifier->value.key=TRUE;
			identifier->lex=KEYWORD;
		}
		else if(!strcmp(identifier->value.identifier,"false"))
		{
			identifier->value.key=FALSE;
			identifier->lex=KEYWORD;
		}
		else if(!strcmp(identifier->value.identifier,"takes"))
		{
			identifier->value.key=TAKES;
			identifier->lex=KEYWORD;
		}
		else if(!strcmp(identifier->value.identifier,"input"))
		{
			identifier->value.key=INPUT;
			identifier->lex=KEYWORD;
		}
		else if(!strcmp(identifier->value.identifier,"returns"))
		{
			identifier->value.key=RETURNS;
			identifier->lex=KEYWORD;
		}
		else if(!strcmp(identifier->value.identifier,"AND"))
		{
			identifier->value.key=AND;
			identifier->lex=KEYWORD;
		}
		else if(!strcmp(identifier->value.identifier,"OR"))
		{
			identifier->value.key=OR;
			identifier->lex=KEYWORD;
		}
		else if(!strcmp(identifier->value.identifier,"for"))
		{
			identifier->value.key=FOR;
			identifier->lex=KEYWORD;
		}
		else if(!strcmp(identifier->value.identifier,"in"))
		{
			identifier->value.key=IN;
			identifier->lex=KEYWORD;
		}
		else if(!strcmp(identifier->value.identifier,"switch"))
		{
			identifier->value.key=SWITCH;
			identifier->lex=KEYWORD;
		}
		else if(!strcmp(identifier->value.identifier,"case"))
		{
			identifier->value.key=CASE;
			identifier->lex=KEYWORD;
		}
		else if(!strcmp(identifier->value.identifier,"break"))
		{
			identifier->value.key=BREAK;
			identifier->lex=KEYWORD;
		}
		else if(!strcmp(identifier->value.identifier,"default"))
		{
			identifier->value.key=DEFAULT;
			identifier->lex=KEYWORD;
		}
		else if(!strcmp(identifier->value.identifier,"while"))
		{
			identifier->value.key=WHILE;
			identifier->lex=KEYWORD;
		}
		else
		{
			return 0;
		}
		return 1;
	}
}
/*This function is the lexer. It returns a token from the input stream. There are three parameters passed to this function. The first parameter is the input stream. The second parameter keeps track of the current line number. It is passed by reference. The third parameter, also passed by reference, is used especially to deal with a range operator that occurs after an integer. This is done because only one character can be pushed back into the input stream by ungetc.*/
Token Lexer(FILE *input, int *line, bool *DDot)
{
	//The following token is what is eventually returned. It's various parameters are initialized.
	Token toReturn;
	toReturn.lex=EMPTY_TOKEN;
	toReturn.length=0;
	toReturn.line_no = 0;
	//Does this require any changes.
	toReturn.value.integer=0;
	toReturn.error=false;
	/*The lexer works by simulating a deterministic finite automaton. The variable 'state' stores the current state of this automaton. State 1 is the initial state.*/
	int state=1;
	//'c' reads from the input stream character by character.
	char c=0;
	/*The switch statement is the automaton itself. Each case is a state. The terminal states are those with a return statement. A transition is an assignment to 'state', followed by another iteration of the while loop.*/
	while(1)
	{
		switch(state)
		{
			case 1:
				if(EOF==(c=fgetc(input)))
				{
					toReturn.lex=END_OF_INPUT_STREAM;
					return toReturn;
				}
				//All whitespace characters to be ignored.
				else if(c==' '||c=='\t')
				{
				}
				//Go to state 2 if there is a newline.
				else if(c=='\n')
				{
					state=2;
				}
				/*Go to state 3 if an alphabetical character is encountered. Store all encountered characters, to be used in a symbol table.*/
				else if(isalpha(c))
				{
					state=3;
					toReturn.lex=ID;
					toReturn.value.identifier[toReturn.length++]=c;
				}
				/*If a digit is encountered go to state 3. Store this digit to be used in semantics.*/
				else if(isdigit(c))
				{
					state=7;
					toReturn.lex=NUM;
					toReturn.value.integer*=10;	
					toReturn.value.integer+=c-'0';
				}
				//If a + is encountered go to state 16.
				else if(c=='+')
				{
					state=16;
				}
				//If a - is encountered go to state 17.
				else if(c=='-')
				{
					state=17;
				}
				//If a , is encountered go to state 18.
				else if(c==',')
				{
					state=18;
				}
				//If a / is encountered go to state 19.
				else if(c=='/')
				{
					state=19;
				}
				//If a ! is encountered go to state 20.
				else if(c=='!')
				{
					state=20;
				}
				//If a = is encountered go to state 22.
				else if(c=='=')
				{
					state=22;
				}
				//If a . is encountered go to state 24.
				else if(c=='.')
				{
					state=24;
				}
				//If a > is encountered go to state 26.
				else if(c=='>')
				{
					state=26;
				}
				//If a : is encountered go to state 30.
				else if(c==':')
				{
					state=30;
				}
				//If a < is encountered go to state 33.
				else if(c=='<')
				{
					state=33;
				}
				//If a * is encountered go to state 37.
				else if(c=='*')
				{
					state=37;
				}
				//If a [ is encountered go to state 42.
				else if(c=='[')
				{
					state=42;
				}
				//If a ] is encountered go to state 43.
				else if(c==']')
				{
					state=43;
				}
				//If a ; is encountered go to state 44.
				else if(c==';')
				{
					state=44;
				}
				//If a ( is encountered go to state 45.
				else if(c=='(')
				{
					state=45;
				}
				//If a ) is encountered go to state 46.
				else if(c==')')
				{
					state=46;
				}
				/*If any other character is encountered prepare for an error. Since no token is currently being processed return an empty token.*/
				else
				{
					toReturn.lex=EMPTY_TOKEN;
					toReturn.error=true;
					toReturn.length=*line;
					toReturn.value.identifier[0]=c;
					return toReturn;
				}	
				break;
				/*If a newline is encountered increment line. This is done purely for error reporting. While not strictly a DFA transition as no input character is read, there is no way else to deal with storing line counts. The next iteration starts in state 1 again.*/
			case 2:
				*line=*line+1;
				state=1;
				break;
				/*The following sequence of characters represents either a keyword or an identifier.*/
			case 3:
				if(EOF==(c=fgetc(input)))
				{
					state = 6;
					//This is to take care of the fact that a keyword like end might hae EOF right after it.
				}
				/*Keep reading characters as long as they're alphabets or the _. Keep storing these characters for further use.*/
				else if(isalpha(c)||c=='_')
				{
					if(toReturn.length>=21)
					{
						toReturn.error = true;
						toReturn.lex=ID;
						toReturn.error=true;
						toReturn.length=*line;
						toReturn.value.identifier[0]=c;
						return toReturn;

					}
					toReturn.value.identifier[toReturn.length++]=c;
				}
				/*If a digit is encountered this string cannot be a keyword. Go on to state 4 for this case.*/
				else if(isdigit(c))
				{
					state=4;
					if(toReturn.length>=21)
					{
						toReturn.error = true;
						toReturn.lex=ID;
						toReturn.error=true;
						toReturn.length=*line;
						toReturn.value.identifier[0]=c;
						return toReturn;

					}
					toReturn.value.identifier[toReturn.length++]=c;
				}
				/*A lack of any further alphanumerics or _ means the end of this token. Go on to state 6.*/
				else
				{
					state=6;
				}
				break;
			case 4:
				if(EOF==(c=fgetc(input)))
				{
					state = 5;
					//This is to take care of the situation where an EOF occurs right after an identifier.
				}
				/*Any alphanumerics or _ must be a part of this identifier. Keep tracking these characters for further use.*/
				else if(isalnum(c)||c=='_')
				{
					if(toReturn.length>=21)
					{
						toReturn.error = true;
						toReturn.lex=ID;
						toReturn.error=true;
						toReturn.length=*line;
						toReturn.value.identifier[0]=c;
						return toReturn;

					}
					toReturn.value.identifier[toReturn.length++]=c;
				}
				/*Any other character represents the end of this token. In this case go to state 5.*/
				else
				{
					state=5;
				}
				break;
			case 5:
				/*In this case push back the extra character back into the input stream. Then return the identifier as a token.*/
				if(EOF!=c)ungetc(c,input);
				toReturn.value.identifier[toReturn.length]=0;
				return toReturn;
			case 6:
				//Return the extra character back to the input stream.
				if(EOF!=c)ungetc(c,input);
				toReturn.value.identifier[toReturn.length]=0;
				/*This string could infact be a key word. USe the Lookup function to check if it is actually a keyword. A return of -1 means that things have gone horribly wrong. This should never actually occur.*/
				if(-1==Lookup(&toReturn))
				{
					printf("This code should never execute.\n");
				}
				return toReturn;
				/*The following sequence of characters represents either an integer or a real value.*/
			case 7:
				if(EOF==(c=fgetc(input)))
				{
					state = 8;
					//This is to take care of the case where an EOF occurs right after an integer.
				}
				/*If a digit is encountered, it needs to be kept track of. No change of state is necessary in this case.*/
				else if(isdigit(c))
				{
					toReturn.value.integer*=10;	
					toReturn.value.integer+=c-'0';
				}
				/*If a dot is encountered, the number is either a real number or the dot is a part of an instance of the range operator. A change of state to state 9 is necessary to reflect this.*/
				else if(c=='.')
				{
					state=9;
					toReturn.lex=RNUM;
				}
				/*Any other character signals the end of the integer token. The state must then change to 8.*/
				else
				{
					state=8;
				}
				break;
			case 8:
				/*Return the extra character read in state 7 to the input stream. Return the integer so far obtained as the token.*/
				if(c!=EOF)ungetc(c,input);
				return toReturn;
			case 9:
			       	c=fgetc(input);
				/*If another dot has been read, the range operator was encountered. Go to state 10 in this case.*/
				if(c=='.')
				{
					state=10;
				}
				/*If a digit is now encountered, a real value is being parsed. Go to state 11 in this case.*/
				else if(isdigit(c))
				{
					toReturn.value.real=toReturn.value.integer;
					toReturn.value.real+=(c-'0')/pow(10,++toReturn.length);
					state=11;
				}
				/*Any other character is unexpected and is a lexical error. This error was encountered while parsing a real number.*/
				else
				{
					toReturn.lex=RNUM;
					toReturn.error=true;
					toReturn.length=*line;
					toReturn.value.identifier[0]=c;
					return toReturn;
				}
				break;
			case 10:
				/*Only one character can be returned to the input stream, unlike the two we actually need to return for the range operator. To deal with this, set the flag DDot, to be dealt with in the next call of Lexer. Also return the current lexeme as an integer.*/
				toReturn.lex=NUM;
				*DDot=true;
				ungetc(c,input);
				return toReturn;
			case 11:		
				if(EOF==(c=fgetc(input)))
				{
					state = 15;
					//Incase there is an EOF right after an RNUM.
				}
				/*If a digit was read then keep track of it as a float. Use the length parameter to keep track of where to store the next digit in the float. No state change is necessary in this case.*/
				else if(isdigit(c))
				{
					toReturn.value.real+=(c-'0')/pow(10,++toReturn.length);
				}
				/*To deal with floats of the form 1.2E-1. Go to state 12.*/
				else if(c=='E'||c=='e')
				{
					state=12;
					toReturn.length=0;
				}
				/*Any other character signals the end of this token. Go to state 15.*/
				else
				{
					state=15;
				}
				break;
			case 12:
			       	c=fgetc(input);
				/*A + or - sign can be encountered after the E or e. Go to state 13 to deal with this. Use the length parameter to store the exponent. To do this, store the sign in this parameter as 1 or -1.*/
				if(c=='+'||c=='-')
				{
					toReturn.length=(c=='+'?1:-1);
					state=13;
				}
				/*Store the exponent in the length parametere. Go to state 14 to deal with this case. No explicit sign assumes the exponent to be positive.*/
				else if(isdigit(c))
				{
					toReturn.length*=10;
					toReturn.length+=c-'0';
					state=14;
				}
				/*If any other character is encountered an error has occurred. And the token that was being parsed is a real number. Sotre the line number and the extraneous character and return.*/
				else
				{
					toReturn.lex=RNUM;
					toReturn.error=true;
					toReturn.length=*line;
					toReturn.value.identifier[0]=c;
					return toReturn;
				}
				break;
			case 13:
			       	c=fgetc(input);
				/*If it's a digit keep track of it as the length parameter. This is done by multiplication because length stores the sign in th eprevious state. Go to state 14.*/
				if(isdigit(c))
				{
					toReturn.length*=c-'0';
					state=14;
				}
				/*State 13 is used to ensure that atleast one digit occurs after the + or - sign. Any other character is a lexical error that occurred while parsing a float.*/
				else
				{
					toReturn.lex=RNUM;
					toReturn.error=true;
					toReturn.length=*line;
					toReturn.value.identifier[0]=c;
					return toReturn;
				}
				break;
			case 14:		
				if(EOF==(c=fgetc(input)))
				{
					state = 15;
					//Incase there is an EOF right after the RNUM.
				}
				/*Keep track of the exponent in the length parameter. Since the parameter can be either positive or negative the new digit must either be added or subtracted accordingly.*/
				else if(isdigit(c))
				{
					toReturn.length*=10;
					if(toReturn.length<0)
						toReturn.length-=c-'0';
					else
						toReturn.length+=c-'0';
				}
				/*Any other character signals the end of this float. Store the value of the exponent into the float to get the final form of the float.*/
				else
				{
					state=15;
					toReturn.value.real*=pow(10,toReturn.length);
				}
				break;
			case 15:
				/*Return the extra character to the input stream. Return this token also.*/
				if(c!=EOF)ungetc(c,input);
				return toReturn;
			case 16:
				/*If the character ecnountered is a plus, return the apropriate lexeme.*/
				toReturn.lex=PLUS;
				return toReturn;
			case 17:
				/*If the character encountered is a minus, return the MINUS lexeme.*/
				toReturn.lex=MINUS;
				return toReturn;
			case 18:
				/*If the character encountered is a comma, return the COMMA lexeme.*/
				toReturn.lex=COMMA;
				return toReturn;
			case 19:
				/*If the character encountered is a forward slash, return the DIV lexeme.*/
				toReturn.lex=DIV;
				return toReturn;
			case 20:
				/*If the character encountered is a '!', another character must be read. If this is also '=', go to state 21.*/
				c=fgetc(input);
				if(c=='=')
				{
					state=21;
				}
				/*If the character is not '=' an error has been encountered while looking for '!='. In this case set the lex field to NE and return the token.*/
				else
				{
					toReturn.lex=NE;
					toReturn.error=true;
					toReturn.length=*line;
					toReturn.value.identifier[0]=c;
					return toReturn;
				}
				break;
			case 21:
				/*Set the lexeme to NE and return the token.*/
				toReturn.lex=NE;
				return toReturn;
			case 22:
				/*If the character encountered is a '=', another character must be read. If this is also '=', go to state 23.*/
				c=fgetc(input);
				if(c=='=')
				{
					state=23;
				}
				/*If the character is not '=' an error has been encountered while looking for '=='. In this case set the lex field to EQ and return the token.*/
				else
				{
					toReturn.lex=EQ;
					toReturn.error=true;
					toReturn.length=*line;
					toReturn.value.identifier[0]=c;
					return toReturn;
				}
				break;
			case 23:
				/*Set the lexeme to EQ and return the token.*/
				toReturn.lex=EQ;
				return toReturn;
			case 24:
				/*If a dot has been encountered, there are two possibilities. One situation is when the '.' was pushed back while parsing a range operator immediately following an integer. In this case DDot will have been set. In such a case immediately return the RANGEOP token.*/
				if(*DDot)
				{
					toReturn.lex=RANGEOP;
					*DDot=false;
					return toReturn;
				}
				/*If DDot was not set, then another character must be read. If this character is also a dot, then go to state 25, where the token RANGEOP will be returned.*/
				else if('.'==(c=fgetc(input)))
				{
					state=25;
				}
				/*If an alternate character is returned, then a lexical error has been encountered while looking for '..'. In this case set the lex field as RANGEOP and return the token.*/
				else
				{
					toReturn.lex=RANGEOP;
					toReturn.error=true;
					toReturn.length=*line;
					toReturn.value.identifier[0]=c;
					return toReturn;
				}
				break;
			case 25:
				/*Return the token RANGEOP by setting the lex field.*/
				toReturn.lex=RANGEOP;
				return toReturn;
			case 26:
				/*If a '>' was encountered earlier there are three possible tokens. Another character has to be read to decide the correct token.*/
				if(EOF==(c=fgetc(input)))
				{
					state = 29;
				}
				/*If a '>' is read again go to state 27.*/
				else if(c=='>')
				{
					state=27;
				}
				/*If a '=' is read again go to state 28.*/
				else if(c=='=')
				{
					state=28;
				}
				/*If any other character is read, the token has the be '>'. In this case go to state 29.*/
				else
				{
					state=29;
				}
				break;
			case 27:
				/*In this case the token is '>>'. Set the lex field to ENDDEF and return the token.*/
				/*toReturn.lex=ENDDEF;
				return toReturn;*/
				if(EOF==(c=fgetc(input)))
				{
					state = 50;
				}
				else if(c=='>')
				{
					state=49;
				}
				else
				{
					state=50;
				}
				break;
			case 28:
				/*In this case the token is '>='. Set the lex field to GE and return the token.*/
				toReturn.lex=GE;
				return toReturn;
			case 29:
				/*In this case the last character must be returned to the input stream, the lex field must be set to GT and 'toReturn' must be returned.*/
				if(c!=EOF)ungetc(c,input);
				toReturn.lex=GT;
				return toReturn;
			case 30:
				if(EOF==(c=fgetc(input)))
				{
					state = 32;
				}
				/*If a '=' is encountered now the lexeme is ':='. In this case the next state is 31.*/
				else if(c=='=')
				{
					state=31;
				}
				/*If any other character is encountered the lexeme is ':'. In this case the next state is 32.*/
				else
				{
					state=32;
				}
				break;
			case 31:
				/*The lex field is set to ASSIGNOP and the token is returned.*/
				toReturn.lex=ASSIGNOP;
				return toReturn;
			case 32:
				/*The lex field is set to COLON and the token is returned.*/
				toReturn.lex=COLON;
				if(c!=EOF)ungetc(c,input);
				return toReturn;
			case 33:
				if(EOF==(c=fgetc(input)))
				{
					state = 35;
				}
				/*The first character encountered to reach this state is '<'. If the next character read is '<' the token is the beginning of a function definition '<<'. In this case the next state is 34.*/
				else if(c=='<')
				{
					state=34;
				}
				/*If the next character read is '=' the token is less than equal to '<='. In this case the next state is 36.*/
				else if(c=='=')
				{
					state=36;
				}
				/*If any other character is encountered the token is just '<'. In this case the next state is 35.*/
				else
				{
					state=35;
				}
				break;
			case 34:
				/*The token here is '<<'. The lex field must be set to DEF and the lexer needs to return.*/
				/*toReturn.lex=DEF;
				return toReturn;*/
				if(EOF==(c=fgetc(input)))
				{
					state = 48;
				}
				else if(c=='<')
				{
					state=47;
				}
				else{
					state = 48;
				}
				break;

			case 35:
				/*The token here is '<'. The extra character is returned to the input stream. The lex field is set appropriately and the function returns.*/
				toReturn.lex=LT;
				if(c!=EOF)ungetc(c,input);
				return toReturn;
			case 36:
				/*The token here is '<='. The lex field is set to LE and the function returns.*/
				toReturn.lex=LE;
				return toReturn;
			case 37:
				if(EOF==(c=fgetc(input)))
				{
					state = 38;
				}
				/*This state is reached if a '*' has been encountered. In this case the token could either be the multiplication operator '*' or the beginning of a comment '**'. If the character further read is '*' then the token here is the beginning of a comment. In this case the next state is 39.*/ 
				else if(c=='*')
				{
					state=39;
				}
				/*Any other character signifies that the token is just the multiplication operator '*'. In this case the next state is 38.*/
				else
				{
					state=38;
				}
				break;
			case 38:
				/*The extra character is pushed into the input stream. The lex field is set to MUL and the function returns.*/
				toReturn.lex=MUL;
				if(c!=EOF)ungetc(c,input);
				return toReturn;
			case 39:
				/*This state is jsut used to ignore characters in a comment.*/
				if(EOF==(c=fgetc(input)))
				{
					toReturn.lex=END_OF_INPUT_STREAM;
					return toReturn;
				}
				/*If a '*' is read, it could just be a single '*' or the first character in the end of comment '**'. This possibility is reflected in state 40.*/
				else if(c=='*')
				{
					state=40;
				}
				else if(c=='\n')
				{
					*line = *line +1;
					state = 39;
				}
				/*Any other character must be a part of the comment itself and is just absorbed into this state.*/
				else
				{
					state=39;
				}
				break;
			case 40:
				if(EOF==(c=fgetc(input)))
				{
					toReturn.lex=END_OF_INPUT_STREAM;
					return toReturn;
				}
				/*If the next character is also a '*' the end of the comment has been reached. In this case the next state is 41.*/
				else if(c=='*')
				{
					state=41;
				}
				else if(c=='\n')
				{
					*line = *line+1;
					state = 39;
				}
				/*Any other character indicates that the previous character was just a stray '*'. In this case the lexer must again go back to the previous state 39.*/
				else
				{
					state=39;
				}
				break;
			case 41:
				/*In this case the lex field is set to COMMENT just to indicate that a comment was read.*/
				//toReturn.lex=COMMENT;
				toReturn.lex=EMPTY_TOKEN;
				toReturn.length=0;
				toReturn.value.integer=0;
				toReturn.error=false;
				state = 1;
				break;
				//return toReturn;
			case 42:
				/*If the character encountered is '[', return the SQBO lexeme.*/
				toReturn.lex=SQBO;
				return toReturn;
			case 43:
				/*If the character encountered is ']', return the SQBC lexeme.*/
				toReturn.lex=SQBC;
				return toReturn;
			case 44:
				/*If the character encountered is ';', return the SEMICOL lexeme.*/
				toReturn.lex=SEMICOL;
				return toReturn;
			case 45:
				/*If the character encountered is '(', return the BO lexeme.*/
				toReturn.lex=BO;
				return toReturn;
			case 46:
				/*If the character encountered is ')', return the BC lexeme.*/
				toReturn.lex=BC;
				return toReturn;
			case 47:
				//check if this is right.
				toReturn.lex=DRIVERDEF;
				return toReturn;
			case 48:
				toReturn.lex=DEF;
				if(c!=EOF)ungetc(c,input);
				return toReturn;
			case 49:
				toReturn.lex=DRIVERENDDEF;
				return toReturn;
			case 50:
				toReturn.lex=ENDDEF;
				if(c!=EOF)ungetc(c,input);
				return toReturn;
			default:
				printf("This code should never be executed.");	
		}
	}
}



//Invokes the lexer and the corresponding lexemes and tokens are printed on the console.
int runLexer(char lexpath[]){
	FILE* input = fopen(lexpath, "r");
    int line=1;
	bool DDot=false;
	Token t;
	while((t=Lexer(input, &line, &DDot)).lex!=END_OF_INPUT_STREAM)
	{
		if(t.error==true)
		{
			if(t.lex==ID&&(t.value.identifier[0]=='_'||isalnum(t.value.identifier[0])))
			{
				printf("Line %d: Lexical error, Length of identifier exceeded.\n",(t.length));
			}
			else{
				printf("Line %d: Lexical error, Unexpected character '%c' while parsing a %s\n", (t.length), t.value.identifier[0], LexemeToString(t.lex));
			}
		}
		else if(t.lex==KEYWORD)
			printf("%-10d%-20s%-20s\n",line, LexemeToString(t.lex),KeywordToString(t.value.key));
		else if(t.lex==ID)
			printf("%-10d%-20s%-20s\n",line,LexemeToString(t.lex),t.value.identifier);
		else if(t.lex==NUM)
			printf("%-10d%-20s%-20lld\n",line,LexemeToString(t.lex),t.value.integer);
		else if(t.lex==RNUM)
			printf("%-10d%-20s%-20f\n",line,LexemeToString(t.lex),t.value.real);
		else
			printf("%-10d%-20s%-20s\n",line,"Symbol:",LexemeToString(t.lex));
	}
}
