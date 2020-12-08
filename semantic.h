// Group Number - 25
// Name : Aayush Atul Verma         ID : 2017A7PS0061P
// Name : Aditya Ramaswamy          ID : 2017A7PS0130P
// Name : Jithin Kallukalam Sojan   ID : 2017A7PS0163P

#ifndef SEMANTIC_H
#define SEMANTIC_H
#include "typeDef.h"

void semanticCheck(astNode* Root, hashTable* st, int* error);
void modOpCheck(astNode* node, hashTable* st, int* error);
void stmtsCheck(astNode* node, hashTable* st, int* error);
void offsetComp(astNode* node);
void printARSize(hashTable* st);
void printST(hashTable* st);
void printScopeVar(hashTable* st,int nestLevel);
void printArrFeatures(hashTable* st);
void printArrRec(hashTable *st);

#endif