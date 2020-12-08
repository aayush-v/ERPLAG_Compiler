// Group Number - 25
// Name : Aayush Atul Verma         ID : 2017A7PS0061P
// Name : Aditya Ramaswamy          ID : 2017A7PS0130P
// Name : Jithin Kallukalam Sojan   ID : 2017A7PS0163P

#ifndef TYPEDEF_H
#define TYPEDEF_H
#include "hashtable.h"
#include "astDef.h"

#define SIZE_OF_REAL 8
#define SIZE_OF_INTEGER 4
#define SIZE_OF_BOOLEAN 2
#define SIZE_OF_POINTER 8
#define SIZE_AFTER_BP 24

typedef struct whileCheck{
    struct hashEntry* entry;
    struct astNode* index;    
    struct whileCheck* next;
}whileCheck;

#endif