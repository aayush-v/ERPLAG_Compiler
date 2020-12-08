// Group Number - 25
// Name : Aayush Atul Verma         ID : 2017A7PS0061P
// Name : Aditya Ramaswamy          ID : 2017A7PS0130P
// Name : Jithin Kallukalam Sojan   ID : 2017A7PS0163P


#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include<ctype.h>
#include <stdbool.h>
#include "codeGen.h"
#include "Semantic-Accessory.h"

IrCode* codeTree(astNode* Root){
    short a = 10;
    short *label = &a;

    Root->False = *label;
    *label = *label+1;

    TravModLabel(Root->First,label);

    IrCode* program = programGen(Root,label);

    return program;
}

void TravModLabel(astNode* node,short int *label){
    astNode* curr = node;
    astNode* temp = NULL;

    while(curr){
        switch(curr->tag){
            case _MODULE:
                temp = curr->First;
                while(temp->tag!=_STATEMENTS)temp = temp->Right;
                labelStmts(temp,label);
                break;

            case _DRIVERMODULE:
                labelStmts(curr->First,label);
                break;
                
        }
        curr = curr->Right;
    }
}

void labelStmts(astNode* node, short int* label){
    //If curr->False exists, we make the label. If current->True exists, it will be made or is already made.

    node->True = *label;
    *label+=1;
    //True at statements node means the beginning of the statements.
    //False at statemenets node mean the last of the statements.

    astNode* curr = node->First;
    astNode* temp = NULL;
    if(curr==NULL)return;

    while(curr){
        switch(curr->tag){
            case _FOR:
                labelStmts(curr->First->Right->Right->Right,label);
                curr->True = curr->First->Right->Right->Right->True;
                curr->False = *label;
                *label = *label + 1;
                break;
                //curr->False in that movie    

            case _WHILE:
                labelStmts(curr->First->Right,label);
                curr->True = curr->First->Right->True;
                curr->False = *label;
                *label = *label+1;
                break;
            
            case _CONDITIONALSTMT:
                curr->False = *label;
                *label+=1;
                temp = curr->First->Right;
                temp->True = *label;
                *label+=1;
                while(temp->tag!=_DEFAULT){
                    labelStmts(temp->Right,label);
                    temp->False = *label;
                    *label+=1;

                    temp->Right->Right->True = temp->False;

                    temp = temp->Right->Right;
                }

                if(temp->tag==_DEFAULT){
                    if(temp->First){
                        labelStmts(temp->First,label);
                    }
                }
                break;
            
        }
        curr = curr->Right;
    }
}

IrCode* createInst(){
    IrCode* new = (IrCode*)malloc(sizeof(IrCode));
    new->label = 0;
    new->Op = ONULLOP;
    new->tag1 = 0;
    new->tag2 = 0;
    new->tag3 = 0;
    new->jump = 0;
    new->next = NULL;
    return new;
}

IrCode* addInstr(IrCode* a,IrCode* b){
    if(!a)return b;
    if(!b)return a;
    IrCode* first = a;
    while(a->next)a = a->next;
    a->next = b;
    return first;
}

IrCode* modGen(astNode* node,short int* label){
    IrCode* first = NULL;
    IrCode* prev = NULL;
    IrCode* curr = NULL;

    curr = createInst();
    first = curr;
    curr->Op = CREATE;
    curr->tag1 = 1;
    curr->op1.a = node->First;
    prev = curr;

    prev->next = createInst();
    curr = prev->next;

    curr->Op = MOVQ;
    curr->tag1 = 3;
    curr->op1.R = RBP;

    curr->tag2 = 3;
    curr->op2.R = RSP;
    prev = curr;

    prev->next = createInst();
    curr = prev->next;

    curr->Op = OSUBQ;
    curr->tag1 = 3;
    curr->op1.R = RSP;

    curr->tag2 = 3;
    curr->op2.R = RSP;

    curr->tag3 = 6;
    curr->op3.offset = node->First->entry->derive->size;
    prev = curr;

    if(node->First->Right->Right->tag==_OUTPUT_PLIST){
        prev->next = stmtsGen(node->First->Right->Right->Right,label);
    }
    else{
        prev->next = stmtsGen(node->First->Right->Right,label);
    }
    while(prev->next)prev = prev->next;

    prev->next = createInst();
    curr = prev->next;

    curr->Op = MOVQ;
    curr->tag1 = 3;
    curr->op1.R = RSP;

    curr->tag2 = 3;
    curr->op2.R = RBP;
    prev = curr;

    prev->next = createInst();
    curr = prev->next;

    curr->Op = RET;
    prev = curr;

    return first;
}

IrCode* driverGen(astNode* node, short int* label){
    astNode* line = node;
    while(line->Parent)line = line->Parent;
    short target = line->False;

    IrCode* first = NULL;
    IrCode* prev = NULL;
    IrCode* curr = NULL;

    curr = createInst();
    first = curr;
    curr->Op = ODRIVER;
    prev = curr;

    prev->next = createInst();
    curr = prev->next;

    curr->Op = MOVQ;
    curr->tag1 = 3;
    curr->op1.R = RBP;

    curr->tag2 = 3;
    curr->op2.R = RSP;
    prev = curr;

    prev->next = createInst();
    curr = prev->next;

    curr->Op = OSUBQ;
    curr->tag1 = 3;
    curr->op1.R = RSP;

    curr->tag2 = 3;
    curr->op2.R = RSP;

    curr->tag3 = 6;
    curr->op3.offset = node->entry->derive->size;    
    prev = curr;

    prev->next = stmtsGen(node->First,label);
    while(prev->next)prev = prev->next;

    prev->next = createInst();
    curr = prev->next;

    curr->Op = DRIVERRET;
    //Will this work??
    prev = curr;

    return first;
}

IrCode* expressionCreate(astNode* node, short int* label){
    IrCode* i1 = NULL;
    IrCode* i2 = NULL;
    IrCode* new = NULL;
    IrCode* curr = NULL;
    IrCode* prev = NULL;
    IrCode* first = NULL;

    astNode* line = node;
    while(line->Parent)line = line->Parent;
    short target = line->False;

    int check = 0;
    int dynCheck = 0;
    if(node->tag==_PLUS || node->tag==_MINUS ||node->tag==_MUL ||node->tag==_DIV ||node->tag==_LT ||node->tag==_LE ||node->tag==_GT ||node->tag==_GE ||node->tag==_EQ ||node->tag==_NE){
            i1 = expressionCreate(node->First,label);
            i2 = expressionCreate(node->First->Right,label);
            new = createInst();
            new->label = 0;
            new->tag1 = 1;
            new->op1.a = node;
            check = getType(node->First);
            if(check==1){
                if(!node->First->entry){
                    new->tag2 = 2;
                    new->op2.num = (int)node->First->t->value.integer;
                }
                else{
                    new->tag2 = 1;
                    new->op2.a = node->First;
                }
                if(!node->First->Right->entry){
                    new->tag3 = 2;
                    new->op3.num = (int)node->First->Right->t->value.integer;
                }
                else{
                    new->tag3 = 1;
                    new->op3.a = node->First->Right;
                }
            }
            else if(check==2){
                if(!node->First->entry){
                    new->tag2 = 2;
                    new->op2.real = (double)node->First->t->value.real;
                }
                else{
                    new->tag2 = 1;
                    new->op2.a = node->First;
                }
                if(!node->First->Right->entry){
                    new->tag3 = 2;
                    new->op3.real = (double)node->First->Right->t->value.real;
                }
                else{
                    new->tag3 = 1;
                    new->op3.a = node->First->Right;
                }
            }
            else{
                printf("Not supposed to happen.");
            }

            switch(node->tag){
                case _PLUS:
                    if(check==1){
                        new->Op = OADD;
                    }
                    else{
                        new->Op = OFADD;
                    }
                    break;
                case _MINUS:
                    if(check==1){
                        new->Op = OSUB;
                    }
                    else{
                        new->Op = OFSUB;
                    }
                    break;
                case _MUL:
                    if(check==1){
                        new->Op = OMUL;
                    }
                    else{
                        new->Op = OFMUL;
                    }
                    break;
                case _DIV:
                    ;
                    astNode* a1 = node;
                    while(a1->First)a1 = a1->First;
                    curr = createInst();
                    first = curr;

                    curr->Op = JNEQ;
                    if(node->First->Right->tag==_ID){
                        curr->tag1 = 1;
                        curr->op1.a = node->First->Right;
                    }
                    else{
                        curr->tag1 = 2;
                        if(check==1){
                            curr->op1.num=(int)node->First->Right->t->value.integer;
                        }
                        else{
                            curr->op1.real = node->First->Right->t->value.real;
                        }
                    }
                    if(check==1){
                        new->Op = ODIV;

                        curr->tag2 = 2;
                        curr->op2.num = 0;
                    }
                    else{
                        new->Op = OFDIV;

                        curr->tag2 = 2;
                        curr->op2.real = 0;
                    }
                    curr->jump = *label;
                    *label+=1;
                    prev = curr;

                    prev->next = createInst();
                    curr = prev->next;

                    curr->Op = ZEROERROR;

                    curr->tag1 = 2;
                    curr->op1.num = a1->t->line_no;
                    //Remember that division by zero error does not have line number.
                    //curr->op1.num = node->First->Right->t->line_no;
                    prev = curr;

                    prev->next = createInst();
                    curr = prev->next;

                    curr->Op = EXIT;
                    prev = curr;

                    new->label = *label-1;
                    break;
                case _LT:
                    if(check==1){
                        new->Op = OLT;
                    }
                    else{
                        new->Op = OFLT;
                    }
                    break;
                case _LE:
                    if(check==1){
                        new->Op = OLE;
                    }
                    else{
                        new->Op = OFLE;
                    }
                    break;
                case _GT:
                    if(check==1){
                        new->Op = OGT;
                    }
                    else{
                        new->Op = OFGT;
                    }
                    break;
                case _GE:
                    if(check==1){
                        new->Op = OGE;
                    }
                    else{
                        new->Op = OFGE;
                    }
                    break;
                case _EQ:
                    if(check==1){
                        new->Op = OEQ;
                    }
                    else{
                        new->Op = OFEQ;
                    }
                    break;
                case _NE:
                    if(check==1){
                        new->Op = ONE;
                    }
                    else{
                        new->Op = OFNE;
                    }
                    break;
            }

            if(first!=NULL)new = addInstr(first,new);
            i2 = addInstr(i2,new);
            i1 = addInstr(i1,i2);


    }
    else if(node->tag == _AND || node->tag==_OR){
            i1 = expressionCreate(node->First,label);
            i2 = expressionCreate(node->First->Right,label);
            new = createInst();
            new->label = 0;
            new->tag1 = 1;
            new->op1.a = node;
            
                if(!node->First->entry){
                    new->tag2 = 2;
                    if(node->First->tag==_TRUE)
                    new->op2.boolVal = true;
                    else new->op2.boolVal = false;
                }
                else{
                    new->tag2 = 1;
                    new->op2.a = node->First;
                }
                if(!node->First->Right->entry){
                    new->tag3 = 2;
                    if(node->First->Right->tag==_TRUE)
                    new->op3.boolVal = true;
                    else new->op3.boolVal = false; 
                }
                else{
                    new->tag3 = 1;
                    new->op3.a = node->First->Right;
                }

            switch(node->tag){
                case _AND:
                    new->Op = OAND;
                    break;
                case _OR:
                    new->Op = OOR;
                    break;
            }

            i2 = addInstr(i2,new);
            i1 = addInstr(i1,i2);
    }
    else{
        //This has to be one of the leaf nodes.
        if(node->First){
            if(node->entry->isStatic==0 || node->First->tag==_ID){
                i1 = dynamicCheck(node,label);
            }
        }
        else{
            return NULL;    
        }
    }

    return i1;
}

IrCode* dynamicCheck(astNode* node, short int* label){
    IrCode* i1;
    IrCode* i2;
    IrCode* i3;
    IrCode* i4;
    IrCode* i5;
    IrCode* i6;
    IrCode* i7;
    astNode* l = node;
    while(l->Parent)l = l->Parent;
    short target = l->False;
    //The end of the code.

        i1 = createInst();
        i1->Op = JGE;
        //op1 greater than op2 means jump
        if(node->First->tag==_ID){
            i1->tag1 = 1;
            i1->op1.a = node->First;
        }
        else{
            i1->tag1 = 2;
            i1->op1.num = (int)node->First->t->value.integer;
        }

        if(node->entry->type->First->Right->tag==_ID){
            // i1->op2.offset = -(node->entry->offset + SIZE_OF_POINTER + SIZE_OF_INTEGER);
            i1->tag2 = 1;
            i1->op2.a = node->entry->type->First->Right;
            //mostly will have to add SIZE_OF_INTEGER as well.
            //Check how values are placed in the stack.
        }
        else{
            i1->tag2 = 2;
            i1->op2.num = (int)node->entry->type->First->Right->t->value.integer;
        }

        i1->jump = *label;
        *label+=1;

        i2 = createInst();
        i2->Op = PRINTERROR1;

        i2->tag1 = 2;
        i2->op1.num = (int)node->First->t->line_no;
        //For the line number while printing error.

        i1->next = i2;

        i3 = createInst();
        i3->Op = EXIT;

        i2->next = i3;

        i4 = createInst();
        i4->label = *label-1;
        i4->Op = JLE;

        if(node->First->tag==_ID){
            i4->tag1 = 1;
            i4->op1.a = node->First;
        }
        else{
            i4->tag1 = 2;
            i4->op1.num = (int)node->First->t->value.integer;
        }

        if(node->entry->type->First->Right->Right->tag==_ID){
            // i4->op2.offset = -(node->entry->offset + SIZE_OF_POINTER + SIZE_OF_INTEGER + SIZE_OF_INTEGER);
            i4->tag2 = 1;
            i4->op2.a = node->entry->type->First->Right->Right;
            //mostly will have to add SIZE_OF_INTEGER as well.
        }
        else{
            i4->tag2 = 2;
            i4->op2.num = (int)node->entry->type->First->Right->Right->t->value.integer;
        }

        i4->jump = *label;
        *label+=1;

        i3->next = i4;

        i5 = createInst();
        i5->Op = PRINTERROR1;

        i5->tag1 = 2;
        i5->op1.num = (int)node->First->t->line_no;

        i4->next = i5;
        
        i6 = createInst();
        i6->Op = EXIT;

        i5->next = i6;

        i7 = createInst();
        i7->label = *label-1;
        i7->Op = ONULLOP;

        i6->next = i7;

        return i1;

}

IrCode* assignment(astNode* node, short int* label){
    int check1 = getType(node->First);
    int tag = 0;
    int tag2 = 0;
    astNode* l = node;
    while(l->Parent)l = l->Parent;
    short target = l->False;
    IrCode* i1;
    IrCode* i2;
    IrCode* i3;
    IrCode* curr = NULL;
    IrCode* prev = NULL;
    IrCode* dynCheck;
    IrCode* expr = NULL;
    dynCheck = NULL;

    if(check1>=4){
        if(!node->First->entry->isStatic || !node->First->Right->entry->isStatic){
            tag = 1;
            i1 = createInst();
            i1->Op = JEQ;
            if(node->First->entry->type->First->Right->tag==_ID){
                i1->tag1 = 1;
                i1->op1.a = node->First->entry->type->First->Right;
            }
            else{
                i1->tag1 = 2;
                i1->op1.num = (int)node->First->entry->type->First->Right->t->value.integer;
            }
            if(node->First->Right->entry->type->First->Right->tag==_ID){
                i1->tag2 = 1;
                i1->op2.a = node->First->Right->entry->type->First->Right;
            }
            else{
                i1->tag2 = 2;
                i1->op2.num = (int)node->First->Right->entry->type->First->Right->t->value.integer;
            }

            i1->jump = *label;
            *label+=1;

            curr = createInst();
            curr->Op = PRINTERROR2;
            curr->tag1 = 2;
            curr->op1.num = node->First->t->line_no;
            prev = curr;
            i1->next = curr;

            prev->next = createInst();
            curr = prev->next;
            curr->Op = EXIT;
            prev = curr;

            i2 = createInst();
            prev->next = i2;
            i2->label = *label-1;
            i2->Op = JEQ;
            if(node->First->entry->type->First->Right->Right->tag==_ID){
                i2->tag1 = 1;
                i2->op1.a = node->First->entry->type->First->Right->Right;
            }
            else{
                i2->tag1 = 2;
                i2->op1.num = (int)node->First->entry->type->First->Right->Right->t->value.integer;
            }
            if(node->First->Right->entry->type->First->Right->Right->tag==_ID){
                i2->tag2 = 1;
                i2->op2.a = node->First->Right->entry->type->First->Right->Right;
            }
            else{
                i2->tag2 = 2;
                i2->op2.num = (int)node->First->Right->entry->type->First->Right->Right->t->value.integer;
            }

            i2->jump = *label;
            *label+=1;

            curr = createInst();
            curr->Op = PRINTERROR2;
            curr->tag1 = 2;
            curr->op1.num = node->First->t->line_no;
            prev = curr;
            i2->next = curr;

            prev->next = createInst();
            curr = prev->next;
            curr->Op = EXIT;
            prev = curr;

            prev->next = createInst();
            curr = prev->next;
            curr->label = *label-1;
            prev = curr;
            
        }

        i3 = createInst();
        i3->Op = MOVQ;
        i3->tag1 = 5;
        i3->op1.offset = -(node->First->entry->offset + SIZE_OF_POINTER);

        if(node->First->Right->entry->io==1){
            i3->tag2 = 5;
            i3->op2.offset = (SIZE_AFTER_BP + node->First->Right->entry->offset);
        }
        else{
            i3->tag2 = 5;
            i3->op2.offset = -(node->First->Right->entry->offset + SIZE_OF_POINTER);
        }

        if(tag==1){
            prev->next = i3;
            return i1;
        }
        else{
            return i3;
        }   
    }
    else{
        if(node->First->First){
            tag2 = 1;
            dynCheck = dynamicCheck(node->First,label);
        }        

        if(node->First->Right->tag==_U){
            tag=1;
            i1 = createInst();
            if(check1==1){
                if(node->First->Right->First->tag==_PLUS){
                    i1->Op = OADD;
                }
                else{
                    i1->Op = OSUB;
                }
                i1->tag1 = 1;
                i1->op1.a = node->First;

                i1->tag2 = 2;
                i1->op2.num = 0;
                
                
                if(node->First->Right->First->Right->entry==NULL){
                    i1->tag3 = 2;
                    if(node->First->Right->First->Right->tag==_NUM){
                        i1->op3.num = (int)node->First->Right->First->Right->t->value.integer;
                    }
                    else{
                        printf("This code should not execute.\n");
                    }
                }
                else{
                    i1->tag3 = 1;
                    i1->op3.a = node->First->Right->First->Right;
                }

            }
            else{
                if(node->First->Right->First->tag==_PLUS){
                    i1->Op = OFADD;
                }
                else{
                    i1->Op = OFSUB;
                }
                i1->tag1 = 1;
                i1->op1.a = node->First;

                i1->tag2 = 2;
                i1->op2.real = 0;

                if(node->First->Right->First->Right->entry==NULL){
                    i1->tag3 = 2;
                    if(node->First->Right->First->Right->tag==_RNUM){
                        i1->op3.real = node->First->Right->First->Right->t->value.real;
                    }
                    else{
                        printf("This code should not execute.\n");
                    }
                }
                else{
                    i1->tag3 = 1;
                    i1->op3.a = node->First->Right->First->Right;
                }
            }

            expr = expressionCreate(node->First->Right->First->Right,label);
            expr = addInstr(expr,i1);
        }
        else{
            i1 = createInst();
            if(check1==1){
                i1->Op=MOVD;
            }
            else if(check1==2){
                i1->Op=MOVQ;
            }
            else{
                i1->Op=MOVW;
            }
            i1->tag1 = 1;
            i1->op1.a = node->First;

            if(node->First->Right->entry==NULL){
                i1->tag2 = 2;
                if(node->First->Right->tag==_NUM){
                    i1->op2.num = (int)node->First->Right->t->value.integer;
                }
                else if(node->First->Right->tag==_RNUM){
                    i1->op2.real = node->First->Right->t->value.real;
                }
                else if(node->First->Right->tag==_TRUE){
                    i1->op2.boolVal = 1;
                }
                else if(node->First->Right->tag==_FALSE){
                    i1->op2.boolVal = 0;
                }
                else{
                    printf("This is not supposed to happen.\n");
                }
            }
            else{
                i1->tag2 = 1;
                i1->op2.a = node->First->Right;
            }

            expr = expressionCreate(node->First->Right,label);
            expr = addInstr(expr,i1);
        }

        if(tag2==1){
            dynCheck = addInstr(dynCheck,expr);
            return dynCheck;
        }
        else{
            return expr;
        }
    }
}

IrCode* declArray(astNode* node, short int* label){
    //Write code for dynamic checks in the decl array. Write a different function. It will be needed for function call as well.

    astNode* temp = node->First->First;
    astNode* type = node->First->Right;
    int isStatic = 1;
    if(!temp->entry->isStatic)isStatic = 0;

    IrCode* first = NULL;
    IrCode* prev = NULL;
    IrCode* curr = NULL;

    if(!isStatic){
        first = dynamicArrayCheck(type,label);
        prev = first;
        while(prev->next)prev = prev->next;
    }

    while(temp){
            if(prev != NULL){
                prev->next = createInst();
                curr = prev->next;
            }
            else curr = createInst();

            if(isStatic){
                curr->Op = OSUBQ;
                curr->tag1 = 5;
                curr->op1.offset = -(temp->entry->offset + SIZE_OF_POINTER);

                curr->tag2 = 3;
                curr->op2.R = RBP;

                curr->tag3 = 6;
                curr->op3.offset = temp->entry->offset + SIZE_OF_POINTER + 2 * SIZE_OF_INTEGER;
            }
            else{
                curr->Op = MOVQ;
                curr->tag1 = 5;
                curr->op1.offset = -(temp->entry->offset + SIZE_OF_POINTER);

                curr->tag2 = 3;
                curr->op2.R = RSP;
            }

            if(first==NULL){
                first = curr;
            }
            prev = curr;

            // if(type->First->Right->tag==_ID){
            //     curr->Op = MOV;
            //     curr->tag1 = 5;
            //     curr->op1.offset = -(temp->entry->offset + SIZE_OF_POINTER + SIZE_OF_INTEGER);

            //     curr->tag2 = 1;
            //     curr->op2.a = type->First->Right;
            // }

            if(isStatic){
                prev->next = createInst();
                curr = prev->next;

                curr->Op = MOVD;
                curr->tag1 = 5;
                curr->op1.offset = -(temp->entry->offset + SIZE_OF_POINTER + SIZE_OF_INTEGER);

                curr->tag2 = 2;
                curr->op2.num = (int)type->First->Right->t->value.integer; 

                prev = curr;
            }

            if(isStatic){
                prev->next = createInst();
                curr = prev->next;

                curr->Op = MOVD;
                curr->tag1 = 5;
                curr->op1.offset = -(temp->entry->offset + SIZE_OF_POINTER + 2 * SIZE_OF_INTEGER);

                curr->tag2 = 2;
                curr->op2.num = (int)type->First->Right->Right->t->value.integer;

                prev = curr;
            }

            if(!isStatic){
                prev->next = createInst();
                curr = prev->next;

                curr->Op = MOVQ;
                curr->tag1 = 3;
                curr->op1.R = RDX;

                curr->tag2 = 2;
                curr->op2.num = 0;
                prev = curr;

                prev->next = createInst();
                curr = prev->next;

                curr->Op = OSUB;
                curr->tag1 = 3;
                curr->op1.R = EDX;

                if(type->First->Right->Right->tag==_ID){
                    curr->tag2 = 1;
                    curr->op2.a = type->First->Right->Right;
                }
                else{
                    curr->tag2 = 2;
                    curr->op2.num = (int)type->First->Right->Right->t->value.integer;
                }

                if(type->First->Right->tag==_ID){
                    curr->tag3 = 1;
                    curr->op3.a = type->First->Right;
                }
                else{
                    curr->tag3 = 2;
                    curr->op3.num = (int)type->First->Right->t->value.integer;
                }
                prev = curr;

                prev->next = createInst();
                curr = prev->next;

                curr->Op = OADD;
                curr->tag1 = 3;
                curr->op1.R = EDX;

                curr->tag2 = 3;
                curr->op2.R = EDX;

                curr->tag3 = 2;
                curr->op3.num = 1;
                prev = curr;

                prev->next = createInst();
                curr = prev->next;

                curr->Op = OMUL;
                curr->tag1 = 3;
                curr->op1.R = EDX;

                curr->tag2 = 3;
                curr->op2.R = EDX;

                curr->tag3 = 6;
                if(type->First->tag==_INTEGER){
                    curr->op3.offset = SIZE_OF_INTEGER;
                }
                else if(type->First->tag==_REAL){
                    curr->op3.offset = SIZE_OF_REAL;
                }
                else{
                    curr->op3.offset = SIZE_OF_BOOLEAN;
                }
                prev = curr;

                prev->next = createInst();
                curr = prev->next;

                curr->Op = OSUBQ;
                curr->tag1 = 3;
                curr->op1.R  = RSP;

                curr->tag2 = 3;
                curr->op2.R = RSP;

                curr->tag3 = 3;
                curr->op3.R = RDX;

                prev = curr;
            }

        temp = temp->Right;
    }
    return first;
}

IrCode* forStmt(astNode* node, short int* label){
    IrCode* first = NULL;
    IrCode* curr = NULL;
    IrCode* prev = NULL;

    curr = createInst();
    first = curr;
    curr->Op = MOVD;
    curr->tag1 = 1;
    curr->op1.a = node->First;

    curr->tag2 = 2;
    curr->op2.num = (int)node->First->Right->t->value.integer;
    prev = curr;

    prev->next = createInst();
    curr = prev->next;

    curr->label = node->True;
    curr->Op = JG;

    curr->tag1 = 1;
    curr->op1.a = node->First;

    curr->tag2 = 2;
    curr->op2.num = (int)node->First->Right->Right->t->value.integer;

    curr->jump = node->False;
    prev = curr;

    prev->next = stmtsGen(node->First->Right->Right->Right,label);
    while(prev->next)prev = prev->next;

    prev->next = createInst();
    curr = prev->next;

    curr->Op = OADD;
    curr->tag1 = 1;
    curr->op1.a = node->First;

    curr->tag2 = 1;
    curr->op2.a = node->First;

    curr->tag3 = 2;
    curr->op3.num = (int)1;
    prev = curr;

    prev->next = createInst();
    curr = prev->next;

    curr->Op = JMP;
    curr->jump = node->True; 
    prev = curr;

    prev->next = createInst();
    curr = prev->next;

    curr->label = node->False;

    return first;
}

IrCode* whileStmt(astNode* node, short int* label){
    IrCode* first = NULL;
    IrCode* curr = NULL;
    IrCode* prev = NULL;
  
    first = expressionCreate(node->First,label);
    if(first==NULL){
        first = createInst();
        first->label = node->True;
    }
    else{
        first->label = node->True;
    }
    prev = first;
    while(prev->next)prev = prev->next; 

    prev->next = createInst();
    curr = prev->next;

    curr->Op = JNEQW;
    if(node->First->entry){
        curr->tag1 = 1;
        curr->op1.a = node->First;
    }
    else{
        curr->tag1 = 2;
        if(node->First->tag==_TRUE){
            curr->op1.boolVal = 1;
        }
        else{
            curr->op1.boolVal = 0;
        }
    }

    curr->tag2 = 2;
    curr->op2.boolVal = 1;
    
    curr->jump = node->False;
    prev = curr;

    prev->next = stmtsGen(node->First->Right,label);
    while(prev->next)prev = prev->next;

    prev->next = createInst();
    curr = prev->next;

    curr->Op = JMP;

    curr->jump = node->True;
    prev = curr;

    prev->next = createInst();
    curr = prev->next;

    curr->label = node->False;

    return first;
}

IrCode* caseStmts(astNode* node,short int* label){
    IrCode* first = NULL;
    IrCode* curr = NULL;
    IrCode* prev = NULL;

    astNode* temp = node->First->Right;
    while(temp->tag!=_DEFAULT){
        if(prev==NULL){
            curr = createInst();
            first = curr;
        }
        else{
            prev->next = createInst();
            curr = prev->next;
        }
        
        curr->label = temp->True;
        
        curr->tag1 = 1;
        curr->op1.a = node->First;

        curr->tag2 = 2;
        if(temp->tag==_NUM){
            curr->Op = JNEQ;
            curr->op2.num = (int)temp->t->value.integer;
        }
        else{
            curr->Op = JNEQW;
            if(temp->tag==_TRUE){
                curr->op2.boolVal = 1;
            }
            else{
                curr->op2.boolVal = 0;
            }
        }

        curr->jump = temp->False;
        prev = curr;

        prev->next = stmtsGen(temp->Right,label);
        while(prev->next)prev = prev->next;

        prev->next = createInst();
        curr = prev->next;

        curr->Op = JMP;
        curr->jump = node->False;
        prev = curr;

        temp = temp->Right->Right;
    }

    prev->next = createInst();
    curr = prev->next;

    curr->label = temp->True;
    prev = curr;

    if(temp->First){
        prev->next = stmtsGen(temp->First,label);
        while(prev->next)prev = prev->next;
    }

    prev->next = createInst();
    curr = prev->next;

    curr->label = node->False;

    return first;
}

IrCode* dynamicArrayCheck(astNode* node,short int* label){
    IrCode* first = NULL;
    IrCode* curr = NULL;
    IrCode* prev = NULL;

    astNode* line = node;
    while(line->Parent)line = line->Parent;
    short target = line->False;

    short lineNum = 0;
    if(node->First->Right->tag==_ID){
        lineNum = node->First->Right->t->line_no;
    }
    else{
        lineNum = node->First->Right->Right->t->line_no;
    }

    curr = createInst();
    first = curr;
    curr->Op = JLE;

    curr->tag1 = 2;
    curr->op1.num = 0;

    if(node->First->Right->tag==_ID){
        curr->tag2 = 1;
        curr->op2.a = node->First->Right;
    }
    else{
        curr->tag2 = 2;
        curr->op2.num = (int)node->First->Right->t->value.integer;
    }

    curr->jump = *label;
    *label+=1;
    prev = curr;

    prev->next = createInst();
    curr = prev->next;

    curr->Op = PRINTERROR3;

    curr->tag1 = 2;
    curr->op1.num = lineNum;
    prev = curr;

    prev->next = createInst();
    curr = prev->next;

    curr->Op = EXIT;
    prev = curr;

    prev->next = createInst();
    curr = prev->next;

    curr->label = *label-1;
    curr->Op = JLE;

    curr->tag1 = 2;
    curr->op1.num = 0;

    if(node->First->Right->Right->tag==_ID){
        curr->tag2 = 1;
        curr->op2.a = node->First->Right->Right;
    }
    else{
        curr->tag2 = 2;
        curr->op2.num = (int)node->First->Right->Right->t->value.integer;
    }

    curr->jump = *label;
    *label+=1;
    prev = curr;

    prev->next = createInst();
    curr = prev->next;

    curr->Op = PRINTERROR3;

    curr->tag1 = 2;
    curr->op1.num = lineNum;
    prev = curr;

    prev->next = createInst();
    curr = prev->next;

    curr->Op = EXIT;
    prev = curr;

    prev->next = createInst();
    curr = prev->next;

    curr->label = *label-1;
    curr->Op = JLE;

    if(node->First->Right->tag==_ID){
        curr->tag1 = 1;
        curr->op1.a = node->First->Right;
    }
    else{
        curr->tag1 = 2;
        curr->op1.num = (int)node->First->Right->t->value.integer;
    }

    if(node->First->Right->Right->tag==_ID){
        curr->tag2 = 1;
        curr->op2.a = node->First->Right->Right;
    }
    else{
        curr->tag2 = 2;
        curr->op2.num = (int)node->First->Right->Right->t->value.integer;
    }

    curr->jump = *label;
    *label+=1;
    prev = curr;

    prev->next = createInst();
    curr = prev->next;

    curr->Op = PRINTERROR3;

    curr->tag1 = 2;
    curr->op1.num = lineNum;
    prev = curr;

    prev->next = createInst();
    curr = prev->next;

    curr->Op = EXIT;
    prev = curr;

    prev->next = createInst();
    curr = prev->next;

    curr->label = *label-1;

    return first;
}

IrCode* modReuseStmt(astNode* node, short int* label){
    int outSize = 0;
    int inSize = 0;
    int totalSize = 0;
    int check = 0;

    astNode* line = node;
    while(line->Parent)line = line->Parent;
    short target = line->False;

    if(node->First->tag==_IDLIST){
        outSize = sizeIDList(node->First->First);
        inSize = sizeIDList(node->First->Right->Right->First);
    }
    else{
        inSize = sizeIDList(node->First->Right->First);
    }

    // if(outSize>inSize)totalSize = outSize;
    // else totalSize = inSize;
    totalSize = inSize + outSize;

    IrCode* curr = NULL;
    IrCode* prev = NULL;
    IrCode* first = NULL;

    astNode* temp;
    astNode* temp2;
    int check1 = 0;
    if(node->First->tag==_IDLIST){
        temp = node->First->Right->Right->First;
        temp2 = node->First->Right->entry->type->First;
    }
    else{
        temp = node->First->Right->First;
        temp2 = node->First->entry->type->First;
    }

    int currentOff = 0;
    while(temp){
        if(prev==NULL){
            curr = createInst();
            first = curr;
        }
        else{
            prev->next = createInst();
            curr = prev->next;
        }

        check1 = getType(temp);

        if(check1==1){
            curr->Op = MOVD;
            curr->tag1 = 4;
            // curr->op1.offset = -(currentOff + SIZE_OF_INTEGER);
            // currentOff = currentOff + SIZE_OF_INTEGER;
            curr->op1.offset = -(totalSize-temp2->entry->offset);

            curr->tag2 = 1;
            curr->op2.a = temp;
            prev = curr;
        }
        else if(check1 == 2){
            curr->Op = MOVQ;
            curr->tag1 = 4;
            curr->op1.offset = -(totalSize-temp2->entry->offset);
            //currentOff = currentOff + SIZE_OF_REAL;

            curr->tag2 = 1;
            curr->op2.a = temp;
            prev = curr;
        }
        else if(check1==3){
            curr->Op = MOVW;
            curr->tag1 = 4;
            curr->op1.offset = -(totalSize-temp2->entry->offset);
            //currentOff = currentOff + SIZE_OF_BOOLEAN;

            curr->tag2 = 1;
            curr->op2.a = temp;
            prev = curr;
        }
        else{
            curr->Op = MOVQ;
            //Here the pointer will be stored first, and then the two integers incase of static array.
            curr->tag1 = 4;
            curr->op1.offset = -(totalSize-temp2->entry->offset);
            //currentOff = currentOff + SIZE_OF_POINTER;

            //possible that temp could be an io parameter.
            if(temp->entry->io==1){
                curr->tag2 = 5;
                curr->op2.offset = (SIZE_AFTER_BP + temp->entry->offset);
            }
            else{
                curr->tag2 = 5;
                curr->op2.offset = -(temp->entry->offset + SIZE_OF_POINTER);
            }
            prev = curr;

            if(temp2->entry->type->First->Right->tag==_NUM && temp->entry->type->First->Right->tag==_ID){
                prev->next = createInst();
                curr = prev->next;
                curr->Op = JEQ;

                curr->tag1 = 1;
                curr->op1.a = temp->entry->type->First->Right;

                curr->tag2 = 2;
                curr->op2.num = (int)temp2->entry->type->First->Right->t->value.integer;

                curr->jump = *label;
                *label+=1;
                prev = curr;

                prev->next = createInst();
                curr = prev->next;
                
                curr->Op = PRINTERROR4;
                curr->tag1 = 2;
                curr->op1.num = temp->t->line_no;
                prev = curr;

                prev->next = createInst();
                curr = prev->next;

                curr->Op = EXIT;
                prev = curr;

                prev->next = createInst();
                curr = prev->next;

                curr->label = *label-1;
                prev = curr;
            }
            if(temp2->entry->type->First->Right->Right->tag==_NUM && temp->entry->type->First->Right->Right->tag==_ID){
                prev->next = createInst();
                curr = prev->next;
                curr->Op = JEQ;

                curr->tag1 = 1;
                curr->op1.a = temp->entry->type->First->Right->Right;

                curr->tag2 = 2;
                curr->op2.num = (int)temp2->entry->type->First->Right->Right->t->value.integer;

                curr->jump = *label;
                *label+=1;
                prev = curr;

                prev->next = createInst();
                curr = prev->next;
                
                curr->Op = PRINTERROR4;
                curr->tag1 = 2;
                curr->op1.num = temp->t->line_no;
                prev = curr;

                prev->next = createInst();
                curr = prev->next;

                curr->Op = EXIT;
                prev = curr;

                prev->next = createInst();
                curr = prev->next;

                curr->label = *label-1;
                prev = curr;
            }

            if(temp2->entry->isStatic){
                prev->next = createInst();
                curr = prev->next;

                curr->Op = MOVD;
                curr->tag1 = 4;
                curr->op1.offset = -(totalSize-(temp2->entry->offset+SIZE_OF_POINTER));

                curr->tag2 = 2;
                curr->op2.num = (int)temp2->entry->type->First->Right->t->value.integer;
                prev = curr;

                prev->next = createInst();
                curr = prev->next;

                curr->Op = MOVD;
                curr->tag1 = 4;
                curr->op1.offset = -(totalSize - (temp2->entry->offset + SIZE_OF_POINTER + SIZE_OF_INTEGER));

                curr->tag2 = 2;
                curr->op2.num = (int)temp2->entry->type->First->Right->Right->t->value.integer;
                prev = curr;
            }
            else{
                if(temp2->entry->type->First->Right->tag==_ID){
                    prev->next = createInst();
                    curr = prev->next;

                    curr->Op = MOVD;
                    curr->tag1 = 4;
                    curr->op1.offset = -(totalSize - (temp2->entry->type->First->Right->entry->offset));

                    if(temp->entry->type->First->Right->tag==_ID){
                        curr->tag2 = 1;
                        curr->op2.a = temp->entry->type->First->Right;
                    }
                    else{
                        curr->tag2 = 2;
                        curr->op2.num = (int)temp->entry->type->First->Right->t->value.integer;
                    }
                    prev = curr;
                }
                if(temp2->entry->type->First->Right->Right->tag==_ID){
                    prev->next = createInst();
                    curr = prev->next;

                    curr->Op = MOVD;
                    curr->tag1 = 4;
                    curr->op1.offset = -(totalSize - (temp2->entry->type->First->Right->Right->entry->offset));

                    if(temp->entry->type->First->Right->Right->tag==_ID){
                        curr->tag2 = 1;
                        curr->op2.a = temp->entry->type->First->Right->Right;
                    }
                    else{
                        curr->tag2 = 2;
                        curr->op2.num = (int)temp->entry->type->First->Right->Right->t->value.integer;
                    }
                    prev = curr;
                }

            }
        }
        prev = curr;

        temp = temp->Right;
        temp2 = temp2->Right->Right;
    }

    prev->next = createInst();
    curr = prev->next;

    curr->Op = MOVQ;
    curr->tag1 = 3;
    curr->op1.R = RCX;

    curr->tag2 = 3;
    curr->op2.R = RSP;
    prev = curr;

    prev->next = createInst();
    curr = prev->next;

    curr->Op = OSUBQ;
    curr->tag1 = 3;
    curr->op1.R = RSP;

    curr->tag2 = 3;
    curr->op2.R = RSP;

    curr->tag3 = 6;
    curr->op3.offset = totalSize;
    prev = curr;

    prev->next = createInst();
    curr = prev->next;

    curr->Op = MOVQ;
    curr->tag1 = 4;
    curr->op1.offset = -(SIZE_OF_POINTER);

    curr->tag2 = 3;
    curr->op2.R = RBP;
    prev = curr;

    prev->next = createInst();
    curr = prev->next;

    curr->Op = MOVQ;
    curr->tag1 = 4;
    curr->op1.offset = -(2* SIZE_OF_POINTER);

    curr->tag2 = 3;
    curr->op2.R = RCX;
    prev = curr;

    prev->next = createInst();
    curr = prev->next;

    curr->Op = OSUBQ;
    curr->tag1 = 3;
    curr->op1.R = RSP;

    curr->tag2 = 3;
    curr->op2.R = RSP;

    curr->tag3 = 6;
    curr->op3.offset = 2 * SIZE_OF_POINTER;
    prev = curr;

    prev->next = createInst();
    curr = prev->next;

    curr->Op = CALL;
    curr->tag1 = 1;
    if(node->First->tag==_MODID){
        curr->op1.a = node->First;
    }
    else{
        curr->op1.a = node->First->Right;
    }
    prev = curr;

    prev->next = createInst();
    curr = prev->next;

    curr->Op = MOVQ;
    curr->tag1 = 3;
    curr->op1.R = RCX;
    
    curr->tag2 = 4;
    curr->op2.offset = 0;
    prev = curr;

    prev->next = createInst();
    curr = prev->next;

    curr->Op = MOVQ;
    curr->tag1 = 3;
    curr->op1.R = RBP;

    curr->tag2 = 4;
    curr->op2.offset = SIZE_OF_POINTER;
    prev = curr;

    prev->next = createInst();
    curr = prev->next;

    curr->Op = MOVQ;
    curr->tag1 = 3;
    curr->op1.R = RSP;

    curr->tag2 = 3;
    curr->op2.R = RCX;
    prev = curr;

    if(node->First->tag==_IDLIST){
        temp = node->First->First;
        temp2 = node->First->Right->entry->type->Right->First;

        while(temp){
            prev->next = createInst();
            curr = prev->next;

            check = getType(temp);

            if(check==1){
                curr->Op = MOVD;
                curr->tag1 = 1;
                curr->op1.a = temp;

                curr->tag2 = 4;
                curr->op2.offset = -(totalSize - temp2->entry->offset);
            }
            else if(check==2){
                curr->Op = MOVQ;
                curr->tag1 = 1;
                curr->op1.a = temp;

                curr->tag2 = 4;
                curr->op2.offset = -(totalSize - temp2->entry->offset);
            }
            else if(check==3){
                curr->Op = MOVW;
                curr->tag1 = 1;
                curr->op1.a = temp;

                curr->tag2 = 4;
                curr->op2.offset = -(totalSize - temp2->entry->offset);
            }
            else{
                //Assumption that none of the output parameters are lists.
                printf("This code is not supposed to execute.\n");
            }
            prev = curr;

            temp = temp->Right;
            temp2 = temp2->Right->Right;
        }

    }


    //Going to have to change every instruction with memory accesses to MOVB, MOVW, MOVD, MOVQ. ->Important!!!!


    return first;
}

int sizeIDList(astNode* node){
    int size = 0;
    astNode* curr = node;
    int check = 0;

    while(curr){
        check = getType(curr);
        if(check==1){
            size+=SIZE_OF_INTEGER;
        }
        else if(check==2){
            size+=SIZE_OF_REAL;
        }
        else if(check==3){
            size+=SIZE_OF_BOOLEAN;
        }
        else if(check>=4){
            size+=(SIZE_OF_POINTER + 2*SIZE_OF_INTEGER);
            //Either way - dynamic or static will need a pointer and two integers.
        }
        else{
            printf("This code should not run.\n");
        }
        curr = curr->Right;
    }
    return size;
}

int sizeParamList(astNode* node){
    int size = 0;
    astNode* curr = node;

    while(curr){
        if(curr->Right->tag==_INTEGER){
            size+=SIZE_OF_INTEGER;
        }
        else if(curr->Right->tag==_REAL){
            size+=SIZE_OF_REAL;
        }
        else if(curr->Right->tag==_BOOLEAN){
            size+=SIZE_OF_BOOLEAN;
        }
        else{
            size+=(SIZE_OF_POINTER + 2*SIZE_OF_INTEGER);
        }

        curr = curr->Right->Right;
    }
    return size;
}

IrCode* stmtsGen(astNode* node, short int* label){
    IrCode* prev = NULL;
    IrCode* first = NULL;
    IrCode* curr = NULL;
    IrCode* i1 = NULL;

    astNode* temp = node->First;

    while(temp){
        switch(temp->tag){
            case _PRINT:
                if(temp->entry && temp->First->First && (temp->First->First->tag==_ID||temp->First->entry->isStatic==0)){
                    i1 = dynamicCheck(temp->First,label);
                    if(first==NULL)first = i1;
                    if(prev==NULL){
                        prev = i1;
                    }
                    else{
                        prev->next = i1;
                    }
                    while(prev->next)prev = prev->next;
                }
                if(prev==NULL){
                    curr = createInst();  
                    first = curr;  
                }
                else{
                    prev->next = createInst();
                    curr = prev->next;
                }

                curr->Op = OPRINT;
                curr->tag1 = 1;
                curr->op1.a = temp->First;
                prev = curr;
                break;
            
            case _GET_VALUE:
                if(temp->First->First && (temp->First->First->tag==_ID||temp->First->entry->isStatic==0)){
                    i1 = dynamicCheck(temp->First,label);
                    if(first==NULL)first = i1;
                    if(prev==NULL){
                        prev = i1;
                    }
                    else{
                        prev->next = i1;
                    }
                    while(prev->next)prev = prev->next;
                }
                if(prev==NULL){
                    curr = createInst();
                    first = curr;
                }
                else{
                    prev->next = createInst();
                    curr = prev->next;
                }

                curr->Op = OGETVALUE;
                curr->tag1 = 1;
                curr->op1.a = temp->First;
                prev = curr;
                break;
            
            case _ASSIGNOP:
                if(prev==NULL){
                    first = assignment(temp,label);
                    prev = first;
                }
                else{
                    prev->next = assignment(temp,label);
                }
                while(prev->next)prev = prev->next;
                break;
            
            case _MODULEREUSESTMT:
                if(prev==NULL){
                    first = modReuseStmt(temp,label);
                    prev = first;
                }
                else{
                    prev->next = modReuseStmt(temp,label);
                }
                while(prev->next)prev = prev->next;
                break;
            
            case _FOR:
                if(prev==NULL){
                    first = forStmt(temp,label);
                    prev = first;
                }
                else{
                    prev->next = forStmt(temp,label);
                }
                while(prev->next)prev = prev->next;
                break;
            
            case _WHILE:
                if(prev==NULL){
                    first = whileStmt(temp,label);
                    prev = first;
                }
                else{
                    prev->next = whileStmt(temp,label);
                }
                while(prev->next)prev = prev->next;
                break;
            
            case _CONDITIONALSTMT:
                if(prev==NULL){
                    first = caseStmts(temp,label);
                    prev = first;
                }
                else{
                    prev->next = caseStmts(temp,label);
                }
                while(prev->next)prev = prev->next;
                break;
            
            case _DECLARESTMT:
                if(temp->First->Right->tag!=_ARRAY){
                    break;
                }
                if(prev==NULL){
                    first = declArray(temp,label);
                    prev = first;
                }
                else{
                    prev->next = declArray(temp,label);
                }
                while(prev->next)prev = prev->next;
                break;

        }
        temp = temp->Right;
    }
    return first;
}

IrCode* programGen(astNode* node,short int* label){
    IrCode* first = NULL;
    IrCode* prev = NULL;
    IrCode* curr = NULL;

    astNode* temp = node->First;
    while(temp){
        switch(temp->tag){
            case _MODULE:
                if(prev==NULL){
                    first = modGen(temp,label);
                    prev = first;
                }
                else{
                    prev->next = modGen(temp,label);
                }
                while(prev->next)prev = prev->next;
                break;
            
            case _DRIVERMODULE:
                if(prev==NULL){
                    first = driverGen(temp,label);
                    prev = first;
                }
                else{
                    prev->next = driverGen(temp,label);
                }
                while(prev->next)prev = prev->next;

                break;
        }
        temp = temp->Right;
    }
    return first;    
}

