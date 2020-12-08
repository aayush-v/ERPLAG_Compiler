// Group Number - 25
// Name : Aayush Atul Verma         ID : 2017A7PS0061P
// Name : Aditya Ramaswamy          ID : 2017A7PS0130P
// Name : Jithin Kallukalam Sojan   ID : 2017A7PS0163P

#ifndef HASHTABLE_H
#define HASHTABLE_H
#include "hashTableDef.h"
//#include "astDef.h"

struct astNode;

unsigned long int hashFunc(char*,int);
struct hashTable* initHashTable();
int insert(struct hashTable*,char*,struct astNode*);
void freeHashTable(struct hashTable*);
int retrieve(struct hashTable*,char*);
#endif