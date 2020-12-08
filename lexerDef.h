// Group Number - 25
// Name : Aayush Atul Verma         ID : 2017A7PS0061P
// Name : Aditya Ramaswamy          ID : 2017A7PS0130P
// Name : Jithin Kallukalam Sojan   ID : 2017A7PS0163P
#ifndef LEXERDEF_H
#define LEXERDEF_H
#include<stdbool.h>
#include <stdio.h>

typedef enum {INTEGER, REAL, BOOLEAN, OF, ARRAY, START, END, DECLARE, MODULE, DRIVER, PROGRAM, RECORD, TAGGED, UNION, GET_VALUE, PRINT, USE, WITH, PARAMETERS, TRUE, FALSE, TAKES, INPUT, RETURNS, AND, OR, FOR, IN, SWITCH, CASE, BREAK,DEFAULT, WHILE} Keyword;		//A list of all keywords in the language. Implemented here as enumerated constants.
typedef enum {EMPTY_TOKEN, PLUS, MINUS, MUL, DIV, LT, LE, GE, GT, EQ, NE, DEF, ENDDEF, COLON, RANGEOP, SEMICOL, COMMA, ASSIGNOP, SQBO, SQBC, BO, BC, ID, NUM, RNUM, KEYWORD, END_OF_INPUT_STREAM, COMMENT, DRIVERDEF, DRIVERENDDEF} Lexeme;		/*A list of possible tokens, and some values of use to communicate with the parser. EMPTY_TOKEN is used if there is an extraneous character in the input stream encountered while not parsing any other token. KEYWORD is used to indiacate that a key word was actually encountered insted of clubbing keywords along with other lexemes. END_OF_INPUT_STREAM indiactes that there are no characters left to read. COMMENT indiactes to the parser that a comment was just parsed.*/


/*Since the following declarations make use of bool values they can only be accessed if stdbool.h has been included.*/

typedef struct{
	Lexeme lex;
	/*Stores the lexeme read during normal operation. In case of an error it stores the lexeme that was being parsed. It also indicates the end of the input stream when it occurs.*/
	short length;
	/*This variable serves multiple purposes, mostly all internal. While parsing a floating point value it initially stores the position in the decimal portion where the next digit is to be inserted. If the float is expressed in exponent notation it subsequently stores the value of the exponent in the exponent notation. When used with either a keyword or an identifier it indicates the length of the string stored in identifier[]. If it is an error the line number is returned in this variable.*/
	short line_no;
	//This is for storing the line number that has to be printed after creating the parse tree.
	bool error;
	//Simply indicates if an error was encountered.
	union Value{
		/*The following variables cannot ever be of use together. This is why they are stored in a union.*/
		Keyword key;
		/*Stores the value of the encountered Keyword if lex=KEYWORD.*/
		char identifier[21];
		/*Stores the name of the identifier if lex=ID, during normal operation. In case of an error, the first character stores the extraneous character encountered.*/
		long long int integer;
		/*If the lexeme is a NUM its value is stored here.*/
		double real;
		/*If the lexeme is an RNUM its value is stored here.*/
	} value;
} Token;
#endif