// Group Number - 25
// Name : Aayush Atul Verma         ID : 2017A7PS0061P
// Name : Aditya Ramaswamy          ID : 2017A7PS0130P
// Name : Jithin Kallukalam Sojan   ID : 2017A7PS0163P

#include "Semantic-Accessory.h"
#include <stdlib.h>
#include <string.h>



int cmpVal(astNode* a,astNode* b){
    if(a->tag==_NUM && b->tag==_NUM && a->t->value.integer==b->t->value.integer){
        return 1;
    }
    else if(a->tag==_RNUM && b->tag==_RNUM && a->t->value.real==b->t->value.real){
        return 1;
    }
    else if(a->tag==_TRUE && b->tag==_TRUE){
        return 1;
    }
    else if(a->tag==_FALSE && b->tag==_FALSE){
        return 1;
    }
    else if(a->tag==_ID && b->tag==_ID && a->entry==b->entry){
        //Making sure they are the same variable.
        if(a->First && b->First){
            int check = cmpVal(a->First,b->First);
            if(check==1)return 1;
        }
        else if(!(a->First) && !(b->First)){
            return 1;
        }
    }
    return 0;
}

int getType(astNode* node){
    //1 for integer, 2 for real and 3 for boolean, 4 for int array, 5 for real array and 6 for boolean array/

    if(node->entry!=NULL){
        if(node->entry->type->tag==_ARRAY){
            if(node->First){
                if(node->entry->type->First->tag==_INTEGER){
                    return 1;
                }
                else if(node->entry->type->First->tag==_REAL){
                    return 2;
                }
                else if(node->entry->type->First->tag==_BOOLEAN){
                    return 3;
                }
                else{
                    printf("Not supposed to execute.\n");
                    return 7;
                }
            }
            else{
                if(node->entry->type->First->tag==_INTEGER){
                    return 4;
                }
                else if(node->entry->type->First->tag==_REAL){
                    return 5;
                }
                else if(node->entry->type->First->tag==_BOOLEAN){
                    return 6;
                }
                else{
                    printf("Not supposed to execute.\n");
                    return 7;
                }
            }
        }
        else{
            if(node->tag==_NUM || (node->entry->type->tag==_INTEGER)){
                return 1;
            }
            else if(node->tag==_RNUM || (node->entry->type->tag==_REAL)){
                return 2;
            }
            else if(node->tag==_TRUE || node->tag==_FALSE || (node->entry->type->tag==_BOOLEAN)){
                return 3;
            }
            else{
                printf("Not supposed to execute.\n");
                return 7;
            }
        }

    }
    else{
        if(node->tag==_NUM)return 1;
        else if(node->tag==_RNUM)return 2;
        else if(node->tag==_TRUE || node->tag==_FALSE || node->tag==_BOOLEAN)return 3;
        printf("This code is not supposed to be executed.\n");
        return 7;
    }
}

int typeCmp(astNode* a,astNode* b){
    int check1 = getType(a);
    int check2 = getType(b);
    
    if(check1!=check2){
        return 0;
    }
    else{
        if(check1>=4){
            //It means they are arrays.
            if(a->entry->isStatic && b->entry->isStatic){
                int check3 = cmpVal(a->entry->type->First->Right,b->entry->type->First->Right);
                int check4 = cmpVal(a->entry->type->First->Right->Right,b->entry->type->First->Right->Right);

                if((!check3)||(!check4)){
                    return 0;
                }
            }
            else if(a->entry->type->First->Right->tag==_NUM && b->entry->type->First->Right->tag==_NUM && !cmpVal(a->entry->type->First->Right,b->entry->type->First->Right)){
                return 0;
            }
            else if(a->entry->type->First->Right->Right->tag==_NUM && b->entry->type->First->Right->Right->tag==_NUM && !cmpVal(a->entry->type->First->Right->Right,b->entry->type->First->Right->Right)){
                return 0;
            }
        }
    }
    return 1;
}

char* printId(astNode* node){
    //should print something like A[10] or A[a]
    char output[28];
    strcpy(output,node->t->value.identifier);
    char* new = output;
    if(node->First){
        new = strcat(output,"[");
        char num[6];
        if(node->First->tag==_NUM){
            sprintf(num,"%lld",node->First->t->value.integer);
            new = strcat(output,num);
        }
        else{
            new = strcat(output,printId(node->First));
        }
        new = strcat(output,"]");
    }
    return new;
}


char* printOp(astNode* node){
    switch(node->tag){
        case _PLUS:
            return "+";
        case _MINUS:
            return "-";
        case _MUL:
            return "*";
        case _DIV:
            return "/";
        case _LT:
            return "<";
        case _LE:
            return "<=";
        case _GT:
            return ">";
        case _GE:
            return ">=";
        case _EQ:
            return "==";
        case _NE:
            return "!=";
        case _AND:
            return "AND";
        case _OR:
            return "OR";
        default:
            printf("Not supposed to execute.\n");
            return "";

    }
}

char* printType(astNode* node){
    if(node->tag==_NUM || (node->entry && (node->entry->type->tag==_INTEGER || (node->First && node->entry->type->tag==_ARRAY && node->entry->type->First->tag==_INTEGER)))){
        return "Integer";
    }
    else if(node->tag==_RNUM || (node->entry && (node->entry->type->tag==_REAL || (node->First && node->entry->type->tag==_ARRAY && node->entry->type->First->tag==_REAL)))){
        return "Real";
    }
    else if(node->tag==_TRUE || node->tag==_FALSE || (node->entry && (node->entry->type->tag==_BOOLEAN || (node->First && node->entry->type->tag==_ARRAY && node->entry->type->First->tag==_BOOLEAN)))){
        return "Boolean";
    }
    else if(node->entry->type->tag==_ARRAY){
        char* new = (char*)malloc(sizeof(char)*30);
        strcpy(new,"Array of ");
        if(node->entry->type->First->tag==_INTEGER){
            new = strcat(new,"Integer");
        }
        else if(node->entry->type->First->tag==_REAL){
            new = strcat(new,"Real");
        }
        else{
            new = strcat(new,"Boolean");
        }
        new = strcat(new,"[");
        if(node->entry->type->First->Right->tag==_ID){
            new = strcat(new,node->entry->type->First->Right->t->value.identifier);
        }
        else{
            char num1[10];
            sprintf(num1,"%lld",node->entry->type->First->Right->t->value.integer);
            new = strcat(new,num1);
        }
        new = strcat(new,"..");
        if(node->entry->type->First->Right->Right->tag==_ID){
            new = strcat(new,node->entry->type->First->Right->Right->t->value.identifier);
        }
        else{
            char num2[10];
            sprintf(num2,"%lld",node->entry->type->First->Right->Right->t->value.integer);
            new = strcat(new,num2);
        }
        new = strcat(new,"]");
        return new;
        
    }
    else{
        printf("This code should not run.\n");
        return "";
    }
}