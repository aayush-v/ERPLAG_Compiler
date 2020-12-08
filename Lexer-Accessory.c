// Group Number - 25
// Name : Aayush Atul Verma         ID : 2017A7PS0061P
// Name : Aditya Ramaswamy          ID : 2017A7PS0130P
// Name : Jithin Kallukalam Sojan   ID : 2017A7PS0163P


#include "lexer.h"

/*Returns a string representation of a lexeme.*/
char* LexemeToString(Lexeme lex)
{
	switch(lex)
	{
		case PLUS: return "+";
		case EMPTY_TOKEN: return "Empty token";
		case MINUS: return "-";
		case MUL: return "*";
		case DIV: return "/";
		case LT: return "<";
		case LE: return "<=";
		case GE: return ">=";
		case GT: return ">";
		case EQ: return "==";
		case NE: return "!=";
		case DEF: return "Function Definition";
		case ENDDEF: return "Function Definition End";
		case COLON: return ":";
		case RANGEOP: return "..";
		case SEMICOL: return ";";
		case COMMA: return ",";
		case ASSIGNOP: return ":=";
		case SQBO: return "[";
		case SQBC: return "]";
		case BO: return "(";
		case BC: return ")";
		case ID: return "Identifier:";
		case NUM: return "Integer:";
		case RNUM: return "Float:";
		case KEYWORD: return "Keyword: ";
		case END_OF_INPUT_STREAM: return "End of input stream";
		case COMMENT: return "Comment";
		case DRIVERDEF: return "Driver Definition";
		case DRIVERENDDEF: return "Driver Definition End";
		default: return "Unknown value";
	}
}

/*Returns a string representation of a keyword.*/
char* KeywordToString(Keyword key)
{
	switch(key)
	{
		case INTEGER: return "Integer";
		case REAL: return "Real";
		case BOOLEAN: return "Boolean";
		case OF: return "Of";
		case ARRAY: return "Array";
		case START: return "Start";
		case END: return "End";
		case DECLARE: return "Declare";
		case MODULE: return "Module";
		case DRIVER: return "Driver";
		case PROGRAM: return "Program";
		case RECORD: return "Record";
		case TAGGED: return "Tagged";
		case UNION: return "Union";
		case GET_VALUE: return "Get_value";
		case PRINT: return "Print";
		case USE: return "Use";
		case WITH: return "With";
		case PARAMETERS: return "Parameters";
		case TRUE: return "True";
		case FALSE: return "False";
		case TAKES: return "Takes";
		case INPUT: return "Input";
		case RETURNS: return "Returns";
		case AND: return "And";
		case OR: return "Or";
		case FOR: return "For";
		case IN: return "In";
		case SWITCH: return "Switch";
		case CASE: return "Case";
		case BREAK: return "Break";
		case DEFAULT: return "Default";
		case WHILE: return "While";
		default: return "Unknown";
	}
}