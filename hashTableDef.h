// Group Number - 25
// Name : Aayush Atul Verma         ID : 2017A7PS0061P
// Name : Aditya Ramaswamy          ID : 2017A7PS0130P
// Name : Jithin Kallukalam Sojan   ID : 2017A7PS0163P

#ifndef HASHTABLEDEF_H
#define HASHTABLEDEF_H
//#include<stdbool.h>
#include "astDef.h"
#include "parserDef.h"
#include "lexerDef.h"
#include "typeDef.h"

#define TABLESIZE 2000

typedef struct hashEntry{
    char id[22];
    //To account for input and output parameters.
    struct astNode* type;
    struct astNode* extra;
    //For internal usage. Basically figuring out wether a varaible in the while loop condition is assigned a value.Also keeps a for loop's scope.
    bool isStatic;
    bool use;
    bool forBlock;
    bool io;
    //bool temp;
    //1 when in the scope of the for loop, 0 otherwise.
    bool whileBlock;
    //1 when in the scope of the while loop, 0 otherwise.
    bool whileUse;
    bool whileHas;
    //1 when in the scope of the for loop, 0 otherwise.
    struct whileCheck* check;
    int offset;
    int width;
    int locOffset;
    struct hashEntry* next;
    struct hashTable* partOf;
    struct hashTable* derive; 
}hashEntry;

typedef struct hashTable{
    short scope[2];
    //bool currentUse;
    int size;
    int ioSize;
    //int initSize;
    int locSize;
    int locIoSize;
    int nextTemp;
    struct hashEntry* first;
    struct hashEntry* current;
    struct hashEntry* parentEntry;
    struct hashEntry* Table[TABLESIZE];
}hashTable;

#endif