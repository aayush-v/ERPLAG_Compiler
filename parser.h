// Group Number - 25
// Name : Aayush Atul Verma         ID : 2017A7PS0061P
// Name : Aditya Ramaswamy          ID : 2017A7PS0130P
// Name : Jithin Kallukalam Sojan   ID : 2017A7PS0163P

#ifndef PARSER_H
#define PARSER_H
#include "parserDef.h"

Grammar* createGrammar();
void freeCells(ParseTable P);
void ComputeFirstAndFollowSets (Grammar* G, FirstAndFollow* F);
void ComputeFirst(Grammar* G, FirstAndFollow* F, Non_Terminal NT);
void ComputeFollow(Grammar* G, unsigned long long int temp[], FirstAndFollow* F);
unsigned long long int ComputeFirstSet(FirstAndFollow* F, Cell* C);
void createParseTable(Grammar* G, FirstAndFollow* F, ParseTable P,RuleNum R);
void PrintFirstAndFollow(FirstAndFollow* F);
ParseNode* parseInputSourceCode(char* testcaseFile, ParseTable P, FirstAndFollow* F, RuleNum R, int*);
void printParseTree(ParseNode* P, int* parseTreeCount,int flag);
void inorder(ParseNode* P,int* parseTreeCount,int flag);
ParseNode* creatGramAndParse(char path1[], char path2[], int flag, Grammar* G, int* error, int* parseTreeCount);
#endif