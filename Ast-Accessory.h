// Group Number - 25
// Name : Aayush Atul Verma         ID : 2017A7PS0061P
// Name : Aditya Ramaswamy          ID : 2017A7PS0130P
// Name : Jithin Kallukalam Sojan   ID : 2017A7PS0163P

#ifndef AST_ACCESSORY_H
#define AST_ACCESSORY_H

#include "astDef.h"

char* NodeToString(Label lab);
void PrintASTTree(astNode* root, int* ASTTreeCount,int flag);
void ASTinorder(astNode* root, int* ASTTreeCount,int flag);


#endif