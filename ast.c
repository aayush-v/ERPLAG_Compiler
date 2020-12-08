// Group Number - 25
// Name : Aayush Atul Verma         ID : 2017A7PS0061P
// Name : Aditya Ramaswamy          ID : 2017A7PS0130P
// Name : Jithin Kallukalam Sojan   ID : 2017A7PS0163P

/*
Each node is a parseNode itself. This module is created with the intention of reducing the number of nodes in the parse tree largely.
If a particular parseNode doesnt require Token*, it is freed.\
But we do not free Cell* as there might be other nodes using it.
*/

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include "ast.h"
#include "Lexer-Accessory.h"
#include "Parser-Accessory.h"
#include "stack.h"

astNode* createAST(ParseNode* Root){
    PostOrder(Root);
    
    astNode* aroot = createNode(_PROGRAM,NULL);
    addChild(aroot,Root->Node);
    free(Root);
    return aroot;
    //Remember to free the parseTable and stuff.
}

void PostOrder(ParseNode* Root){
    ParseNode* child;
    ParseNode* temp;
    child = Root->First;
    if(child){
        firstTrav(child);
        //This is to take care of the inh attributes
        PostOrder(child);
        temp = child->Right;
        while(temp){
            firstTrav(temp);
            //This is to take care of the inh attributes
            PostOrder(temp);
            temp = temp->Right;
        }
    }
    secondTrav(Root);
    return;
    //if any of the required nodes are NULL: syntactic error, so make the syn attribute NULL.
    //There should ideally be no semantic analysis.
    //This is to take care of the syn attributes
}

void firstTrav(ParseNode* node){
    short ruleNum = node->rule1;
    if(ruleNum!=-1){
        //As long as it is not the NTPROGRAM.

        switch(ruleNum){
            case (short)((NTDATATYPE<<3)|(4)):
                if((node->c)->tag==1 && ((node->c)->value).NT==NTTYPE){
                    node->inh = (node->Parent)->First->Right->Right->Node;
                }
                break;
            case (short)((NTARITHMETICORBOOLEANEXPR<<3)|(1)):
                if((node->c)->tag==1 && ((node->c)->value).NT==NTN7){
                    node->inh = (node->Parent)->First->Node; 
                }
                break;
            case (short)((NTN7<<3)|(1)):
                if((node->c)->tag==1 && ((node->c)->value).NT==NTN7){
                    node->inh = (node->Parent)->First->Node;
                    if(node->inh && (node->Parent)->inh)
                        node->inh->First = (node->Parent)->inh;
                    else{
                        node->inh = NULL;
                    }
                    if(node->inh && (node->Parent)->First->Right->Node)
                        node->inh->First->Right = (node->Parent)->First->Right->Node;
                    else{
                        node->inh = NULL;
                    } 
                }
                break;
            case (short)((NTANYTERM<<3)|(1)):
                if((node->c)->tag==1 && ((node->c)->value).NT==NTN8){
                    node->inh = (node->Parent)->First->Node; 
                }
                break;
            case (short)((NTN8<<3)|(1)):
                if((node->c)->tag==1 && ((node->c)->value).NT==NTARITHMETICEXPR){
                    node->inh = (node->Parent)->First->Node;
                    if(node->inh && (node->Parent)->inh)
                        node->inh->First = (node->Parent)->inh;
                    else node->inh = NULL;
                }
                break;
            case (short)((NTARITHMETICEXPR<<3)|(1)):
                if((node->c)->tag==1 && ((node->c)->value).NT==NTN4){
                    node->inh = (node->Parent)->First->Node; 
                }
                break;
            case (short)((NTN4<<3)|(1)):
                if((node->c)->tag==1 && ((node->c)->value).NT==NTN4){
                    node->inh = (node->Parent)->First->Node;
                    if(node->inh && (node->Parent)->inh)
                        node->inh->First = (node->Parent)->inh;
                    else node->inh = NULL;
                    if(node->inh && (node->Parent)->First->Right->Node)
                        node->inh->First->Right = (node->Parent)->First->Right->Node;
                    else node->inh = NULL;
                }
                break;
            case (short)((NTTERM<<3)|(1)):
                if((node->c)->tag==1 && ((node->c)->value).NT==NTN5){
                    node->inh = (node->Parent)->First->Node; 
                }
                break;
            case (short)((NTN5<<3)|(1)):
                if((node->c)->tag==1 && ((node->c)->value).NT==NTN5){
                    node->inh = (node->Parent)->First->Node;
                    if(node->inh && (node->Parent)->inh)
                        node->inh->First = (node->Parent)->inh;
                    else node->inh = NULL;
                    if(node->inh && (node->Parent)->First->Right->Node)
                        node->inh->First->Right = (node->Parent)->First->Right->Node;
                    else node->inh = NULL;
                }
                break;
        }
    }
}

void addChild(astNode* curr,astNode* child){
    if(curr->First==NULL){
        curr->First = child;
    }
    else{
        astNode* temp = curr->First;
        while(temp->Right)temp = temp->Right;
        temp->Right = child;
    }
    while(child){
        child->Parent = curr;
        child = child->Right;
    }
}

astNode* addSibling(astNode* curr,astNode* sib){
    astNode* temp = curr;
    if(temp==NULL){
        return sib;
    }
    else{
        while(temp->Right)temp = temp->Right;
        temp->Right = sib;
        return curr;
    }
}

void freeList(ParseNode* list){
    ParseNode* next;
    while(list){
        next = list->Right;
        if(list->t){
            free(list->t);
        }
        free(list);
        list = next;
    }
}

astNode* createNode(Label l,Token* t){
    astNode* new = (astNode*)malloc(sizeof(astNode));
    new->tag = l;
    new->t = t;
    new->First = NULL;
    new->Right = NULL;
    new->Parent = NULL;
    new->entry = NULL;
    new->error = 0;
    new->True = 0;
    new->False = 0;
}

void secondTrav(ParseNode* node){
        //For debugging:
        // if(node->c->tag==2){
        //     printf("%s\n",NonTerminalToString(node->c->value.T));
        // }
        // else if(node->c->tag==1){
        //     printf("%s\n",NonTerminalToString(node->c->value.NT));
        // }
        // else{
        //     printf("Its something else.\n");
        // }
        short ruleNum = node->rule2;
        //An error should be cut of at the statement level.
        //If a null statement, let it be.

        if(ruleNum==0 && node->c->tag==1){
            node->Node = NULL;
            return;
            //As it does not expand any rule.
        }

        if(node->First==NULL){
            node->Node = NULL;
            return;
        }
        astNode* temp;
        //end will never be NULL.
        ParseNode* curr;
        short start;
        short end;
        switch(ruleNum){
            case (short)((NTPROGRAM<<3)|(1)):
                //Three variables, current, next and the current end of the children under the node in consideration.
                curr = node->First;
                temp = createNode(_MODULEDECLARATIONS,NULL);
                addChild(temp,curr->Node);
                node->Node = addSibling(node->Node,temp);

                curr = curr->Right;
                node->Node = addSibling(node->Node,curr->Node);

                curr = curr->Right;
                temp = createNode(_DRIVERMODULE,NULL);
                addChild(temp,curr->Node);
                temp->scope[0] = curr->Node->scope[0];
                temp->scope[1] = curr->Node->scope[1];
                node->Node = addSibling(node->Node,temp);
                

                curr = curr->Right;
                node->Node = addSibling(node->Node,curr->Node);

                break;
            
            case (short)((NTMODULEDECLARATIONS<<3)|(1)):
                curr = node->First;
                node->Node = addSibling(node->Node,curr->Node);

                curr = curr->Right;
                node->Node = addSibling(node->Node,curr->Node);

                break;

            case (short)((NTMODULEDECLARATIONS<<3)|(2)):
                node->Node = NULL;
                
                break;

            case (short)((NTMODULEDECLARATION<<3)|(1)):
                curr = node->First->Right->Right;
                if(curr->t==NULL)node->Node==NULL;
                else{
                    node->Node = createNode(_MODID,curr->t);
                    curr->t = NULL;
                    //This is so that the token doesn't get freed.
                }

                break;

            case (short)((NTOTHERMODULES<<3)|(1)):
                curr = node->First;
                temp = createNode(_MODULE,NULL);
                addChild(temp,curr->Node);
                if(curr->Node->Right->Right->Right){
                    temp->scope[0] = curr->Node->Right->Right->Right->scope[0];
                    temp->scope[1] = curr->Node->Right->Right->Right->scope[1];
                }
                else{
                    temp->scope[0] = curr->Node->Right->Right->scope[0];
                    temp->scope[1] = curr->Node->Right->Right->scope[1];
                }
                node->Node = addSibling(node->Node,temp);

                curr = curr->Right;
                node->Node = addSibling(node->Node,curr->Node);

                break;
            
            case (short)((NTOTHERMODULES<<3)|(2)):
                node->Node = NULL;

                break;
            
            case (short)((NTDRIVERMODULE<<3)|(1)):
                curr = node->First->Right->Right->Right->Right;
                node->Node = addSibling(node->Node,curr->Node);

                break;
            
            case (short)((NTMODULE<<3)|(1)):
                curr = node->First->Right->Right;
                if(curr->t==NULL){
                    break;
                }
                else{
                    temp = createNode(_MODID,curr->t);
                    node->Node = temp;
                    curr->t = NULL;
                }

                curr = curr->Right->Right->Right->Right->Right;
                temp = createNode(_INPUT_PLIST,NULL);
                addChild(temp,curr->Node);
                node->Node = addSibling(node->Node,temp);

                curr = curr->Right->Right->Right;
                node->Node = addSibling(node->Node,curr->Node);

                curr = curr->Right;
                node->Node = addSibling(node->Node,curr->Node);

                break;
            
            case (short)((NTRET<<3)|(1)):
                curr = node->First->Right->Right;
                temp = createNode(_OUTPUT_PLIST,NULL);
                addChild(temp,curr->Node);
                node->Node = addSibling(node->Node,temp);

                break;

            case (short)((NTRET<<3)|(2)):
                node->Node = NULL;

                break;

            case (short)((NTINPUT_PLIST<<3)|(1)):
                curr = node->First;
                if(curr->t==NULL){
                    node->Node=NULL;
                    //break;
                }
                else{
                    temp = createNode(_ID,curr->t);
                    node->Node = addSibling(node->Node,temp);
                    curr->t = NULL;
                }

                curr = curr->Right->Right;
                node->Node = addSibling(node->Node,curr->Node);

                curr = curr->Right;
                node->Node = addSibling(node->Node,curr->Node);

                break;
            
            case (short)((NTN1<<3)|(1)):
                curr = node->First->Right;
                if(curr->t==NULL){
                    node->Node = NULL;
                    //break;
                }
                else{
                    temp = createNode(_ID,curr->t);
                    node->Node = addSibling(node->Node,temp);
                    curr->t = NULL;
                }

                curr = curr->Right->Right;
                node->Node = addSibling(node->Node,curr->Node);

                curr = curr->Right;
                node->Node = addSibling(node->Node,curr->Node);

                break;
            
            case (short)((NTN1<<3)|(2)):
                node->Node = NULL;

                break;
            
            case (short)((NTOUTPUT_PLIST<<3)|(1)):
                curr = node->First;
                if(curr->t==NULL){
                    node->Node=NULL;
                    //is the break really necessary.
                    //break;
                }
                else{
                    temp = createNode(_ID,curr->t);
                    node->Node = addSibling(node->Node,temp);
                    curr->t = NULL;
                }

                curr = curr->Right->Right;
                node->Node = addSibling(node->Node,curr->Node);

                curr = curr->Right;
                node->Node = addSibling(node->Node,curr->Node);

                break;
            
            case (short)((NTN2<<3)|(1)):
                curr = node->First->Right;
                if(curr->t==NULL){
                    node->Node = NULL;
                    //is the break necessary?
                    //break;
                }
                else{
                    temp = createNode(_ID,curr->t);
                    node->Node = addSibling(node->Node,temp);
                    curr->t = NULL;
                }

                curr = curr->Right->Right;
                node->Node = addSibling(node->Node,curr->Node);

                curr = curr->Right;
                node->Node = addSibling(node->Node,curr->Node);

                break;
            
            case (short)((NTN2<<3)|(2)):
                node->Node = NULL;

                break;
            case (short)((NTDATATYPE<<3)|(1)):
                node->Node = createNode(_INTEGER,NULL);

                break;

            case (short)((NTDATATYPE<<3)|(2)):
                node->Node = createNode(_REAL,NULL);

                break;

            case (short)((NTDATATYPE<<3)|(3)):
                node->Node = createNode(_BOOLEAN,NULL);

                break;

            case (short)((NTDATATYPE<<3)|(4)):
                curr = node->First->Right->Right->Right->Right->Right;
                temp = createNode(_ARRAY,NULL);
                addChild(temp,curr->Node);
                node->Node = addSibling(node->Node,temp);

                break;
                
            case (short)((NTRANGE_ARRAYS<<3)|(1)):
                //<index>.child, instead of just <index>
                curr = node->First;
                node->Node = addSibling(node->Node,curr->Node);

                curr = curr->Right->Right;
                node->Node = addSibling(node->Node,curr->Node);

                break;

            case (short)((NTTYPE<<3)|(1)):

                temp = createNode(_INTEGER,NULL);
                node->Node = addSibling(node->Node,temp);

                node->Node = addSibling(node->Node,node->inh);

                break;

            case (short)((NTTYPE<<3)|(2)):

                temp = createNode(_REAL,NULL);
                node->Node = addSibling(node->Node,temp);

                node->Node = addSibling(node->Node,node->inh);

                break;

            case (short)((NTTYPE<<3)|(3)):

                temp = createNode(_BOOLEAN,NULL);
                node->Node = addSibling(node->Node,temp);

                node->Node = addSibling(node->Node,node->inh);

                break;

            case (short)((NTMODULEDEF<<3)|(1)):
                curr = node->First;
                start = curr->t->line_no;

                curr = curr->Right;
                temp = createNode(_STATEMENTS,NULL);
                addChild(temp,curr->Node);
                node->Node = addSibling(node->Node,temp);

                curr = curr->Right;
                end = curr->t->line_no;

                node->Node->scope[0] = start;
                node->Node->scope[1] = end;

                break;
            
            case (short)((NTSTATEMENTS<<3)|(1)):
                curr = node->First;
                node->Node = addSibling(node->Node,curr->Node);

                curr = curr->Right;
                node->Node = addSibling(node->Node,curr->Node);

                break;
            
            case (short)((NTSTATEMENTS<<3)|(2)):
                node->Node = NULL;

                break;
            
            case (short)((NTSTATEMENT<<3)|(1)):
                curr = node->First;
                node->Node = addSibling(node->Node,curr->Node);

                break;
            
            case (short)((NTSTATEMENT<<3)|(2)):
                curr = node->First;
                node->Node = addSibling(node->Node,curr->Node);

                break;

            case (short)((NTSTATEMENT<<3)|(3)):
                curr = node->First;
                temp = createNode(_DECLARESTMT,NULL);
                addChild(temp,curr->Node);
                node->Node = addSibling(node->Node,temp);

                break;
            
            case (short)((NTSTATEMENT<<3)|(4)):
                curr = node->First;
                temp = createNode(_CONDITIONALSTMT,NULL);
                addChild(temp,curr->Node);
                temp->scope[0] = curr->Node->scope[0];
                temp->scope[1] = curr->Node->scope[1];
                node->Node = addSibling(node->Node,temp);

                break;
            case (short)((NTSTATEMENT<<3)|(5)):
                curr = node->First;
                node->Node = addSibling(node->Node,curr->Node);

                break;
            
            case (short)((NTIOSTMT<<3)|(1)):
                curr = node->First->Right->Right;
                if(curr->t==NULL){
                    node->Node = NULL;
                }
                else{
                    temp = createNode(_GET_VALUE,NULL);
                    addChild(temp,createNode(_ID,curr->t));
                    curr->t = NULL;
                    node->Node = addSibling(node->Node,temp);
                }

                break;
            
            case (short)((NTIOSTMT<<3)|(2)):
                curr = node->First->Right->Right;
                if(curr->Node==NULL){
                    //Will this work?

                    node->Node = NULL;
                }
                else{
                    temp = createNode(_PRINT,NULL);
                    addChild(temp,curr->Node);
                    node->Node = addSibling(node->Node,temp);
                }  

                break;             

            case (short)((NTBOOLCONSTT<<3)|(1)):
                curr = node->First;
                temp = createNode(_TRUE,curr->t);
                curr->t = NULL;
                node->Node = addSibling(node->Node,temp);

                break;

            case (short)((NTBOOLCONSTT<<3)|(2)):
                curr = node->First;
                temp = createNode(_FALSE,curr->t);
                curr->t = NULL;
                node->Node = addSibling(node->Node,temp);

                break;

            case (short)((NTVAR_ID_NUM<<3)|(1)):
                curr = node->First;
                if(curr->t==NULL){
                    node->Node = NULL;
                    break;
                }
                else{
                    temp = createNode(_ID,curr->t);
                    curr->t = NULL;
                    node->Node = addSibling(node->Node,temp);
                }

                curr = curr->Right;
                addChild(node->Node,curr->Node);

                break;
            
            case (short)((NTVAR_ID_NUM<<3)|(2)):
                curr = node->First;
                if(curr->t==NULL){
                    node->Node = NULL;
                    break;
                }
                else{
                    temp = createNode(_NUM,curr->t);
                    curr->t = NULL;
                    node->Node = addSibling(node->Node,temp);
                }

                break;

            case (short)((NTVAR_ID_NUM<<3)|(3)):
                curr = node->First;
                if(curr->t==NULL){
                    node->Node = NULL;
                    break;
                }
                else{
                    temp = createNode(_RNUM,curr->t);
                    curr->t = NULL;
                    node->Node = addSibling(node->Node,temp);
                }

                break;

            case (short)((NTVAR<<3)|(1)):
                curr = node->First;
                node->Node = addSibling(node->Node,curr->Node);

                break;
            
            case (short)((NTVAR<<3)|(2)):
                curr = node->First;
                node->Node = addSibling(node->Node,curr->Node);

                break;
            
            case (short)((NTWHICHID<<3)|(1)):
                curr = node->First->Right;
                node->Node = addSibling(node->Node,curr->Node);

                break;
            
            case (short)((NTWHICHID<<3)|(2)):
                node->Node = NULL;

                break;
            
            case (short)((NTSIMPLESTMT<<3)|(1)):
                curr = node->First;
                temp = createNode(_ASSIGNOP,NULL);
                addChild(temp,curr->Node);
                node->Node = addSibling(node->Node,temp);

                break;
            
            case (short)((NTSIMPLESTMT<<3)|(2)):
                curr = node->First;
                temp = createNode(_MODULEREUSESTMT,NULL);
                addChild(temp,curr->Node);
                node->Node = addSibling(node->Node,temp);

                break;
            
            case (short)((NTASSIGNMENTSTMT<<3)|(1)):
                curr = node->First;
                if(curr->t==NULL){
                    //If the ID doesnt exist, the expression is not calculated.
                    node->Node = NULL;
                    break;
                }
                else{
                    temp = createNode(_ID,curr->t);
                    curr->t = NULL;
                    node->Node = addSibling(node->Node,temp);
                }
                
                curr = curr->Right;
                if(curr->Node==NULL){
                    node->Node = NULL;
                    break;
                }
                temp = curr->Node->Right;
                if(temp){
                    curr->Node->Right = NULL;
                    addChild(node->Node,temp);
                }
                node->Node = addSibling(node->Node,curr->Node);

                break;
            
            case (short)((NTWHICHSTMT<<3)|(1)):
                curr = node->First;
                node->Node = addSibling(node->Node,curr->Node);

                break;
            
            case (short)((NTWHICHSTMT<<3)|(2)):
                curr = node->First;
                node->Node = addSibling(node->Node,curr->Node);

                break;
            
            case (short)((NTLVALUEIDSTMT<<3)|(1)):
                curr = node->First->Right;
                node->Node = addSibling(node->Node,curr->Node);

                break;
            
            case (short)((NTLVALUEARRSTMT<<3)|(1)):
                curr = node->First->Right->Right->Right->Right;
                node->Node = addSibling(node->Node,curr->Node);

                curr = node->First->Right;
                node->Node = addSibling(node->Node,curr->Node);

                //Error handling done in the ASSIGNMENTSTMT construct.

                break;
            
            case (short)((NTINDEX<<3)|(1)):
                curr = node->First;
                if(curr->t==NULL){
                    node->Node = NULL;
                    break;
                }
                else{
                    temp = createNode(_NUM,curr->t);
                    curr->t = NULL;
                    node->Node = addSibling(node->Node,temp);
                }

                break;
            
            case (short)((NTINDEX<<3)|(2)):
                curr = node->First;
                if(curr->t==NULL){
                    node->Node = NULL;
                    break;
                }
                else{
                    temp = createNode(_ID,curr->t);
                    curr->t = NULL;
                    node->Node = addSibling(node->Node,temp);
                }

                break;

            case (short)((NTMODULEREUSESTMT<<3)|(1)):
                curr = node->First;
                node->Node = addSibling(node->Node,curr->Node);

                curr = curr->Right->Right->Right;
                if(curr->t==NULL){
                    node->Node = NULL;
                    break;
                }
                else{
                    temp = createNode(_MODID,curr->t);
                    curr->t = NULL;
                    node->Node = addSibling(node->Node,temp);
                }

                curr = curr->Right->Right->Right;
                temp = createNode(_IDLIST,NULL);
                addChild(temp,curr->Node);
                node->Node = addSibling(node->Node,temp);

                break;

            case (short)((NTOPTIONAL<<3)|(1)):
                curr = node->First->Right;
                temp = createNode(_IDLIST,NULL);
                addChild(temp,curr->Node);
                node->Node = addSibling(node->Node,temp);

                break;

            case (short)((NTOPTIONAL<<3)|(2)):
                node->Node = NULL;

                break;

            case (short)((NTIDLIST<<3)|(1)):
                curr = node->First;
                if(curr->t==NULL){
                    node->Node = NULL;
                }
                else{
                    temp = createNode(_ID,curr->t);
                    curr->t = NULL;
                    node->Node = addSibling(node->Node,temp);
                }

                curr = curr->Right;
                node->Node = addSibling(node->Node,curr->Node);

                break;
            
            case (short)((NTN3<<3)|(1)):
                curr = node->First->Right;
                if(curr->t==NULL){
                    node->Node = NULL;
                }
                else{
                    temp = createNode(_ID,curr->t);
                    curr->t = NULL;
                    node->Node = addSibling(node->Node,temp);
                }

                curr = curr->Right;
                node->Node = addSibling(node->Node,curr->Node);

                break;
            
            case (short)((NTN3<<3)|(2)):
                node->Node = NULL;

                break;
            
            case (short)((NTEXPRESSION<<3)|(1)):
                curr = node->First;
                node->Node = addSibling(node->Node,curr->Node);

                break;
            
            case (short)((NTEXPRESSION<<3)|(2)):
                curr = node->First;
                temp = createNode(_U,NULL);
                addChild(temp,curr->Node);
                node->Node = addSibling(node->Node,temp);

                break;
            
            case (short)((NTU<<3)|(1)):
                curr = node->First;
                node->Node = addSibling(node->Node,curr->Node);

                curr = curr->Right;
                node->Node = addSibling(node->Node,curr->Node);

                break;
            
            case (short)((NTNEW_NT<<3)|(2)):
                curr = node->First;
                node->Node = addSibling(node->Node,curr->Node);

                break;
            
            case (short)((NTNEW_NT<<3)|(1)):
                curr = node->First->Right;
                node->Node = addSibling(node->Node,curr->Node);

                break;
            
            case (short)((NTUNARY_OP<<3)|(1)):
                curr = node->First;
                temp = createNode(_PLUS,NULL);
                node->Node = addSibling(node->Node,temp);

                break;
            
            case (short)((NTUNARY_OP<<3)|(2)):
                curr = node->First;
                temp = createNode(_MINUS,NULL);
                node->Node = addSibling(node->Node,temp);

                break;

            case (short)((NTARITHMETICORBOOLEANEXPR<<3)|(1)):
                curr = node->First->Right;
                node->Node = addSibling(node->Node,curr->Node);
                //printf("%d\n",curr->Node->tag);

                break;
            
            case (short)((NTN7<<3)|(1)):
                curr = node->First->Right->Right;
                node->Node = addSibling(node->Node,curr->Node);

                break;
            
            case (short)((NTN7<<3)|(2)):
                node->Node = addSibling(node->Node,node->inh);

                break;
            
            case (short)((NTANYTERM<<3)|(1)):
                curr = node->First->Right;
                node->Node = addSibling(node->Node,curr->Node);

                break;
            
            case (short)((NTANYTERM<<3)|(2)):
                curr = node->First;
                node->Node = addSibling(node->Node,curr->Node);

                break;
            
            case (short)((NTN8<<3)|(1)):
                curr = node->First->Right;
                node->Node = addSibling(node->Node,curr->inh);
                addChild(node->Node,curr->Node);

                break;

            case (short)((NTN8<<3)|(2)):
                node->Node = addSibling(node->Node,node->inh);

                break;
            
            case (short)((NTARITHMETICEXPR<<3)|(1)):
                curr = node->First->Right;
                node->Node = addSibling(node->Node,curr->Node);

                break;
            
            case (short)((NTN4<<3)|(1)):
                curr = node->First->Right->Right;
                node->Node = addSibling(node->Node,curr->Node);

                break;
            
            case (short)((NTN4<<3)|(2)):
                node->Node = addSibling(node->Node,node->inh);

                break;
            
            case (short)((NTTERM<<3)|(1)):
                curr = node->First->Right;
                node->Node = addSibling(node->Node,curr->Node);

                break;
            
            case (short)((NTN5<<3)|(1)):
                curr = node->First->Right->Right;
                node->Node = addSibling(node->Node,curr->Node);

                break;
            
            case (short)((NTN5<<3)|(2)):
                node->Node = addSibling(node->Node,node->inh);

                break;

            case (short)((NTFACTOR<<3)|(1)):
                curr = node->First->Right;
                node->Node = addSibling(node->Node,curr->Node);

                break;
            
            case (short)((NTFACTOR<<3)|(2)):
                curr = node->First;
                node->Node = addSibling(node->Node,curr->Node);

                break;

            case (short)((NTOP1<<3)|(1)):
                temp = createNode(_PLUS,NULL);
                node->Node = addSibling(node->Node,temp);

                break;
            
            case (short)((NTOP1<<3)|(2)):
                temp = createNode(_MINUS,NULL);
                node->Node = addSibling(node->Node,temp);

                break;
            
            case (short)((NTOP2<<3)|(1)):
                temp = createNode(_MUL,NULL);
                node->Node = addSibling(node->Node,temp);

                break;
            
            case (short)((NTOP2<<3)|(2)):
                temp = createNode(_DIV,NULL);
                node->Node = addSibling(node->Node,temp);

                break;

            case (short)((NTLOGICALOP<<3)|(1)):
                temp = createNode(_AND,NULL);
                node->Node = addSibling(node->Node,temp);

                break;
            
            case (short)((NTLOGICALOP<<3)|(2)):
                temp = createNode(_OR,NULL);
                node->Node = addSibling(node->Node,temp);

                break;
            
            case (short)((NTRELATIONALOP<<3)|(1)):
                temp = createNode(_LT,NULL);
                node->Node = addSibling(node->Node,temp);

                break;
            
            case (short)((NTRELATIONALOP<<3)|(2)):
                temp = createNode(_LE,NULL);
                node->Node = addSibling(node->Node,temp);

                break;
            
            case (short)((NTRELATIONALOP<<3)|(3)):
                temp = createNode(_GT,NULL);
                node->Node = addSibling(node->Node,temp);

                break;
            
            case (short)((NTRELATIONALOP<<3)|(4)):
                temp = createNode(_GE,NULL);
                node->Node = addSibling(node->Node,temp);

                break;
            
            case (short)((NTRELATIONALOP<<3)|(5)):
                temp = createNode(_EQ,NULL);
                node->Node = addSibling(node->Node,temp);

                break;
            
            case (short)((NTRELATIONALOP<<3)|(6)):
                temp = createNode(_NE,NULL);
                node->Node = addSibling(node->Node,temp);

                break;

            case (short)((NTDECLARESTMT<<3)|(1)):
                curr = node->First->Right;
                temp = createNode(_IDLIST,NULL);
                addChild(temp,curr->Node);
                node->Node = addSibling(node->Node,temp);

                curr = curr->Right->Right;
                node->Node = addSibling(node->Node,curr->Node);

                break;
            
            case (short)((NTCONDITIONALSTMT<<3)|(1)):
                curr = node->First->Right->Right;
                if(curr->t==NULL){
                    node->Node = NULL;
                    break;
                }
                else{
                    temp = createNode(_ID,curr->t);
                    curr->t=NULL;
                    node->Node = addSibling(node->Node,temp);
                }

                curr = curr->Right->Right;
                start = curr->t->line_no;
                
                curr = curr->Right;
                node->Node = addSibling(node->Node,curr->Node);

                curr = curr->Right;
                temp = createNode(_DEFAULT,NULL);
                addChild(temp,curr->Node);
                node->Node = addSibling(node->Node,temp);

                curr = curr->Right;
                end = curr->t->line_no;

                node->Node->scope[0] = start;
                node->Node->scope[1] = end;

                break;

            case (short)((NTCASESTMTS<<3)|(1)):
                curr = node->First->Right;
                if(curr->Node==NULL){
                    //No use of caseStmt without value.NTIND
                    node->Node = NULL;
                    //break;
                }
                else{
                    node->Node = addSibling(node->Node,curr->Node);    
                }

                curr = curr->Right->Right;
                temp = createNode(_STATEMENTS,NULL);
                addChild(temp,curr->Node);
                node->Node = addSibling(node->Node,temp);

                curr = curr->Right->Right->Right;
                node->Node = addSibling(node->Node,curr->Node);

                break;

            case (short)((NTN9<<3)|(1)):
                curr = node->First->Right;
                if(curr->Node==NULL){
                    //No use of caseStmt without value.
                    node->Node = NULL;
                    //break;
                }
                else{
                    node->Node = addSibling(node->Node,curr->Node);    
                }

                curr = curr->Right->Right;
                temp = createNode(_STATEMENTS,NULL);
                addChild(temp,curr->Node);
                node->Node = addSibling(node->Node,temp);

                curr = curr->Right->Right->Right;
                node->Node = addSibling(node->Node,curr->Node);

                break;

            case (short)((NTN9<<3)|(2)):
                node->Node = NULL;

                break;

            case (short)((NTVALUE<<3)|(1)):
                curr = node->First;
                if(curr->t==NULL){
                    node->Node = NULL;
                    break;
                }
                else{
                    temp = createNode(_NUM,curr->t);
                    curr->t = NULL;
                    node->Node = addSibling(node->Node,temp);
                }

                break;

            case (short)((NTVALUE<<3)|(2)):
                curr = node->First;
                temp = createNode(_TRUE,curr->t);
                curr->t = NULL;
                node->Node = addSibling(node->Node,temp);

                break;

            case (short)((NTVALUE<<3)|(3)):
                curr = node->First;
                temp = createNode(_FALSE,curr->t);
                curr->t = NULL;
                node->Node = addSibling(node->Node,temp);

                break;

            case (short)((NTDEFAULT<<3)|(1)):
                curr = node->First->Right->Right;
                temp = createNode(_STATEMENTS,NULL);
                addChild(temp,curr->Node);
                node->Node = addSibling(node->Node,temp);

                //DEFAULT is the only other node under which there is a list of statements.
                break;
            
            case (short)((NTDEFAULT<<3)|(2)):
                node->Node = NULL;

                break;
            
            case (short)((NTITERATIVESTMT<<3)|(1)):
                //end in this case is being used for the children of node->First.
                temp = createNode(_FOR,NULL);
                node->Node = addSibling(node->Node,temp);

                curr = node->First->Right->Right;
                if(curr->t==NULL){
                    addChild(node->Node,NULL);
                }
                else{
                    temp = createNode(_ID,curr->t);
                    curr->t = NULL;
                    addChild(node->Node,temp);
                }

                curr = curr->Right->Right;
                if(curr->Node==NULL){
                    addChild(node->Node,NULL);
                }
                else{
                    addChild(node->Node,curr->Node);
                }

                curr = curr->Right->Right;
                start = curr->t->line_no;

                curr = curr->Right;
                temp = createNode(_STATEMENTS,NULL);
                addChild(temp,curr->Node);
                addChild(node->Node,temp);

                curr = curr->Right;
                end = curr->t->line_no;

                node->Node->scope[0] = start;
                node->Node->scope[1] = end;

                break;

            case (short)((NTITERATIVESTMT<<3)|(2)):
                //end in this case is being used for the children of node->First.
                temp = createNode(_WHILE,NULL);
                node->Node = addSibling(node->Node,temp);

                curr = node->First->Right->Right;
                if(curr->Node==NULL){
                    addChild(node->Node,NULL);
                }
                else{
                    addChild(node->Node,curr->Node);
                }

                curr = curr->Right->Right;
                start = curr->t->line_no;

                curr = curr->Right;
                temp = createNode(_STATEMENTS,NULL);
                addChild(temp,curr->Node);
                addChild(node->Node,temp);

                curr = curr->Right;
                end = curr->t->line_no;

                node->Node->scope[0] = start;
                node->Node->scope[1] = end;

                break;
                            
            case (short)((NTRANGE<<3)|(1)):
                curr = node->First;
                if(curr->t==NULL){
                    node->Node = NULL;
                    break;
                }
                else{
                    temp = createNode(_NUM,curr->t);
                    curr->t=NULL;
                    node->Node = addSibling(node->Node,temp);
                }

                curr = curr->Right->Right;
                if(curr->t==NULL){
                    node->Node = NULL;
                    break;
                }
                else{
                    temp = createNode(_NUM,curr->t);
                    curr->t=NULL;
                    node->Node = addSibling(node->Node,temp);
                }

                break;
                
        }
        freeList(node->First);
}
