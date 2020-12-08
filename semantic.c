#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include<ctype.h>
#include <stdbool.h>
#include "semantic.h"
#include "Semantic-Accessory.h"

void semanticCheck(astNode* Root, hashTable* st, int* error){
    astNode* node = Root->First;
    astNode* curr = NULL;
    while(node){
        Label l = node->tag;
        switch(l){
            case _MODULEDECLARATIONS:
                curr = node->First;

                while(curr){
                    if(curr->error==0 && curr->entry->use==0){
                        printf("Line %d: Semantic error: The module declaration %s is redundant.\n",curr->t->line_no, curr->t->value.identifier);
                        curr->error = 1;
                        *error = 1;
                    }
                    curr = curr->Right;
                }
                break;

            case _MODULE:
                if(node->error == 0){
                    modOpCheck(node,node->entry->derive,error);
                    curr = node->First;
                    while(curr->tag!=_STATEMENTS)curr = curr->Right;
                    stmtsCheck(curr,node->entry->derive,error);
                }
                break;

            case _DRIVERMODULE:
                stmtsCheck(node->First,node->entry->derive,error);
                break;
        }
        node = node->Right;
    }
    return;
}

void modOpCheck(astNode* node, hashTable* st, int* error){
    //Checking output parameters.
    if(node->First->Right->Right->tag==_OUTPUT_PLIST){
        astNode* curr = node->First->Right->Right->First;
        while(curr){
            if(curr->error==0){
                if(curr->entry->use==0){
                    printf("Line %d: Semantic Error: The output parameter %s defined is not assigned a value.\n",curr->t->line_no, curr->t->value.identifier);
                    *error = 1;
                    curr->error = 1;
                }
            }
            curr = curr->Right->Right;
        }
    }
    return;
}

void stmtsCheck(astNode* node, hashTable* st, int* error){
    if(node==NULL)return;
    astNode* curr = node->First;
    astNode* temp = NULL;
    astNode* temp1 = NULL;
    astNode* temp2 = NULL;
    int check = 0;
    while(curr){
        switch(curr->tag){
            case _MODULEREUSESTMT:
                if(curr->error==1)break;

                if(curr->First->tag==_IDLIST){
                    temp2 = curr->First->First;
                    temp = curr->First->Right;
                    //temp has the value of modid.
                }
                else{
                    temp = curr->First;
                    //temp has the value of modid.
                }

                if(temp->entry->type->Right->tag==_OUTPUT_PLIST){
                    temp1 = temp->entry->type->Right->First;
                }

                while(temp1 && temp2){
                    while(temp1->error){
                        temp1 = temp1->Right->Right;
                        temp2 = temp2->Right;
                    }
                    if(!temp1 || !temp2)break;

                    if(temp2->error == 1){
                        //If necessary we can remove the printing of this error.
                        printf("Line %d: Semantic Error: Output parameter %s of module %s not assigned correct value.\n",temp->t->line_no,temp1->t->value.identifier,temp->t->value.identifier);
                        curr->error = 1;
                    }
                    else{
                        check = typeCmp(temp1,temp2);
                        if(check==0){
                            printf("Line %d: Semantic Error: Output parameter %s of module %s assigned variable of type %s.\n",temp->t->line_no,temp1->t->value.identifier,temp->t->value.identifier,printType(temp2));
                            *error = 1;
                            temp2->error = 1;
                            curr->error = 1;
                        }
                    }
                        
                    temp1 = temp1->Right->Right;
                    temp2 = temp2->Right;
                }
                
                if(temp1){
                    while(temp1){
                        while(temp1->error)temp1 = temp1->Right->Right;
                        if(!temp1)break;
                        printf("Line %d: Semantic Error: Output parameter %s of module %s not assigned variable.\n",temp->t->line_no, temp1->t->value.identifier,temp->t->value.identifier);
                        temp1 = temp1->Right->Right;
                    }
                    *error = 1;
                    curr->error = 1;
                }

                if(temp2){
                    while(temp2){
                        while(temp2->error)temp2 = temp2->Right;
                        if(!temp2)break;
                        printf("Line %d: Semantic Error: Extra variable %s placed in output argument list of module %s.\n",temp->t->line_no,temp2->t->value.identifier,temp->t->value.identifier);
                        temp2 = temp2->Right;
                    }
                    *error = 1;
                    curr->error = 1;
                }

                //Same thing for input variables.

                temp2 = temp->Right->First;
                temp1 = temp->entry->type->First;

                while(temp1 && temp2){
                    while(temp1->error){
                        temp1 = temp1->Right->Right;
                        temp2 = temp2->Right;
                    }
                    if(!temp1 || !temp2)break;

                    if(temp2->error == 1){
                        //If necessary we can remove the printing of this error.
                        printf("Line %d: Semantic Error: Input parameter %s of module %s not assigned correct value.\n",temp->t->line_no,temp1->t->value.identifier,temp->t->value.identifier);
                        curr->error = 1;
                    }
                    else{
                        check = typeCmp(temp1,temp2);
                        if(check==0){
                            printf("Line %d: Semantic Error: Input parameter %s of module %s assigned variable of type %s.\n",temp->t->line_no,temp1->t->value.identifier,temp->t->value.identifier,printType(temp2));
                            *error = 1;
                            temp2->error = 1;
                            curr->error = 1;
                        }
                    }
                        
                    temp1 = temp1->Right->Right;
                    temp2 = temp2->Right;
                }
                
                if(temp1){
                    while(temp1){
                        while(temp1->error)temp1 = temp1->Right->Right;
                        if(!temp1)break;
                        printf("Line %d: Semantic Error: Input parameter %s of module %s not assigned variable.\n",temp->t->line_no,temp1->t->value.identifier,temp->t->value.identifier);
                        temp1 = temp1->Right->Right;
                    }
                    *error = 1;
                    curr->error = 1;
                }

                if(temp2){
                    while(temp2){
                        while(temp2->error)temp2 = temp2->Right;
                        if(!temp2)break;
                        printf("Line %d: Semantic Error: Extra variable %s placed in input argument list of module %s.\n",temp->t->line_no,temp2->t->value.identifier,temp->t->value.identifier);
                        temp2 = temp2->Right;
                    }
                    *error = 1;
                    curr->error = 1;
                }

                break;
            
            case _ASSIGNOP:
                //take care of it in while also.
                if(curr->error==0){
                    if(curr->First->Right->tag==_U){
                        offsetComp(curr->First->Right->First->Right);
                    }
                    else{
                        offsetComp(curr->First->Right);
                    }
                }

                break;
            
            case _FOR:
                curr->entry->derive->size = st->size;
                curr->entry->derive->locSize = st->locSize;
                stmtsCheck(curr->First->Right->Right->Right,curr->entry->derive,error);
                st->locSize = curr->entry->derive->locSize;
                st->size = curr->entry->derive->size;
                break;

            case _WHILE:
                if(!curr->error && curr->entry->whileHas && !curr->entry->whileUse){
                    printf("Line %d: Semantic Error: None of the variables in the conditional expression of the while loop defined in line %d to line %d is assigned a value in the loop.\n",curr->scope[0],curr->scope[0],curr->scope[1]);
                    *error = 1;
                    curr->error = 1;
                }

                if(!curr->First->error){
                    offsetComp(curr->First);
                }

                curr->entry->derive->size = st->size;
                curr->entry->derive->locSize = st->locSize;
                stmtsCheck(curr->First->Right,curr->entry->derive,error);
                st->locSize = curr->entry->derive->locSize;
                st->size = curr->entry->derive->size;

                break;
            
            case _CONDITIONALSTMT:
                if(!curr->error){
                    check = getType(curr->First);
                
                    temp = curr->First;
                    temp = temp->Right;
                    while(temp->tag!=_DEFAULT)temp = temp->Right->Right;

                    if(check==1 && temp->First==NULL){
                        printf("Line %d: Semantic Error: The switch statement defined in line %d to line %d does not have a default statment.\n",curr->scope[0],curr->scope[0],curr->scope[1]);
                        curr->error = 1;
                        *error = 1;
                    }
                    
                    if(check==3 && temp->First){
                        printf("Line %d: Semantic Error: The switch statement defined in line %d to line %d should not have a default statment.\n",curr->scope[0],curr->scope[0],curr->scope[1]);
                        curr->error = 1;
                        *error = 1;       
                    }
                }

                check = getType(curr->First);
                if(check!=1 && check!=3){
                    break;
                }

                st->locSize = curr->entry->derive->locSize;
                st->size = curr->entry->derive->size;

                temp = curr->First->Right;
                while(temp!=NULL && temp->tag!=_DEFAULT){
                    temp = temp->Right;
                    stmtsCheck(temp,curr->entry->derive,error);
                    temp = temp->Right;
                }

                if(temp->tag==_DEFAULT){
                    if(temp->First){
                        stmtsCheck(temp->First,curr->entry->derive,error);
                    }
                }

                st->locSize = curr->entry->derive->locSize;
                st->size = curr->entry->derive->size;

                break;

        }
        curr = curr->Right;
    }
}

void offsetComp(astNode* node){
    if(node->tag==_ID || node->tag==_TRUE || node->tag==_FALSE || node->tag==_NUM || node->tag==_RNUM){
        return;
    }
    else{
        offsetComp(node->First);
        offsetComp(node->First->Right);
        int type = getType(node);

        if(node->entry->partOf->first==NULL){
            node->entry->partOf->first = node->entry;
            node->entry->partOf->current = node->entry;
        }
        else{
            node->entry->partOf->current->next = node->entry;
            node->entry->partOf->current = node->entry;
        }

        if(type==1){
            node->entry->offset = node->entry->partOf->size;
            node->entry->partOf->size+=SIZE_OF_INTEGER;
            node->entry->width = 2;
            node->entry->locOffset = node->entry->partOf->locSize;
            node->entry->partOf->locSize+=2;
        }
        else if(type==2){
            node->entry->offset = node->entry->partOf->size;
            node->entry->partOf->size+=SIZE_OF_REAL;
            node->entry->width = 4;
            node->entry->locOffset = node->entry->partOf->locSize;
            node->entry->partOf->locSize+=4;
        }
        else if(type==3){
            node->entry->offset = node->entry->partOf->size;
            node->entry->partOf->size+=SIZE_OF_BOOLEAN;
            node->entry->width = 1;
            node->entry->locOffset = node->entry->partOf->locSize;
            node->entry->partOf->locSize+=1; 
        }
        else{
            printf("This code should not execute.\n");
        }
    }
    return;
}

void printARSize(hashTable* st){
    printf("\n");
    printf("%-21s%-21s\n","Module name","ARSize");
    printf("\n");
    hashEntry* curr = st->first;
    while(curr){
        char* c = curr->id;
        if(c[0]=='$')c = c+1;
        printf("%-21s%-21d\n",c,curr->derive->locSize);
        curr = curr->next;
    }
    printf("\n");
    return;
}

void printST(hashTable* st){
    printf("\n");
    printf("%-18s%-21s%-21s%-15s%-15s%-21s%-21s%-21s%-15s%-21s","variable_name","scope(module_name)","scope(line_numbers)","width","isArray","static_or_dynamic","range_lexemes","type_of_element","offset","nesting_level");
    printf("\n");

    hashEntry* curr = st->first;
    while(curr){
        printScopeVar(curr->derive,1);
        curr = curr->next;
    }
}

void printScopeVar(hashTable* st,int nestLevel){
    hashEntry* curr = st->first;
    hashEntry* func = curr;
    while(func->partOf->parentEntry)func = func->partOf->parentEntry;
    char* d = func->id;
    if(d[0]=='$')d = d+1;
    int tag = 0;

    while(curr){
        tag = 0;
        if(curr->derive){
            printScopeVar(curr->derive,nestLevel+1);
            curr = curr->next;
            continue;
        }
        char* c= curr->id;
        if(c[0]=='$')c = c+1;
        char scope[20];
        sprintf(scope,"%d-%d",st->scope[0],st->scope[1]);
        printf("%-18s%-21s%-21s%-15d",c,d,scope,curr->width);
        if(curr->type->tag==_ARRAY){
            tag = 1;
            printf("%-15s","yes");
            if(curr->isStatic==0){
                printf("%-21s","dynamic");
            }
            else{
                printf("%-21s","static");
            }

            char* rangearr;
            char range[20] = "[";
            rangearr = range;
            if(curr->type->First->Right->tag==_NUM){
                char integer1[5];
                sprintf(integer1,"%d",(int)curr->type->First->Right->t->value.integer);
                rangearr = strcat(rangearr,integer1);
            }
            else{
                rangearr = strcat(rangearr,curr->type->First->Right->t->value.identifier);
            }

            rangearr = strcat(rangearr,"..");

            if(curr->type->First->Right->Right->tag==_NUM){
                char integer2[5];
                sprintf(integer2,"%d",(int)curr->type->First->Right->Right->t->value.integer);
                rangearr = strcat(rangearr,integer2);
            }
            else{
                rangearr = strcat(rangearr,curr->type->First->Right->Right->t->value.identifier);
            }
            rangearr = strcat(rangearr,"]");

            printf("%-21s",rangearr);

        }
        else{
            printf("%-15s%-21s%-21s","no","---","---");
        }

        astNode* baseType = NULL;
        if(tag==1){
            baseType = curr->type->First;
        }
        else{
            baseType = curr->type;
        }

        if(baseType->tag==_INTEGER){
            printf("%-21s","integer");
        }
        else if(baseType->tag==_REAL){
            printf("%-21s","real");
        }
        else{
            printf("%-21s","boolean");
        }

        printf("%-15d",curr->locOffset);
        if(curr->io){
            printf("%-21d",0);
        }
        else{
            printf("%-21d",nestLevel);
        }      

        printf("\n");
        curr = curr->next;
    }
}

void printArrFeatures(hashTable* st){
    printf("\n");
    printf("%-21s%-21s%-21s%-21s%-21s%-21s","scope(module_name)","scope(line_numbers)","variable_name","static_or_dynamic","range_lexemes","type_of_element");
    printf("\n");

    hashEntry* curr = st->first;
    while(curr){
        printArrRec(curr->derive);
        curr = curr->next;
    }
}

void printArrRec(hashTable *st){
    hashEntry* curr = st->first;
    hashEntry* func = curr;
    while(func->partOf->parentEntry)func = func->partOf->parentEntry;
    char* d = func->id;
    if(d[0]=='$')d = d+1;
    
    while(curr){
        if(curr->derive){
            printArrRec(curr->derive);
            curr = curr->next;
            continue;
        }

        if(curr->type->tag==_ARRAY){
            printf("\n");
            char scope[20];
            sprintf(scope,"%d-%d",st->scope[0],st->scope[1]);
            printf("%-21s%-21s",d,scope);

            char* c= curr->id;
            if(c[0]=='$')c = c+1;
            printf("%-21s",c);

            if(curr->isStatic){
                printf("%-21s","static array");
            }
            else{
                printf("%-21s","dynamic array");
            }

            char* rangearr;
            char range[20] = "[";
            rangearr = range;
            if(curr->type->First->Right->tag==_NUM){
                char integer1[5];
                sprintf(integer1,"%d",(int)curr->type->First->Right->t->value.integer);
                rangearr = strcat(rangearr,integer1);
            }
            else{
                rangearr = strcat(rangearr,curr->type->First->Right->t->value.identifier);
            }

            rangearr = strcat(rangearr,"..");

            if(curr->type->First->Right->Right->tag==_NUM){
                char integer2[5];
                sprintf(integer2,"%d",(int)curr->type->First->Right->Right->t->value.integer);
                rangearr = strcat(rangearr,integer2);
            }
            else{
                rangearr = strcat(rangearr,curr->type->First->Right->Right->t->value.identifier);
            }
            rangearr = strcat(rangearr,"]");

            printf("%-21s",rangearr);

            if(curr->type->First->tag==_INTEGER){
                printf("%-21s","integer");
            }
            else if(curr->type->First->tag==_REAL){
                printf("%-21s","real");
            }
            else{
                printf("%-21s","boolean");
            }
            printf("\n");
        }
        curr = curr->next;
    }
}