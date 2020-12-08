// Group Number - 25
// Name : Aayush Atul Verma         ID : 2017A7PS0061P
// Name : Aditya Ramaswamy          ID : 2017A7PS0130P
// Name : Jithin Kallukalam Sojan   ID : 2017A7PS0163P

#include "parser.h"
#include<stdlib.h>
#include<string.h>

/*Returns a Terminal for the given string input*/
Terminal stringToTerminal(char* str){
    if(strcmp(str,"DECLARE")==0) return TDECLARE;
    else if(strcmp(str,"MODULE")==0)return TMODULE;
    else if(strcmp(str,"ID")==0)return TID;
    else if(strcmp(str,"SEMICOL")==0)return TSEMICOL;
    else if(strcmp(str,"DRIVERDEF")==0)return TDRIVERDEF;
    else if(strcmp(str,"DRIVER")==0)return TDRIVER;
    else if(strcmp(str,"PROGRAM")==0)return TPROGRAM;
    else if(strcmp(str,"DRIVERENDDEF")==0)return TDRIVERENDDEF;
    else if(strcmp(str,"DEF")==0)return TDEF;
    else if(strcmp(str,"ENDDEF")==0)return TENDDEF;
    else if(strcmp(str,"TAKES")==0)return TTAKES;
    else if(strcmp(str,"INPUT")==0)return TINPUT;
    else if(strcmp(str,"SQBO")==0)return TSQBO;
    else if(strcmp(str,"SQBC")==0)return TSQBC;
    else if(strcmp(str,"RETURNS")==0)return TRETURNS;
    else if(strcmp(str,"COLON")==0)return TCOLON;
    else if(strcmp(str,"COMMA")==0)return TCOMMA;
    else if(strcmp(str,"ARRAY")==0)return TARRAY;
    else if(strcmp(str,"OF")==0)return TOF;
    else if(strcmp(str,"RANGEOP")==0)return TRANGEOP;
    else if(strcmp(str,"INTEGER")==0)return TINTEGER;
    else if(strcmp(str,"REAL")==0)return TREAL;
    else if(strcmp(str,"BOOLEAN")==0)return TBOOLEAN;
    else if(strcmp(str,"START")==0)return TSTART;
    else if(strcmp(str,"END")==0)return TEND;
    else if(strcmp(str,"GET_VALUE")==0)return TGET_VALUE;
    else if(strcmp(str,"BO")==0)return TBO;
    else if(strcmp(str,"BC")==0)return TBC;
    else if(strcmp(str,"PRINT")==0)return TPRINT;
    else if(strcmp(str,"TRUE")==0)return TTRUE;
    else if(strcmp(str,"FALSE")==0)return TFALSE;
    else if(strcmp(str,"NUM")==0)return TNUM;
    else if(strcmp(str,"RNUM")==0)return TRNUM;
    else if(strcmp(str,"ASSIGNOP")==0)return TASSIGNOP;
    else if(strcmp(str,"USE")==0)return TUSE;
    else if(strcmp(str,"WITH")==0)return TWITH;
    else if(strcmp(str,"PARAMETERS")==0)return TPARAMETERS;
    else if(strcmp(str,"PLUS")==0)return TPLUS;
    else if(strcmp(str,"MINUS")==0)return TMINUS;
    else if(strcmp(str,"MUL")==0)return TMUL;
    else if(strcmp(str,"DIV")==0)return TDIV;
    else if(strcmp(str,"AND")==0)return TAND;
    else if(strcmp(str,"OR")==0)return TOR;
    else if(strcmp(str,"LT")==0)return TLT;
    else if(strcmp(str,"LE")==0)return TLE;
    else if(strcmp(str,"GT")==0)return TGT;
    else if(strcmp(str,"GE")==0)return TGE;
    else if(strcmp(str,"EQ")==0)return TEQ;
    else if(strcmp(str,"NE")==0)return TNE;
    else if(strcmp(str,"SWITCH")==0)return TSWITCH;
    else if(strcmp(str,"CASE")==0)return TCASE;
    else if(strcmp(str,"BREAK")==0)return TBREAK;
    else if(strcmp(str,"DEFAULT")==0)return TDEFAULT;
    else if(strcmp(str,"FOR")==0)return TFOR;
    else if(strcmp(str,"IN")==0)return TIN;
    else if(strcmp(str,"WHILE")==0)return TWHILE;  
}

/*Returns a Non Terminal for the given string input*/
Non_Terminal stringToNonTerminal(char* str){
    if(strcmp(str,"program")==0)return NTPROGRAM;
    else if(strcmp(str,"moduleDeclarations")==0)return NTMODULEDECLARATIONS;
    else if(strcmp(str,"moduleDeclaration")==0)return NTMODULEDECLARATION;
    else if(strcmp(str,"otherModules")==0)return NTOTHERMODULES;
    else if(strcmp(str,"driverModule")==0)return NTDRIVERMODULE;
    else if(strcmp(str,"module")==0)return NTMODULE;
    else if(strcmp(str,"ret")==0)return NTRET;
    else if(strcmp(str,"input_plist")==0)return NTINPUT_PLIST;
    else if(strcmp(str,"N1")==0)return NTN1;
    else if(strcmp(str,"output_plist")==0)return NTOUTPUT_PLIST;
    else if(strcmp(str,"N2")==0)return NTN2;
    else if(strcmp(str,"dataType")==0)return NTDATATYPE;
    else if(strcmp(str,"range_arrays")==0)return NTRANGE_ARRAYS;
    else if(strcmp(str,"type")==0)return NTTYPE;
    else if(strcmp(str,"moduleDef")==0)return NTMODULEDEF;
    else if(strcmp(str,"statements")==0)return NTSTATEMENTS;
    else if(strcmp(str,"statement")==0)return NTSTATEMENT;
    else if(strcmp(str,"ioStmt")==0)return NTIOSTMT;
    else if(strcmp(str,"boolConstt")==0)return NTBOOLCONSTT;
    else if(strcmp(str,"var_id_num")==0)return NTVAR_ID_NUM;
    else if(strcmp(str,"var")==0)return NTVAR;
    else if(strcmp(str,"whichId")==0)return NTWHICHID;
    else if(strcmp(str,"simpleStmt")==0)return NTSIMPLESTMT;
    else if(strcmp(str,"assignmentStmt")==0)return NTASSIGNMENTSTMT;
    else if(strcmp(str,"whichStmt")==0)return NTWHICHSTMT;
    else if(strcmp(str,"lvalueIDStmt")==0)return NTLVALUEIDSTMT;
    else if(strcmp(str,"lvalueARRStmt")==0)return NTLVALUEARRSTMT;
    else if(strcmp(str,"index")==0)return NTINDEX;
    else if(strcmp(str,"moduleReuseStmt")==0)return NTMODULEREUSESTMT;
    else if(strcmp(str,"optional")==0)return NTOPTIONAL;
    else if(strcmp(str,"idList")==0)return NTIDLIST;
    else if(strcmp(str,"N3")==0)return NTN3;
    else if(strcmp(str,"expression")==0)return NTEXPRESSION;
    else if(strcmp(str,"U")==0)return NTU;
    else if(strcmp(str,"new_NT")==0)return NTNEW_NT;
    else if(strcmp(str,"unary_op")==0)return NTUNARY_OP;
    else if(strcmp(str,"arithmeticOrBooleanExpr")==0)return NTARITHMETICORBOOLEANEXPR;
    else if(strcmp(str,"N7")==0)return NTN7;
    else if(strcmp(str,"AnyTerm")==0)return NTANYTERM;
    else if(strcmp(str,"N8")==0)return NTN8;
    else if(strcmp(str,"arithmeticExpr")==0)return NTARITHMETICEXPR;
    else if(strcmp(str,"N4")==0)return NTN4;
    else if(strcmp(str,"term")==0)return NTTERM;
    else if(strcmp(str,"N5")==0)return NTN5;
    else if(strcmp(str,"factor")==0)return NTFACTOR;
    else if(strcmp(str,"op1")==0)return NTOP1;
    else if(strcmp(str,"op2")==0)return NTOP2;
    else if(strcmp(str,"logicalOp")==0)return NTLOGICALOP;
    else if(strcmp(str,"relationalOp")==0)return NTRELATIONALOP;
    else if(strcmp(str,"declareStmt")==0)return NTDECLARESTMT;
    else if(strcmp(str,"conditionalStmt")==0)return NTCONDITIONALSTMT;
    else if(strcmp(str,"caseStmts")==0)return NTCASESTMTS;
    else if(strcmp(str,"N9")==0)return NTN9;
    else if(strcmp(str,"value")==0)return NTVALUE;
    else if(strcmp(str,"default")==0)return NTDEFAULT;
    else if(strcmp(str,"iterativeStmt")==0)return NTITERATIVESTMT;
    else if(strcmp(str,"range")==0)return NTRANGE;
}

/*Returns a string for the correspondig Non_Terminal(enum value)*/
char* NonTerminalToString(Non_Terminal NT){
    switch (NT)
    {
    case NTPROGRAM:
        return "<PROGRAM>";
        
    case NTMODULEDECLARATIONS:
        return "<MODULEDECLARATIONS>";
           
    case NTMODULEDECLARATION:
        return "<MODULEDECLARATION>";
            
    case NTOTHERMODULES:
        return "<OTHERMODULES>";
        
    case NTDRIVERMODULE:
        return "<DRIVERMODULE>";
        
    case NTMODULE:
        return "<MODULE>";
        
    case NTRET:
        return "<RET>";
        
    case NTINDEX:
        return "<INDEX>";
        
    case NTINPUT_PLIST:
        return "<INPUT_PLIST>";
        
    case NTN1:
        return "<N1>";
        
    case NTOUTPUT_PLIST:
        return "<OUTPUT_PLIST>";
        
    case NTN2:
        return "<N2>";
        
    case NTDATATYPE:
        return "<DATATYPE>";
        
    case NTRANGE_ARRAYS:
        return "<RANGE_ARRAYS>";
        
    case NTTYPE:
        return "<TYPE>";
        
    case NTMODULEDEF:
        return "<MODULEDEF>";
        
    case NTSTATEMENTS:
        return "<STATEMENTS>";
        
    case NTSTATEMENT:
        return "<STATEMENT>";
        
    case NTIOSTMT:
        return "<IOSTMT>";
        
    case NTBOOLCONSTT:
        return "<BOOLCONSTT>";
        
    case NTVAR_ID_NUM:
        return "<VAR_ID_NUM>";
        
    case NTVAR:
        return "<VAR>";
        
    case NTWHICHID:
        return "<WHICHID>";
        
    case NTSIMPLESTMT:
        return "<SIMPLESTMT>";
        
    case NTASSIGNMENTSTMT:
        return "<ASSIGNMENTSTMT>";
        
    case NTWHICHSTMT:
        return "<WHICHSTMT>";
        
    case NTLVALUEIDSTMT:
        return "<LVALUEIDSTMT>";
        
    case NTLVALUEARRSTMT:
        return "<LVALUEARRSTMT>";
        
    case NTMODULEREUSESTMT:
        return "<MODULEREUSESTMT>";
        
    case NTOPTIONAL:
        return "<OPTIONAL>";
        
    case NTIDLIST:
        return "<IDLIST>";
        
    case NTN3:
        return "<N3>";
        
    case NTEXPRESSION:
        return "<EXPRESSION>";
        
    case NTU:
        return "<U>";
        
    case NTNEW_NT:
        return "<NEW_NT>";
        
    case NTUNARY_OP:
        return "<UNARY_OP>";
        
    case NTARITHMETICORBOOLEANEXPR:
        return "<ARITHMETICORBOOLEANEXPR>";
        
    case NTN7:
        return "<N7>";
        
    case NTANYTERM:
        return "<ANYTERM>";
        
    case NTN8:
        return "<N8>";
        
    case NTARITHMETICEXPR:
        return "<ARITHMETICEXPR>";
        
    case NTN4:
        return "<N4>";
        
    case NTTERM:
        return "<TERM>";
        
    case NTN5:
        return "<N5>";
        
    case NTFACTOR:
        return "<FACTOR>";
        
    case NTOP1:
        return "<OP1>";
        
    case NTOP2:
        return "<OP2>";
        
    case NTLOGICALOP:
        return "<LOGICALOP>";
        
    case NTRELATIONALOP:
        return "<RELATIONALOP>";
        
    case NTDECLARESTMT:
        return "<DECLARESTMT>";
        
    case NTCONDITIONALSTMT:
        return "<CONDITIONALSTMT>";
        
    case NTCASESTMTS:
        return "<CASESTMTS>";
        
    case NTN9:
        return "<N9>";
        
    case NTVALUE:
        return "<VALUE>";
        
    case NTDEFAULT:
        return "<DEFAULT>";
        
    case NTITERATIVESTMT:
        return "<ITERATIVESTMT>";
        
    case NTRANGE:
        return "<RANGE>";
            
    default:
        // printf("NONE");
        return "Unknown";
    }
}

/*Returns a string for the correspondig Terminal(enum value)*/
char* TerminalToString(Terminal T){
    switch (T)
    {
    case TDECLARE:
        return "DECLARE";
        
    case TMODULE:
        return "MODULE";
           
    case TID:
        return "ID";
            
    case TSEMICOL:
        return "SEMICOL";
        
    case TDRIVERDEF:
        return "DRIVERDEF";
        
    case TDRIVER:
        return "DRIVER";
        
    case TPROGRAM:
        return "PROGRAM";
        
    case TDRIVERENDDEF:
        return "DRIVERENDDEF";
        
    case TDEF:
        return "DEF";
        
    case TENDDEF:
        return "ENDDEF";
        
    case TTAKES:
        return "TAKES";
        
    case TINPUT:
        return "INPUT";
        
    case TSQBO:
        return "SQBO";
        
    case TSQBC:
        return "SQBC";
        
    case TRETURNS:
        return "RETURNS";
        
    case TCOLON:
        return "COLON";
        
    case TCOMMA:
        return "COMMA";
        
    case TARRAY:
        return "ARRAY";
        
    case TOF:
        return "OF";
        
    case TRANGEOP:
        return "RANGEOP";
        
    case TINTEGER:
        return "INTGER";
        
    case TREAL:
        return "REAL";
        
    case TBOOLEAN:
        return "BOOLEAN";
        
    case TSTART:
        return "START";
        
    case TEND:
        return "END";
        
    case TGET_VALUE:
        return "GET_VALUE";
        
    case TBO:
        return "BO";
        
    case TBC:
        return "BC";
        
    case TPRINT:
        return "PRINT";
        
    case TTRUE:
        return "TRUE";
        
    case TFALSE:
        return "FALSE";
        
    case TNUM:
        return "NUM";
        
    case TRNUM:
        return "RNUM";
        
    case TASSIGNOP:
        return "ASSIGNOP";
        
    case TUSE:
        return "USE";
        
    case TWITH:
        return "WITH";
        
    case TPARAMETERS:
        return "PARAMTERS";
        
    case TPLUS:
        return "PLUS";
        
    case TMINUS:
        return "MINUS";
        
    case TMUL:
        return "MUL";
        
    case TDIV:
        return "DIV";
        
    case TAND:
        return "AND";
        
    case TOR:
        return "OR";
        
    case TLT:
        return "LT";
        
    case TLE:
        return "LE";
        
    case TGT:
        return "GT";
        
    case TGE:
        return "GE";
        
    case TEQ:
        return "EQ";
        
    case TNE:
        return "NE";
        
    case TSWITCH:
        return "SWITCH";
        
    case TCASE:
        return "CASE";
        
    case TBREAK:
        return "BREAK";
        
    case TDEFAULT:
        return "DEFAULT";
        
    case TFOR:
        return "FOR";
        
    case TIN:
        return "IN";
        
    case TWHILE:
        return "WHILE";
            
    default:
        return "Unknown";
    }
}

// Compares the passed token and the terminal and returns True accordingly.
int term_cmp(Token* Tkn, Terminal T){
    if(Tkn->lex == PLUS && T == TPLUS) 
        return 1;
    else if(Tkn->lex == MINUS && T == TMINUS) 
        return 1;
    else if(Tkn->lex == MUL && T == TMUL) 
        return 1;
    else if(Tkn->lex == DIV && T == TDIV) 
        return 1;
    else if(Tkn->lex == LT && T == TLT) 
        return 1;
    else if(Tkn->lex == LE && T == TLE) 
        return 1;
    else if(Tkn->lex == GE && T == TGE) 
        return 1;
    else if(Tkn->lex == GT && T == TGT) 
        return 1;
    else if(Tkn->lex == EQ && T == TEQ) 
        return 1;
    else if(Tkn->lex == NE && T == TNE) 
        return 1;
    else if(Tkn->lex == DEF && T == TDEF) 
        return 1;
    else if(Tkn->lex == ENDDEF && T == TENDDEF) 
        return 1;
    else if(Tkn->lex == COLON && T == TCOLON) 
        return 1;
    else if(Tkn->lex == RANGEOP && T == TRANGEOP) 
        return 1;
    else if(Tkn->lex == SEMICOL && T == TSEMICOL) 
        return 1;
    else if(Tkn->lex == COMMA && T == TCOMMA) 
        return 1;
    else if(Tkn->lex == ASSIGNOP && T == TASSIGNOP) 
        return 1;
    else if(Tkn->lex == SQBO && T == TSQBO) 
        return 1;
    else if(Tkn->lex == SQBC && T == TSQBC) 
        return 1;
    else if(Tkn->lex == BO && T == TBO) 
        return 1;
    else if(Tkn->lex == BC && T == TBC) 
        return 1;
    else if(Tkn->lex == ID && T == TID) 
        return 1;
    else if(Tkn->lex == NUM && T == TNUM)
        return 1;    
    else if(Tkn->lex == RNUM && T == TRNUM)
        return 1;
    else if(Tkn->lex == DRIVERDEF && T == TDRIVERDEF)
        return 1;
    else if(Tkn->lex == DRIVERENDDEF && T == TDRIVERENDDEF)
        return 1;
    else if(Tkn->lex == KEYWORD && Tkn->value.key == INTEGER && T == TINTEGER) 
        return 1;
    else if(Tkn->lex == KEYWORD && Tkn->value.key == REAL && T == TREAL) 
        return 1;
    else if(Tkn->lex == KEYWORD && Tkn->value.key == BOOLEAN && T == TBOOLEAN) 
        return 1;
    else if(Tkn->lex == KEYWORD && Tkn->value.key == OF && T == TOF) 
        return 1;
    else if(Tkn->lex == KEYWORD && Tkn->value.key == ARRAY && T == TARRAY) 
        return 1;
    else if(Tkn->lex == KEYWORD && Tkn->value.key == START && T == TSTART) 
        return 1;
    else if(Tkn->lex == KEYWORD && Tkn->value.key == END && T == TEND) 
        return 1;
    else if(Tkn->lex == KEYWORD && Tkn->value.key == DECLARE && T == TDECLARE) 
        return 1;
    else if(Tkn->lex == KEYWORD && Tkn->value.key == MODULE && T == TMODULE) 
        return 1;
    else if(Tkn->lex == KEYWORD && Tkn->value.key == DRIVER && T == TDRIVER) 
        return 1;
    else if(Tkn->lex == KEYWORD && Tkn->value.key == PROGRAM && T == TPROGRAM) 
        return 1;
    else if(Tkn->lex == KEYWORD && Tkn->value.key == GET_VALUE && T == TGET_VALUE) 
        return 1;
    else if(Tkn->lex == KEYWORD && Tkn->value.key == PRINT && T == TPRINT) 
        return 1;
    else if(Tkn->lex == KEYWORD && Tkn->value.key == USE && T == TUSE) 
        return 1;
    else if(Tkn->lex == KEYWORD && Tkn->value.key == WITH && T == TWITH) 
        return 1;
    else if(Tkn->lex == KEYWORD && Tkn->value.key == PARAMETERS && T == TPARAMETERS) 
        return 1;
    else if(Tkn->lex == KEYWORD && Tkn->value.key == TRUE && T == TTRUE) 
        return 1;
    else if(Tkn->lex == KEYWORD && Tkn->value.key == FALSE && T == TFALSE) 
        return 1;
    else if(Tkn->lex == KEYWORD && Tkn->value.key == TAKES && T == TTAKES) 
        return 1;
    else if(Tkn->lex == KEYWORD && Tkn->value.key == INPUT && T == TINPUT) 
        return 1;
    else if(Tkn->lex == KEYWORD && Tkn->value.key == RETURNS && T == TRETURNS) 
        return 1;
    else if(Tkn->lex == KEYWORD && Tkn->value.key == AND && T == TAND) 
        return 1;
    else if(Tkn->lex == KEYWORD && Tkn->value.key == OR && T == TOR) 
        return 1;
    else if(Tkn->lex == KEYWORD && Tkn->value.key == FOR && T == TFOR) 
        return 1;
    else if(Tkn->lex == KEYWORD && Tkn->value.key == IN && T == TIN) 
        return 1;
    else if(Tkn->lex == KEYWORD && Tkn->value.key == SWITCH && T == TSWITCH) 
        return 1;
    else if(Tkn->lex == KEYWORD && Tkn->value.key == CASE && T == TCASE) 
        return 1;
    else if(Tkn->lex == KEYWORD && Tkn->value.key == BREAK && T == TBREAK) 
        return 1;
    else if(Tkn->lex == KEYWORD && Tkn->value.key == DEFAULT && T == TDEFAULT) 
        return 1;
    else if(Tkn->lex == KEYWORD && Tkn->value.key == WHILE && T == TWHILE) 
        return 1;
    else return 0;    
}

int TokToTerm(Token* Tkn){
    if(Tkn->lex == PLUS) 
        return TPLUS;
    else if(Tkn->lex == MINUS) 
        return TMINUS;
    else if(Tkn->lex == MUL) 
        return TMUL;
    else if(Tkn->lex == DIV) 
        return TDIV;
    else if(Tkn->lex == LT) 
        return TLT;
    else if(Tkn->lex == LE) 
        return TLE;
    else if(Tkn->lex == GE) 
        return TGE;
    else if(Tkn->lex == GT) 
        return TGT;
    else if(Tkn->lex == EQ) 
        return TEQ;
    else if(Tkn->lex == NE) 
        return TNE;
    else if(Tkn->lex == DEF) 
        return TDEF;
    else if(Tkn->lex == ENDDEF) 
        return TENDDEF;
    else if(Tkn->lex == DRIVERDEF)
        return TDRIVERDEF;
    else if(Tkn->lex == DRIVERENDDEF)
        return TDRIVERENDDEF;
    else if(Tkn->lex == COLON) 
        return TCOLON;
    else if(Tkn->lex == RANGEOP) 
        return TRANGEOP;
    else if(Tkn->lex == SEMICOL) 
        return TSEMICOL;
    else if(Tkn->lex == COMMA) 
        return TCOMMA;
    else if(Tkn->lex == ASSIGNOP) 
        return TASSIGNOP;
    else if(Tkn->lex == SQBO) 
        return TSQBO;
    else if(Tkn->lex == SQBC) 
        return TSQBC;
    else if(Tkn->lex == BO) 
        return TBO;
    else if(Tkn->lex == BC) 
        return TBC;
    else if(Tkn->lex == ID) 
        return TID;
    else if(Tkn->lex == NUM)
        return TNUM;    
    else if(Tkn->lex == RNUM)
        return TRNUM;
    else if(Tkn->lex == KEYWORD && Tkn->value.key == INTEGER) 
        return TINTEGER;
    else if(Tkn->lex == KEYWORD && Tkn->value.key == REAL) 
        return TREAL;
    else if(Tkn->lex == KEYWORD && Tkn->value.key == BOOLEAN) 
        return TBOOLEAN;
    else if(Tkn->lex == KEYWORD && Tkn->value.key == OF) 
        return TOF;
    else if(Tkn->lex == KEYWORD && Tkn->value.key == ARRAY) 
        return TARRAY;
    else if(Tkn->lex == KEYWORD && Tkn->value.key == START) 
        return TSTART;
    else if(Tkn->lex == KEYWORD && Tkn->value.key == END) 
        return TEND;
    else if(Tkn->lex == KEYWORD && Tkn->value.key == DECLARE) 
        return TDECLARE;
    else if(Tkn->lex == KEYWORD && Tkn->value.key == MODULE) 
        return TMODULE;
    else if(Tkn->lex == KEYWORD && Tkn->value.key == DRIVER) 
        return TDRIVER;
    else if(Tkn->lex == KEYWORD && Tkn->value.key == PROGRAM) 
        return TPROGRAM;
    // else if(Tkn->lex == KEYWORD && Tkn->value.key == RECORD && T == TRECORD) 
    //     return 1;
    // else if(Tkn->lex == KEYWORD && Tkn->value.key == TAGGED && T == TTAGGED) 
    //     return 1;
    // else if(Tkn->lex == KEYWORD && Tkn->value.key == UNION && T == TUNION) 
    //     return 1;
    else if(Tkn->lex == KEYWORD && Tkn->value.key == GET_VALUE) 
        return TGET_VALUE;
    else if(Tkn->lex == KEYWORD && Tkn->value.key == PRINT) 
        return TPRINT;
    else if(Tkn->lex == KEYWORD && Tkn->value.key == USE) 
        return TUSE;
    else if(Tkn->lex == KEYWORD && Tkn->value.key == WITH) 
        return TWITH;
    else if(Tkn->lex == KEYWORD && Tkn->value.key == PARAMETERS) 
        return TPARAMETERS;
    else if(Tkn->lex == KEYWORD && Tkn->value.key == TRUE) 
        return TTRUE;
    else if(Tkn->lex == KEYWORD && Tkn->value.key == FALSE) 
        return TFALSE;
    else if(Tkn->lex == KEYWORD && Tkn->value.key == TAKES) 
        return TTAKES;
    else if(Tkn->lex == KEYWORD && Tkn->value.key == INPUT) 
        return TINPUT;
    else if(Tkn->lex == KEYWORD && Tkn->value.key == RETURNS) 
        return TRETURNS;
    else if(Tkn->lex == KEYWORD && Tkn->value.key == AND) 
        return TAND;
    else if(Tkn->lex == KEYWORD && Tkn->value.key == OR) 
        return TOR;
    else if(Tkn->lex == KEYWORD && Tkn->value.key == FOR) 
        return TFOR;
    else if(Tkn->lex == KEYWORD && Tkn->value.key == IN) 
        return TIN;
    else if(Tkn->lex == KEYWORD && Tkn->value.key == SWITCH) 
        return TSWITCH;
    else if(Tkn->lex == KEYWORD && Tkn->value.key == CASE) 
        return TCASE;
    else if(Tkn->lex == KEYWORD && Tkn->value.key == BREAK) 
        return TBREAK;
    else if(Tkn->lex == KEYWORD && Tkn->value.key == DEFAULT) 
        return TDEFAULT;
    else if(Tkn->lex == KEYWORD && Tkn->value.key == WHILE) 
        return TWHILE;
    else if(Tkn->lex == END_OF_INPUT_STREAM)
        return NO_OF_T;
}


Token* TermToTok(Terminal T){
    Token* Tkn = (Token*)malloc(sizeof(Token));
    Tkn-> length = 0;
    Tkn->error = false;
    if(T == TPLUS) 
        Tkn->lex = PLUS;
    else if(T == TMINUS) 
        Tkn->lex = MINUS;
    else if(T == TMUL) 
        Tkn->lex = MUL;
    else if(T == TDIV) 
        Tkn->lex = DIV;
    else if(T == TLT) 
        Tkn->lex = LT;
    else if(T == TLE) 
        Tkn->lex = LE;
    else if(T == TGE) 
        Tkn->lex = GE;
    else if(T == TGT) 
        Tkn->lex = GT;
    else if(T == TEQ) 
        Tkn->lex = EQ;
    else if(T == TNE) 
        Tkn->lex = NE;
    else if(T == TDEF) 
        Tkn->lex = DEF;
    else if(T == TENDDEF) 
        Tkn->lex = ENDDEF;
    else if(T == TDRIVERDEF)
        Tkn->lex = DRIVERDEF;
    else if(T == TDRIVERENDDEF)
        Tkn->lex = DRIVERENDDEF;
    else if(T == TCOLON) 
        Tkn->lex = COLON;
    else if(T == TRANGEOP) 
        Tkn->lex = RANGEOP; 
    else if(T == TSEMICOL) 
        Tkn->lex = SEMICOL;
    else if(T == TCOMMA) 
        Tkn->lex = COMMA;
    else if(T == TASSIGNOP) 
        Tkn->lex = ASSIGNOP;
    else if(T == TSQBO) 
        Tkn->lex = SQBO;
    else if(T == TSQBC) 
        Tkn->lex = SQBC;
    else if(T == TBO) 
        Tkn->lex = BO;
    else if(T == TBC) 
        Tkn->lex = BC;
    else if(T == TID) 
        Tkn->lex = ID; 
    else if(T == TNUM)
        Tkn->lex = NUM;    
    else if(T == TRNUM)
        Tkn->lex = RNUM;
    else if(T == TINTEGER){ 
        Tkn->lex = KEYWORD;
        Tkn->value.key = INTEGER;}
    else if(T == TREAL){ 
        Tkn->lex = KEYWORD;
        Tkn->value.key = REAL;} 
    else if(T == TBOOLEAN){ 
        Tkn->lex = KEYWORD;
        Tkn->value.key = BOOLEAN;}
    else if(T == TOF){ 
        Tkn->lex = KEYWORD;
        Tkn->value.key = OF;}
    else if(T == TARRAY){ 
        Tkn->lex = KEYWORD;
        Tkn->value.key = ARRAY;}
    else if(T == TSTART){ 
        Tkn->lex = KEYWORD;
        Tkn->value.key = START;}
    else if(T == TEND){ 
        Tkn->lex = KEYWORD;
        Tkn->value.key = END;}
    else if(T == TDECLARE){ 
        Tkn->lex = KEYWORD;
        Tkn->value.key = DECLARE;}
    else if(T == TMODULE){ 
        Tkn->lex = KEYWORD;
        Tkn->value.key = MODULE;}
    else if(T == TDRIVER){ 
        Tkn->lex = KEYWORD;
        Tkn->value.key = DRIVER;}
    else if(T == TPROGRAM){ 
        Tkn->lex = KEYWORD;
        Tkn->value.key = PROGRAM;}
    else if(T == TGET_VALUE){ 
        Tkn->lex = KEYWORD;
        Tkn->value.key = GET_VALUE;}
    else if(T == TPRINT){ 
        Tkn->lex = KEYWORD;
        Tkn->value.key = PRINT;}
    else if(T == TUSE){ 
        Tkn->lex = KEYWORD;
        Tkn->value.key = USE;}
    else if(T == TWITH){ 
        Tkn->lex = KEYWORD;
        Tkn->value.key = WITH;}
    else if(T == TPARAMETERS){ 
        Tkn->lex = KEYWORD;
        Tkn->value.key = PARAMETERS;}
    else if(T == TTRUE){ 
        Tkn->lex = KEYWORD;
        Tkn->value.key = TRUE;}
    else if(T == TFALSE){ 
        Tkn->lex = KEYWORD;
        Tkn->value.key = FALSE;}
    else if(T == TTAKES){ 
        Tkn->lex = KEYWORD;
        Tkn->value.key = TAKES;}
    else if(T == TINPUT){ 
        Tkn->lex = KEYWORD;
        Tkn->value.key = INPUT;}
    else if(T == TRETURNS){ 
        Tkn->lex = KEYWORD;
        Tkn->value.key = RETURNS;}
    else if(T == TAND){ 
        Tkn->lex = KEYWORD;
        Tkn->value.key = AND;}
    else if(T == TOR){ 
        Tkn->lex = KEYWORD;
        Tkn->value.key = OR;}
    else if(T == TFOR){ 
        Tkn->lex = KEYWORD;
        Tkn->value.key = FOR;}
    else if(T == TIN){ 
        Tkn->lex = KEYWORD;
        Tkn->value.key = IN;}
    else if(T == TSWITCH){ 
        Tkn->lex = KEYWORD;
        Tkn->value.key = SWITCH;}
    else if(T == TCASE){ 
        Tkn->lex = KEYWORD;
        Tkn->value.key = CASE;}
    else if(T == TBREAK){ 
        Tkn->lex = KEYWORD;
        Tkn->value.key = BREAK;}
    else if(T == TDEFAULT){ 
        Tkn->lex = KEYWORD;
        Tkn->value.key = DEFAULT;}
    else if(T == TWHILE){ 
        Tkn->lex = KEYWORD;
        Tkn->value.key = WHILE;}
    else Tkn->lex = EMPTY_TOKEN;
    return Tkn;
}