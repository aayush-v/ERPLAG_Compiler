// Group Number - 25
// Name : Aayush Atul Verma         ID : 2017A7PS0061P
// Name : Aditya Ramaswamy          ID : 2017A7PS0130P
// Name : Jithin Kallukalam Sojan   ID : 2017A7PS0163P

#ifndef TYPE_H
#define TYPE_H
#include "typeDef.h"

struct hashTable* typeStart(struct astNode* Root, int* error);
void Traverse(struct astNode* node, int* error, struct hashTable* st);
void modDecl(struct astNode* node, int* error, struct hashTable* st);
void modDef(struct astNode* node, int* error, struct hashTable* st);
void driverDef(struct astNode* node, int* error, struct hashTable* st);
void insertModIO(struct hashTable* st,struct astNode* node,int* error);
void addVar(struct hashTable* st,struct astNode* node, int* error, int option);
void addArray(struct hashTable* st,struct astNode* node, int* error, int option);
int typeExtract(struct hashTable* st, struct astNode* node, int* error, int option);
int expressionCheck(struct hashTable* st, struct astNode* node, int* error, int option, hashEntry* extra);
int cmpVal(struct astNode* a,struct astNode* b);
int getType(struct astNode* node);
int typeCmp(struct astNode* a,struct astNode*b);
int declCheck(struct hashTable* st, struct astNode* node, int* error);
char* printId(struct astNode* anode);
char* printOp(struct astNode* node);
char* printType(struct astNode* node);
void statements(struct hashTable* st, struct astNode* node, int* error);

#endif