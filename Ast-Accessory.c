// Group Number - 25
// Name : Aayush Atul Verma         ID : 2017A7PS0061P
// Name : Aditya Ramaswamy          ID : 2017A7PS0130P
// Name : Jithin Kallukalam Sojan   ID : 2017A7PS0163P

#include "Ast-Accessory.h"
#include<stdlib.h>
#include<string.h>


char* NodeToString(Label lab){

	switch(lab)
	{
        case _PROGRAM: return "Program";
        case _MODULEDECLARATIONS: return "Module_Declaration";
        case _DRIVERMODULE: return "Driver_Module";
        case _MODID: return "ModID";
        case _MODULE: return "Module";
        case _INPUT_PLIST: return "Input_plist";
        case _OUTPUT_PLIST: return "Output_plist";
        case _ID: return "ID";
        case _INTEGER: return "Integer";
        case _REAL: return "Real";
        case _BOOLEAN: return "Boolean";
        case _ARRAY: return "Array";
        case _STATEMENTS: return "Statements";
        case _DECLARESTMT: return "Declare_Stmt";
        case _CONDITIONALSTMT: return "Conditional_Stmt";
        case _GET_VALUE: return "Get_value";
        case _PRINT: return "Print";
        case _TRUE: return "True";
        case _FALSE: return "False";
        case _NUM: return "Num";
        case _RNUM: return "Rnum";
        case _ASSIGNOP: return "AssignOp";
        case _MODULEREUSESTMT: return "ModuleReuse_Stmt";
        case _U: return "U";
        case _PLUS: return "Plus";
        case _MINUS: return "Minus";
        case _MUL: return "Mul";
        case _DIV: return "Div";
        case _AND: return "AND";
        case _OR: return "OR";
        case _LT: return "LT";
        case _LE: return "LE";
        case _GT: return "GT";
        case _GE: return "GE";
        case _EQ: return "EQ";
        case _NE: return "NE";
        case _DEFAULT: return "Default";
        case _FOR: return "For";
        case _WHILE: return "While";
        case _IDLIST: return "IdList";
    }


}



void PrintASTTree(astNode* root, int* ASTTreeCount,int flag){
    if(flag==1){
        printf("Inorder traversal\n");
        printf("%-25s%-25s%-25s%-25s%-25s\n","CurrentNode","Lexeme","ValueifAny","lineNumber","ParentNode" );
    }
    ASTinorder(root, ASTTreeCount,flag);
}

void ASTinorder(astNode* root, int* ASTTreeCount,int flag){
    if(root == NULL) return;
    *ASTTreeCount +=1;

    if(root->First) ASTinorder(root->First, ASTTreeCount,flag);

    if(flag==1){

        printf("%-25s", NodeToString(root->tag));
        if ((root->t)!=NULL && (root->tag)==3){
            printf("%-25s%-25s%-25d", ((root->t)->value).identifier,"---", (root->t)->line_no);
        }
        else if ((root->t)!=NULL && (root->tag)==7){
            printf("%-25s%-25s%-25d", ((root->t)->value).identifier, "---", (root->t)->line_no);
        }
        else if ((root->t)!=NULL && (root->tag)==19){
            printf("%-25s%-25d%-25d", "---", (int)((root->t)->value).integer ,(root->t)->line_no);
        }
        else if ((root->t)!=NULL && (root->tag)==20){
            printf("%-25s%-25f%-25d", "---", ((root->t)->value).real,(root->t)->line_no);
        }
        else
            printf("%-25s%-25s%-25s","---","---", "---");

        
        if((root->Parent)!= NULL)
            printf("%-25s", NodeToString((root->Parent)->tag))  ;  
        else
        {
            printf("%-25s", "---");
        }

        printf("\n");

    }

    if(root->First){
		astNode* curr = (root->First)->Right;
		while(curr){
			ASTinorder(curr,ASTTreeCount,flag);
			curr = curr->Right;
		}
	}
	return;

}
