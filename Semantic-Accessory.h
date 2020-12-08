// Group Number - 25
// Name : Aayush Atul Verma         ID : 2017A7PS0061P
// Name : Aditya Ramaswamy          ID : 2017A7PS0130P
// Name : Jithin Kallukalam Sojan   ID : 2017A7PS0163P

#ifndef SEMANTIC_ACC_H
#define SEMANTIC_ACC_H

#include "typeDef.h"

int cmpVal(astNode* a,astNode* b);
int getType(astNode* node);
int typeCmp(astNode* a,astNode* b);
char* printId(astNode* node);
char* printOp(astNode* node);
char* printType(astNode* node);

#endif