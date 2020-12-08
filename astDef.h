// Group Number - 25
// Name : Aayush Atul Verma         ID : 2017A7PS0061P
// Name : Aditya Ramaswamy          ID : 2017A7PS0130P
// Name : Jithin Kallukalam Sojan   ID : 2017A7PS0163P

#ifndef ASTDEF_H
#define ASTDEF_H
#include "hashTableDef.h"
//#include "parser.h"
//#include "parserDef.h"
#include "lexerDef.h"

typedef enum Label{_PROGRAM,_MODULEDECLARATIONS,_DRIVERMODULE,_MODID,_MODULE,_INPUT_PLIST,_OUTPUT_PLIST,_ID,_INTEGER,_REAL,_BOOLEAN,_ARRAY,_STATEMENTS,_DECLARESTMT,_CONDITIONALSTMT,_GET_VALUE,_PRINT,_TRUE,_FALSE,_NUM,_RNUM,_ASSIGNOP,_MODULEREUSESTMT,_U,_PLUS,_MINUS,_MUL,_DIV,_AND,_OR,_LT,_LE,_GT,_GE,_EQ,_NE,_DEFAULT,_FOR,_WHILE,_IDLIST}Label;

typedef struct astNode{
    Label tag;     
    Token* t;
    short True;
    short False;
    struct hashEntry* entry;   
    bool error;
    short scope[2];
    struct astNode* Parent;
    struct astNode* First;
    struct astNode* Right; 
} astNode;

//struct hashTable* table;
//We can just pass the current hashTable around.

//entry corresponds to its hashTable entry.
//further represents whether we can proceed with the next stages in semantic analysis. 
#endif