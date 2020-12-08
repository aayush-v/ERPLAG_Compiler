// Group Number - 25
// Name : Aayush Atul Verma         ID : 2017A7PS0061P
// Name : Aditya Ramaswamy          ID : 2017A7PS0130P
// Name : Jithin Kallukalam Sojan   ID : 2017A7PS0163P

#ifndef ASM_GEN_H
#define ASM_GEN_H

#include "codeGenDef.h"
#include "astDef.h"

void asmCreate(IrCode* node,char path[]);
void genOperands(IrCode* code,int op,int size,int reg,int dir,FILE* f1,int option);

#endif