// Group Number - 25
// Name : Aayush Atul Verma         ID : 2017A7PS0061P
// Name : Aditya Ramaswamy          ID : 2017A7PS0130P
// Name : Jithin Kallukalam Sojan   ID : 2017A7PS0163P

#ifndef CODEGEN_H
#define CODEGEN_H

#include "codeGenDef.h"

IrCode* codeTree(struct astNode* Root);
void TravModLabel(struct astNode* node,short int* label);
void labelStmts(struct astNode* node, short int* label);
IrCode* createInst();
IrCode* addInstr(IrCode* a,IrCode* b);
IrCode* modGen(astNode* node,short int* label);
IrCode* driverGen(astNode* node, short int* label);
IrCode* expressionCreate(astNode* node, short int* label);
IrCode* dynamicCheck(astNode* node, short int* label);
IrCode* assignment(astNode* node, short int* label);
IrCode* declArray(astNode* node, short int* label);
IrCode* forStmt(astNode* node, short int* label);
IrCode* whileStmt(astNode* node, short int* label);
IrCode* caseStmts(astNode* node,short int* label);
IrCode* dynamicArrayCheck(astNode* node,short int* label);
IrCode* modReuseStmt(astNode* node, short int* label);
int sizeIDList(astNode* node);
int sizeParamList(astNode* node);
IrCode* stmtsGen(astNode* node, short int* label);
IrCode* programGen(astNode* node,short int* label);

#endif