// Group Number - 25
// Name : Aayush Atul Verma         ID : 2017A7PS0061P
// Name : Aditya Ramaswamy          ID : 2017A7PS0130P
// Name : Jithin Kallukalam Sojan   ID : 2017A7PS0163P

#ifndef AST_H
#define AST_H
#include "astDef.h"

astNode* createAST(ParseNode* Root);
astNode* createNode(Label l,Token* t);
void PostOrder(ParseNode* Root);
void firstTrav(ParseNode* node);
void addChild(astNode* curr,astNode* child);
astNode* addSibling(astNode* curr,astNode* sib);
void freeList(ParseNode* list);
void secondTrav(ParseNode* node);

#endif