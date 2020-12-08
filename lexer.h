// Group Number - 25
// Name : Aayush Atul Verma         ID : 2017A7PS0061P
// Name : Aditya Ramaswamy          ID : 2017A7PS0130P
// Name : Jithin Kallukalam Sojan   ID : 2017A7PS0163P

#ifndef LEXER_H
#define LEXER_H
#include "lexerDef.h"

int Lookup(Token *);
Token Lexer(FILE *, int *, bool *);
int runLexer(char[]);
#endif