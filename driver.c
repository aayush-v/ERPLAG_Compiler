// Group Number - 25
// Name : Aayush Atul Verma         ID : 2017A7PS0061P
// Name : Aditya Ramaswamy          ID : 2017A7PS0130P
// Name : Jithin Kallukalam Sojan   ID : 2017A7PS0163P

#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "hashtable.h"
#include "type.h"
#include "semantic.h"
#include "codeGen.h"
#include "asmGen.h"
#include "Ast-Accessory.h"

int main(int argc, char *argv[]){

    clock_t start_time, end_time;
    double total_CPU_time, total_CPU_time_in_seconds;
    char c = 'z';
    char ch1;

    int a = 0;
    int b = 0;
    int e = 0;
    int* error = &a;
    int* parseTreeCount = &b;
    int* ASTTreeCount = &e;
    Grammar* G=NULL;
    ParseNode* Root=NULL;
    astNode* aroot=NULL;
    hashTable* st = NULL;

    while(c!='0'){ 
        printf("LEVEL 4: Symbol Table/type Checking/ Semantic Rules modules work/ handled static and dynamic arrays in type checking and code generation \n"); 
        printf("Choose a new option.\n0 : Exit\n1 : Lexer\n2 : Parser\n");
        printf("3 : AST\n4 : Memory\n5 : Symbol Table\n6 : Activation Record Size\n");
        printf("7 : Static and Dynamic arrays\n8 : Error Reporting and total compiling time\n");
        printf("9 : Code Generation\n");
        c = getchar();
        ch1 = getchar();
        int d = c - '0';
       
        switch(d){

     // Exit scenario
            case 0:        
                printf("Thank you. Exiting...\n");
                break;


     // Lexer invoked and tokens printed on the console
            case 1:        
                printf("%-10s%-20s%-20s\n","Line_no", "Token_name", "Lexeme");         
                runLexer(argv[1]);
                break;


     // Parser invoked to verify syntactic correctness
            case 2:        
                G = createGrammar(); 
                *error = 0;
                *parseTreeCount = 0;
                Root = creatGramAndParse(argv[1], argv[2],1,G, error, parseTreeCount);
                if(*error==1){
                    Root = NULL;
                }
                break;


     // Creating and printing AST based on traversal order
            case 3:        
                if(Root== NULL){
                    G = createGrammar(); 
                    *error = 0;
                    *parseTreeCount = 0;
                    Root = creatGramAndParse(argv[1], argv[2],0,G, error, parseTreeCount);
                    if(*error==1){
                        printf("Syntactic errors.\n");
                        Root = NULL;
                        break;
                    }
                }
                if(aroot!= NULL){
                    *ASTTreeCount = 0;
                    PrintASTTree(aroot, ASTTreeCount,1);    //Printing AST Tree
                    break;
                }
                else if(!(*error)){
                    aroot = createAST(Root);
                    freeCells(G);                   
                    *ASTTreeCount = 0;
                    PrintASTTree(aroot, ASTTreeCount,1);    //Printing AST Tree
                }
                break;


     // Memory allocated to the Parse Tree and AST
            case 4:    
                if(Root== NULL){
                    G = createGrammar(); 
                    *error = 0;
                    *parseTreeCount = 0;
                    Root = creatGramAndParse(argv[1], argv[2],0,G, error, parseTreeCount);
                    if(*error==1){
                        Root=NULL;
                        printf("Syntactic errors.\n");
                        break;
                    }
                }
                if(aroot==NULL){
                    aroot = createAST(Root);
                    freeCells(G);                   
                    *ASTTreeCount = 0;
                    PrintASTTree(aroot, ASTTreeCount,0);
                }
                else{
                    *ASTTreeCount = 0;
                    PrintASTTree(aroot,ASTTreeCount,0);
                }            
                printf("Parse tree Number of nodes = %d          Allocated Memory = %d bytes\n", *parseTreeCount, (int)((*parseTreeCount)*sizeof(ParseNode)));
                printf("Ast tree Number of nodes = %d          Allocated Memory = %d bytes\n", *ASTTreeCount, (int)((*ASTTreeCount)*sizeof(astNode)));

                // COMPUTING COMPRESSION SIZE
                double comp = (*parseTreeCount)*sizeof(ParseNode)-(*ASTTreeCount)*sizeof(astNode);
                comp/=(*parseTreeCount)*sizeof(ParseNode)*1.0;
                comp*=100;

                printf("Compression precentage= %f\n",comp);

                break;


     // Populating and printng Symbol Table
            case 5:         
                if(Root== NULL){
                    G = createGrammar(); 
                    *error = 0;
                    *parseTreeCount = 0;
                    Root = creatGramAndParse(argv[1], argv[2],0,G, error, parseTreeCount);
                    if(*error==1){
                        Root = NULL;
                        printf("Syntactic errors in code.\n");
                        break;
                    }
                }
                if(aroot==NULL){
                    aroot = createAST(Root);
                    freeCells(G);
                }
                if(st==NULL){
                    *error = 0;
                    st = typeStart(aroot,error);
                    semanticCheck(aroot,st,error);
                    if(*error){
                        printf("Semantic Errors in code.\n");
                        printST(st);            // Printing Symbol Table
                        st = NULL;
                        break;
                    }
                }
                if(st!=NULL)
                    printST(st);        //Printing Symbol Table

                break;


     // Activation Record Size, printing total memory requirement
            case 6:
                if(Root== NULL){
                    G = createGrammar(); 
                    *error = 0;
                    *parseTreeCount = 0;
                    Root = creatGramAndParse(argv[1], argv[2],0,G, error, parseTreeCount);
                    if(*error==1){
                        printf("Syntactic errors in code.\n");
                        break;
                    }
                }
                if(aroot==NULL){
                    aroot = createAST(Root);
                    freeCells(G);
                }
                if(st==NULL){
                    *error = 0;
                    st = typeStart(aroot,error);
                    semanticCheck(aroot,st,error);
                    if(*error){
                        printf("Semantic Errors in code.\n");
                        printARSize(st);            // Printing Activation Record Size in ST
                        st = NULL;
                        break;
                    }
                }
                if(st!=NULL)
                    printARSize(st);        // Printing Activation Record Size in ST    
                break;


     // Static and dynamic arrays
            case 7:
                if(Root== NULL){            // If Parse Tree hasnt been created yet
                    G = createGrammar(); 
                    *error = 0;
                    *parseTreeCount = 0;
                    Root = creatGramAndParse(argv[1], argv[2],0,G, error, parseTreeCount);
                    if(*error==1){
                        printf("Syntactic errors in code.\n");
                        break;
                    }
                }
                if(aroot==NULL){                // If AST Tree hasnt been created yet
                    aroot = createAST(Root);
                    freeCells(G);
                }
                if(st==NULL){
                    *error = 0;
                    st = typeStart(aroot,error);
                    semanticCheck(aroot,st,error);
                    if(*error){
                        printf("Semantic Errors in code.\n");
                        printArrFeatures(st);       
                        st = NULL;
                        break;
                    }
                }
                if(st!=NULL)                // If symbol table is not NULL
                    printArrFeatures(st);         
                break;


     // Error Reporting and printing total compile time
            case 8:         
                start_time = clock();
                *error = 0;
                G = createGrammar();
                Root = creatGramAndParse(argv[1], argv[2],2,G,error, parseTreeCount);

                if(!(*error)){
                    aroot = createAST(Root);
                    freeCells(G);
                    st = typeStart(aroot,error);
                    semanticCheck(aroot,st,error);
                    
                    if(!(*error))printf("Code compiles successfully..........\n");
                }

                end_time = clock();
                total_CPU_time  =  (double) (end_time - start_time);
                total_CPU_time_in_seconds =   total_CPU_time / CLOCKS_PER_SEC;
                printf("Total CPU time is %f\n", total_CPU_time);
                printf("Total CPU time in seconds is %f\n", total_CPU_time_in_seconds);
                break;


     // Code Generation
            case 9:        
                *error = 0;
                G = createGrammar();
                Root = creatGramAndParse(argv[1], argv[2],0,G,error, parseTreeCount);

                if(!(*error)){
                    aroot = createAST(Root);
                    freeCells(G);
                    st = typeStart(aroot,error);
                    semanticCheck(aroot,st,error);
                    
                    if(!(*error))printf("Code compiles successfully..........\n");
                    
                    if(!*error){
                        IrCode* program = codeTree(aroot);
                        asmCreate(program,argv[2]);
                        printf("Asm file created..........\n");
                    }
                }
                break;

        }
    }
}


