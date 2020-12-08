// Group Number - 25
// Name : Aayush Atul Verma         ID : 2017A7PS0061P
// Name : Aditya Ramaswamy          ID : 2017A7PS0130P
// Name : Jithin Kallukalam Sojan   ID : 2017A7PS0163P

#ifndef CODEGENDEF_H
#define CODEGENDEF_H

#include "astDef.h"

typedef enum Opcode{ONULLOP,CREATE,MOVQ,MOVD,MOVW,OSUB,RET,ODRIVER,
OADD,OFADD,OFSUB,OMUL,OFMUL,OSUBQ,
JNEQ,ODIV,OFDIV,PRINTERROR1,PRINTERROR2,PRINTERROR3,PRINTERROR4,
JMP,OLT,OFLT,OLE,OFLE,OGT,OFGT,OGE,OFGE,OEQ,
OFEQ,ONE,OFNE,OAND,OOR,JGE,JLE,JG,JL,JNEQW,
JEQ,CALL,OPRINT,OGETVALUE,EXIT,DRIVERRET,ZEROERROR
}Opcode;

typedef enum reg{RBP,RSP,EAX,RCX,RDX,EDX}reg;

typedef struct IrCode{
    short label;
    //Incase there is a Label before this instruction.

    Opcode Op;
    //Enumerate something for PRINTERROR

    //So that we know what to print.

    int tag1;
    //1 for astNode, 3 for reg, 2 fro num, 4 for [ESP](offset will be the offset), 5 for [EBP](offset will be offset), 6 for an integer
    union op1{
        struct astNode* a;
        reg R;
        int num;
        //It is mentioned that the size of num is 2.
        double real;
        //It is mentioned that the size of real is  4.
        bool boolVal;

        int offset;
    }op1;

    int tag2;
    union op2{
        struct astNode* a;
        reg R;
        int num;
        double real;
        bool boolVal;
        int offset;
    }op2;

    int tag3;
    union op3{
        struct astNode* a;
        reg R;
        int num;
        double real;
        bool boolVal;
        int offset;
    }op3;

    short jump;
    //Basically the label of the next instruction if necessary. Like if a == b goto L5

    struct IrCode* next;
    //The next piece of IrCode.

} IrCode;

#endif