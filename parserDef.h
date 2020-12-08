// Group Number - 25
// Name : Aayush Atul Verma         ID : 2017A7PS0061P
// Name : Aditya Ramaswamy          ID : 2017A7PS0130P
// Name : Jithin Kallukalam Sojan   ID : 2017A7PS0163P

#ifndef PARSERDEF_H
#define PARSERDEF_H
//#include "lexer.h"
#include "lexerDef.h"
#include "astDef.h"

#define NO_OF_NT 57
#define NO_OF_T 56

typedef enum Non_Terminal{NTPROGRAM, NTMODULEDECLARATIONS, NTMODULEDECLARATION, NTOTHERMODULES, NTDRIVERMODULE, NTMODULE, NTRET, NTINPUT_PLIST, NTN1, NTOUTPUT_PLIST, NTN2, NTDATATYPE, NTRANGE_ARRAYS, NTTYPE, NTMODULEDEF, NTSTATEMENTS, NTSTATEMENT, NTIOSTMT, NTBOOLCONSTT, NTVAR_ID_NUM, NTVAR, NTWHICHID, NTSIMPLESTMT, NTASSIGNMENTSTMT, NTWHICHSTMT, NTLVALUEIDSTMT, NTLVALUEARRSTMT, NTINDEX, NTMODULEREUSESTMT, NTOPTIONAL, NTIDLIST, NTN3, NTEXPRESSION, NTU, NTNEW_NT, NTUNARY_OP, NTARITHMETICORBOOLEANEXPR, NTN7, NTANYTERM, NTN8, NTARITHMETICEXPR, NTN4, NTTERM, NTN5, NTFACTOR, NTOP1, NTOP2, NTLOGICALOP, NTRELATIONALOP, NTDECLARESTMT, NTCONDITIONALSTMT, NTCASESTMTS, NTN9, NTVALUE, NTDEFAULT, NTITERATIVESTMT, NTRANGE} Non_Terminal;

typedef enum Terminal{TDECLARE, TMODULE, TID, TSEMICOL, TDRIVERDEF, TDRIVER, TPROGRAM, TDRIVERENDDEF, TDEF, TENDDEF, TTAKES, TINPUT, TSQBO, TSQBC, TRETURNS, TCOLON, TCOMMA, TARRAY, TOF, TRANGEOP, TINTEGER, TREAL, TBOOLEAN, TSTART, TEND, TGET_VALUE, TBO, TBC, TPRINT, TTRUE, TFALSE, TNUM, TRNUM, TASSIGNOP, TUSE, TWITH, TPARAMETERS, TPLUS, TMINUS, TMUL, TDIV, TAND, TOR, TLT, TLE, TGT, TGE, TEQ, TNE, TSWITCH, TCASE, TBREAK, TDEFAULT, TFOR, TIN, TWHILE } Terminal;

// Structure of a cell.
typedef struct Cell{
    int tag;    // 1, 2, 3, 4, 5 for NT, T, OR, ε resepctively.
    struct Cell *next;
    
    union Value1{
        Terminal T;
        Non_Terminal NT;
    }value;
}Cell;

// Structure for the grammar that is read from a file : Pointer to an array of cells.
typedef Cell* Grammar[NO_OF_NT];

// Structure for First and Follow Sets
typedef struct{
    unsigned long long int First[NO_OF_NT];
    unsigned long long int Follow[NO_OF_NT];
}FirstAndFollow;


typedef struct ParseNode{
    Token* t;
    Cell* c;
    short rule1;
    short rule2;
    struct astNode* Node;
    struct astNode* inh;
    struct ParseNode* Parent;
    struct ParseNode* First;
    struct ParseNode* Right;
}ParseNode;

typedef Cell* ParseTable[NO_OF_NT][NO_OF_T+1];
typedef int RuleNum[NO_OF_NT][NO_OF_T+1];
//The P[x][NO_OF_T] will represent the terminal symbol.

#endif