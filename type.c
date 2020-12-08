// Group Number - 25
// Name : Aayush Atul Verma         ID : 2017A7PS0061P
// Name : Aditya Ramaswamy          ID : 2017A7PS0130P
// Name : Jithin Kallukalam Sojan   ID : 2017A7PS0163P

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include<ctype.h>
#include <stdbool.h>
#include "type.h"
#include "ast.h"
#include "Semantic-Accessory.h"

hashTable* typeStart(astNode* Root, int* error){
    //*error is 0 initially.
    hashTable* st = initHashTable();
    st->parentEntry = NULL;
    Traverse(Root->First,error,st);
    return st;
}

void Traverse(astNode* node, int* error, hashTable* st){
    while(node){
        Label l = node->tag;
        switch(l){
            case _MODULEDECLARATIONS:
                modDecl(node,error,st);
                break;
            case _MODULE:
                modDef(node,error,st);
                break;
            case _DRIVERMODULE:
                driverDef(node,error,st);
                break;
        }
        node = node->Right;
    }
}

void modDecl(astNode* node, int* error, hashTable* st){
    astNode* curr = node->First;
    int index;
    while(curr){
        index = insert(st,((curr->t)->value).identifier,NULL);
        if(index==-2){
            printf("Line %d: Semantic Error, Identifier %s defined in this line is already used for another module.\n",curr->t->line_no,curr->t->value.identifier);
            *error = 1;
            curr->error = 1;
        }
        else if(index==-1){
            printf("Overflow of symbol table.\n");
            *error = 1;
            curr->error = 1;
        }
        else{
            curr->entry = st->Table[index];
            curr->error = 0;
            curr->entry->use = 0;
            //Internal use: if another module uses it when it is 2 instead of zero, there is no redundancy error.
        }
        curr = curr->Right;
    }    
}

void modDef(astNode* node, int* error, hashTable* st){
    //MODID,INPUT_PLIST,OUTPUT_PLIST,STATEMENTS
    //or
    //MODID,INPUT_PLIST,STATEMENTS
    int decl = 0;
    astNode* curr = node->First;
    int index = retrieve(st,curr->t->value.identifier);
    if(index!=-2){
        if(st->Table[index]->type!=NULL){
            printf("Line %d: Semantic Error, Module %s is redefined.\n",curr->t->line_no,curr->t->value.identifier);
            *error = 1; 
            node->error = 1;
            return;
            //This means the entire module is not gone through.
        }
        else{
            node->entry = st->Table[index];
            st->Table[index]->type = curr->Right;
            //Basically, the type list does not end with a NULL. Some random stuff on the right.
        }
    }
    else{
        index = insert(st,curr->t->value.identifier,curr->Right);
        //Basically, the type list does not end with a NULL. Some random stuff on the right.
        //Important while code gen.
        if(index==-1){
            printf("Overflow in symbol table.\n");
            *error = 1;
            node->error = 1;
            return;
        }
        node->entry = st->Table[index];
    }

    node->entry->extra = node;
    node->First->entry = node->entry;

    if(st->first==NULL){
        st->first = node->entry;
        st->current = node->entry;
    }
    else{
        st->current->next = node->entry;
        st->current = node->entry;
    }

    hashTable* new = initHashTable();
    node->entry->derive = new;
    new->parentEntry = node->entry;
    new->scope[0] = node->scope[0];
    new->scope[1] = node->scope[1];

    insertModIO(new,node->entry->type,error);

    while(curr->tag!=_STATEMENTS){
        curr = curr->Right;
    }
    statements(new,curr,error);

    //If decl is 1 that means there is an error.
}

void driverDef(astNode* node, int* error, hashTable* st){
    char driver[22] = "$driver";
    astNode* curr = node->First;
    int index = insert(st,driver,NULL);
    node->entry = st->Table[index];

    if(st->first==NULL){
        st->first = node->entry;
        st->current = node->entry;
    }
    else{
        st->current->next = node->entry;
        st->current = node->entry;
    }

    hashTable* new = initHashTable();
    node->entry->derive = new;
    new->parentEntry = node->entry;
    new->scope[0] = node->scope[0];
    new->scope[1] = node->scope[1];

    statements(new,curr,error);

}

void insertModIO(hashTable* st,astNode* node,int* error){
    astNode* curr = node->First;
    astNode* type;
    while(curr){
        type = curr->Right;
        if(type->tag==_ARRAY){
            addArray(st,curr,error,1);
            //1 here means it is an input parameter.
            //need to define it as an input parameter.
        }
        else addVar(st,curr,error,1);

        curr = curr->Right->Right;
        
    }
    node = node->Right;
    if(node->tag==_OUTPUT_PLIST){
        curr = node->First;
        while(curr){
            type = curr->Right;
            if(type->tag==_ARRAY){
                addArray(st,curr,error,4);
            }
            else{
                addVar(st,curr,error,4);
            }

            curr = curr->Right->Right;
        }
    }
}

void addVar(hashTable* st,astNode* node, int* error, int option){
    char input[22] = "$";
    // char output[22] = "#";
    astNode* type = node->Right;
    char tempvar[22] = "temp#";
    char integer[10] = "";
    char* key;
    if(option==1){
        key = strcat(input,node->t->value.identifier);
    }
    else if(option==3){
        sprintf(integer,"%d",st->nextTemp);
        st->nextTemp++;
        //key = strcat(tempvar,strcat(tempvar,integer));

        key = strcat(tempvar,integer);
    }
    else{
        key = node->t->value.identifier;
    }

    int index = insert(st,key,type);
    if(index==-2){
        if(option==3){
            printf("This could should not execute.\n");
            node->error = 1;
            *error = 1;
            return;
        }
        //Doesnt matter for option 3 as this code will never be executed.
        printf("Line %d: Semantic Error, Identifier %s in this line is already defined in the scope line %d to line %d.\n",node->t->line_no,node->t->value.identifier,st->scope[0],st->scope[1]);
        node->error = 1;
        *error = 1;
        return;
    }
    else if(index==-1){
        printf("Overflow in symbol table.\n");
        node->error = 1;
        *error = 1;
        return;
    }
    node->entry = st->Table[index];
    node->entry->isStatic = 1;
    if(option==3){
        return;
    }
    if(node->entry->partOf->first==NULL){
        node->entry->partOf->first = node->entry;
        node->entry->partOf->current = node->entry;
    }
    else{
        node->entry->partOf->current->next = node->entry;
        node->entry->partOf->current = node->entry;
    }
    if(option==1 || option==4){
        node->entry->io = 1;
        node->entry->offset = st->ioSize;
        node->entry->locOffset = st->locIoSize;

        if(type->tag==_INTEGER){
            st->ioSize = st->ioSize + SIZE_OF_INTEGER;
            node->entry->width = 2;
            st->locIoSize = st->locIoSize + 2;
        }
        else if(type->tag==_REAL){
            st->ioSize = st->ioSize + SIZE_OF_REAL;
            node->entry->width = 4;
            st->locIoSize = st->locIoSize + 4;
        }
        else{
            st->ioSize = st->ioSize + SIZE_OF_BOOLEAN;
            node->entry->width = 1;
            st->locIoSize = st->locIoSize + 1;
        }
    }
    else{
        node->entry->offset = st->size;
        node->entry->locOffset = st->locSize;

        if(type->tag==_INTEGER){
            st->size = st->size + SIZE_OF_INTEGER;
            node->entry->width = 2;
            st->locSize = st->locSize + 2;
        }
        else if(type->tag==_REAL){
            st->size = st->size + SIZE_OF_REAL;
            node->entry->width = 4;
            st->locSize = st->locSize + 4;
        }
        else{
            st->size = st->size + SIZE_OF_BOOLEAN;
            node->entry->width = 1;
            st->locSize = st->locSize + 1;
        }
    }
}

void addArray(hashTable* st,astNode* node, int* error, int option){
    int isStatic = 1;
    char input[22] = "$";
    //char output[22] = "#";
    //ouptut parameters cannot be shadowed. 
    astNode* type = node->Right;
    if(type->First->Right->tag==_NUM && type->First->Right->Right->tag==_NUM){
        isStatic = 1;
        long long int low = type->First->Right->t->value.integer;
        long long int high = type->First->Right->Right->t->value.integer;
        if(low<0){
            printf("Line %d: Semantic Error, The indices of the range has a negative value.\n",type->First->Right->t->line_no);
            *error = 1;
            node->error = 1;
            return;
        }
        if(high<0){
            printf("Line %d: Semantic Error, The indices of the range has a negative value.\n",type->First->Right->t->line_no);
            *error = 1;
            node->error = 1;
            return;
        }
        if(high<low){
            printf("Line %d: Semantic Error, The range from %lld to %lld defined here is not valid.\n",type->First->Right->t->line_no,low,high);
            *error = 1;
            node->error = 1;
            return;
        }
    }
    else{
        isStatic = 0;
        if(type->First->Right->tag==_ID){
            if(option==1){
                astNode* extra = (astNode*)malloc(sizeof(astNode));
                extra->tag=_INTEGER;
                extra->Right = NULL;
                extra->First = NULL;
                extra->Parent = NULL;

                astNode* next = type->First->Right->Right;
                type->First->Right->Right = extra;
                addVar(st,type->First->Right,error,1);
                char* c = strcat(type->First->Right->entry->id,"#");
                char integer3[10];
                sprintf(integer3,"%d",st->nextTemp);
                st->nextTemp++;
                c = strcat(c,integer3);
                type->First->Right->Right = next;
            }
            else{
                int check = typeExtract(st,type->First->Right,error,0);
                //typeExtract returns 1 on success.
                if(check==0){
                    node->error = 1;
                    return;
                    //All other errors will be taken care of inside the function itself.
                }
                if(check==1 && type->First->Right->entry->type->tag!=_INTEGER){
                    printf("Line %d: Semantic Error, The identifier %s is not of type INTEGER.",type->First->Right->t->line_no,type->First->Right->t->value.identifier);
                    *error = 1;
                    node->error = 1;
                    type->First->Right->error = 1;
                    return;
                }
                // && retrieve(st,type->First->Right->t->value.identifier)==-2) && retrieve(st,strcat(input,type->First->Right->t->value.identifier))==-2
                //type extraction done in another function.
                //printf("Semantic Error: The value defined");
            }
            
        }
        else{
            if(type->First->Right->t->value.integer<0){
                printf("Line %d: Semantic Error, The range cannot have a negative index.\n",type->First->Right->t->line_no);
                *error = 1;
                node->error = 1;
                type->First->Right->error = 1;
                return;
            }
        }
        if(type->First->Right->Right->tag==_ID){
            if(option==1){
                astNode* extra = (astNode*)malloc(sizeof(astNode));
                extra->tag=_INTEGER;
                extra->Right = NULL;
                extra->First = NULL;
                extra->Parent = NULL;

                astNode* next = type->First->Right->Right->Right;
                type->First->Right->Right->Right = extra;
                addVar(st,type->First->Right->Right,error,1);
                char* c = strcat(type->First->Right->Right->entry->id,"#");
                char integer3[10];
                sprintf(integer3,"%d",st->nextTemp);
                st->nextTemp++;
                c = strcat(c,integer3);
                type->First->Right->Right->Right = next;
            }
            else{
                int check = typeExtract(st,type->First->Right->Right,error,0);
                //typeExtract returns 1 on success.
                if(check==0){
                    node->error = 1;
                    return;
                    //All other errors will be taken care of inside the function itself.
                }
                if(check==1 && type->First->Right->Right->entry->type->tag!=_INTEGER){
                    printf("Line %d: Semantic Error: The identifier %s is not of type INTEGER.",type->First->Right->Right->t->line_no,type->First->Right->Right->t->value.identifier);
                    *error = 1;
                    node->error = 1;
                    type->First->Right->Right->error = 1;
                    return;
                }
                // && retrieve(st,type->First->Right->t->value.identifier)==-2) && retrieve(st,strcat(input,type->First->Right->t->value.identifier))==-2
                //type extraction done in another function.
                //printf("Semantic Error: The value defined");
            }
        }
        else{
            if(type->First->Right->Right->t->value.integer<0){
                printf("Line %d: Semantic Error: The range cannot have a negative index.\n",type->First->Right->Right->t->line_no);
                *error = 1;
                node->error = 1;
                type->First->Right->Right->error = 1;
                return;
            }
        }
    }

    char* key;

    if(option==1){
        key = strcpy(input,node->t->value.identifier);
        //The name is $inputparameter
    }
    else{
        key = node->t->value.identifier;
    }

    int index = insert(st,key,type);
    if(index==-2){
        printf("Line %d: Semantic error: Identifier %s is already defined in the scope line %d to line %d.\n",node->t->line_no,node->t->value.identifier,st->scope[0],st->scope[1]);
        node->error = 1;
        *error = 1;
        return;
    }
    node->entry = st->Table[index];
    node->entry->isStatic = isStatic;
    if(node->entry->partOf->first==NULL){
        node->entry->partOf->first = node->entry;
        node->entry->partOf->current = node->entry;
    }
    else{
        node->entry->partOf->current->next = node->entry;
        node->entry->partOf->current = node->entry;
    }
    if(option==1 || option==4){
        node->entry->io = 1;
        node->entry->offset = st->ioSize;
        node->entry->locOffset = st->locIoSize;
        if(isStatic){
            //we are saving the bounds either way.
            //Incase dynamic=static array or something like that.
            st->ioSize = st->ioSize + SIZE_OF_POINTER;
            st->ioSize = st->ioSize + SIZE_OF_INTEGER;
            st->ioSize = st->ioSize + SIZE_OF_INTEGER;
            node->entry->width = 5;
            st->locIoSize = st->locIoSize + 5;
        }
        else{
            st->ioSize = st->ioSize + SIZE_OF_POINTER;
            node->entry->width = 1;
            st->locIoSize = st->locIoSize + 1;
            //storing the bounds of the dynamic array.
            // st->size = st->size + SIZE_OF_INTEGER;
            //The values will not change.
        }
        
    }
    else{
        node->entry->offset = st->size;
        node->entry->locOffset = st->locSize;
        if(isStatic){
            //we are saving the bounds.
            //Incase dynamic=static array.
            st->size = st->size + SIZE_OF_POINTER;
            st->size = st->size + SIZE_OF_INTEGER;
            st->size = st->size + SIZE_OF_INTEGER;
            node->entry->width = 5;
            st->locSize +=5;
            if(option!=1){
                //We need this memory only if not an input parameter
                if(type->First->tag==_INTEGER){
                    st->size = st->size+((int)(type->First->Right->Right->t->value.integer-type->First->Right->t->value.integer)+1)*SIZE_OF_INTEGER;
                    node->entry->width += ((int)(type->First->Right->Right->t->value.integer-type->First->Right->t->value.integer)+1)*2;
                    st->locSize +=  ((int)(type->First->Right->Right->t->value.integer-type->First->Right->t->value.integer)+1)*2;
                }
                else if(type->First->tag==_REAL){
                    st->size = st->size+((int)(type->First->Right->Right->t->value.integer-type->First->Right->t->value.integer)+1)*SIZE_OF_REAL;
                    node->entry->width += ((int)(type->First->Right->Right->t->value.integer-type->First->Right->t->value.integer)+1)*4;
                    st->locSize +=  ((int)(type->First->Right->Right->t->value.integer-type->First->Right->t->value.integer)+1)*4;
                }
                else{
                    st->size = st->size+((int)(type->First->Right->Right->t->value.integer-type->First->Right->t->value.integer)+1)*SIZE_OF_BOOLEAN;
                    node->entry->width += ((int)(type->First->Right->Right->t->value.integer-type->First->Right->t->value.integer)+1)*1;
                    st->locSize +=  ((int)(type->First->Right->Right->t->value.integer-type->First->Right->t->value.integer)+1)*1;
                }
            }
        }
        else{
            st->size = st->size + SIZE_OF_POINTER;
            node->entry->width = 1;
            st->locSize += 1;
            //storing the bounds of the dynamic array.
            // st->size = st->size + SIZE_OF_INTEGER;
            //The values will not change.
        }
    }

}

int typeExtract(hashTable* st, astNode* node, int* error, int option){
    //returns 1 if the given id exists and binds the record to it.
    //returns 0 if error.
    //also node->error =1 and *error =1;

    //option == 1 means the identifier is being assigned a value.
    //option == 2 means the value for which type is being checked is a while loop concdition variable.

    hashTable* ht = st;
    hashEntry* e1 = NULL;
    int index;
    while(ht->parentEntry!=NULL){
        char input[22] = "$";
        //Checks if the given identifier is an input param as well.
        index = retrieve(ht,node->t->value.identifier);
        if(index>=0){
            node->entry = ht->Table[index];
            break;
        }
        index = retrieve(ht,strcat(input,node->t->value.identifier));
        if(index>=0){
            node->entry = ht->Table[index];
            break;
        }

        e1 = ht->parentEntry;
        ht = (e1)->partOf;
    }
    if(ht->parentEntry){
        if(node->First){
            //Supposed to be an array element.
            //Only ids can have an index below it.
            if(node->entry->type->tag!=_ARRAY){
                printf("Line %d: Semantic Error: Identifier %s used is not an array.\n",node->t->line_no,node->t->value.identifier);
                *error = 1;
                node->error = 1;
                return 0;
            }
            //Static bound checking.
            else if(node->First->tag==_NUM && node->entry->isStatic==1){
                if((node->First->t->value.integer > node->entry->type->First->Right->Right->t->value.integer)
                    || (node->First->t->value.integer < node->entry->type->First->Right->t->value.integer)){
                        printf("Line %d: Semantic Error: The index %lld does not exist in the range of the array %s.\n",node->First->t->line_no,node->First->t->value.integer,node->t->value.identifier);
                        *error = 1;
                        node->error = 1;
                        return 0;
                    }
            }
            else if(node->First->tag==_NUM && node->entry->type->First->Right->tag==_NUM){
                if(node->First->t->value.integer<node->entry->type->First->Right->t->value.integer){
                    printf("Line %d: Semantic Error: The index %lld does not exist in the range of the array %s.\n",node->First->t->line_no,node->First->t->value.integer,node->t->value.identifier);
                    *error = 1;
                    node->error = 1;
                    return 0;
                }
            }
            else if(node->First->tag==_NUM && node->entry->type->First->Right->Right->tag==_NUM){
                if(node->First->t->value.integer>node->entry->type->First->Right->Right->t->value.integer){
                    printf("Line %d: Semantic Error: The index %lld does not exist in the range of the array %s.\n",node->First->t->line_no,node->First->t->value.integer,node->t->value.identifier);
                    *error = 1;
                    node->error = 1;
                    return 0;
                }
            }
            else if(node->First->tag==_ID){
                int check = typeExtract(st,node->First,error,0);
                if(check==0){
                    *error = 1;
                    node->error = 1;
                    return 0;
                }
                else if(node->First->entry->type->tag!=_INTEGER){
                    printf("Line %d: Semantic Error: The index %s used for array %s is not of type Integer.\n",node->First->t->line_no,node->First->t->value.identifier,node->t->value.identifier);
                    *error = 1;
                    node->error = 1;
                    return 0;
                }
            }
            else if(node->First->tag!=_NUM){
                printf("This code should not execute.\n");
                *error = 1;
                node->error = 1;
                return 0;
            }     
        }

            if(option>=1){
                if(ht->Table[index]->forBlock==1){
                    //forBlock is assigned if the current scope in consideration is one that has the current identifier as a loop variable or a parent for loop does.
                    printf("Line %d: Semantic Error: The for loop variable %s is assigned a value.\n",node->t->line_no,node->t->value.identifier);
                    //Finding the exact for loop the variable belongs to is hard.
                    *error = 1;
                    node->error = 1;
                    return 0;
                }

                if(ht->Table[index]->whileBlock==1){
                    whileCheck* wc = ht->Table[index]->check;
                    while(wc){
                        if(wc->entry->whileBlock==1){

                            wc->entry->whileUse = 1;
                            //Since incase of A[10], even if A[3] changes it is fine.
                        }
                        wc = wc->next;
                    }
                }

            }

            if(option==1){
                ht->Table[index]->use = 1;
            }
        return 1;
    }
    *error = 1;
    node->error = 1;
    printf("Line %d: Semantic Error: The identifier %s used has not been declared before usage.\n",node->t->line_no,node->t->value.identifier);
    return 0;
}

int expressionCheck(hashTable* st, astNode* node, int* error, int option, hashEntry* extra){
    //returns 1 on success, 0 on error.
    //option = 1, means normal expression, option = 2, means conditional expression of while loop, option = 3, means checking if any of the variables in the while Conditional expression have been assigned a value.
    //The partial types of dynamic components are calculated.

    if(!node)return 1;

    int check1 = 1;
    int check2 = 1;
    astNode* type = NULL;
    astNode* next = NULL;
    astNode* line = NULL;
    int typeErr = 0;

    if(node->tag==_PLUS || node->tag==_MINUS || node->tag==_MUL || node->tag==_DIV || node->tag==_EQ || node->tag==_NE || node->tag==_LE || node->tag==_LT || node->tag==_GT || node->tag==_GE){
        check1 = expressionCheck(st,node->First,error,option,extra);
        check2 = expressionCheck(st,node->First->Right,error,option,extra);
        if(!check1||!check2){
            node->error = 1;
            return 0;
        }

        if(node->First->tag==_NUM || node->First->tag==_RNUM){
            typeErr = 0;
        }
        else if(node->First->entry){
            if(node->First->entry->type->tag==_INTEGER || node->First->entry->type->tag==_REAL || (node->First->entry->type->tag==_ARRAY && node->First->First && (node->First->entry->type->First->tag==_INTEGER || node->First->entry->type->First->tag==_REAL))){
                typeErr = 0;
            }
            else{
                typeErr = 1;
            }
        }
        else{
            typeErr = 1;
        }

        if(typeErr==1){
            *error = 1;
            node->error = 1;
            line = node;
            while(line->First)line = line->First;
            printf("Line %d : Semantic Error: Expression of basic type %s cannot undergo %s operation.\n",line->t->line_no,printType(node->First),printOp(node));
            return 0;
        }

        typeErr = 0;

        if(node->First->Right->tag==_NUM || node->First->Right->tag==_RNUM){
            typeErr = 0;
        }
        else if(node->First->Right->entry){
            if(node->First->Right->entry->type->tag==_INTEGER || node->First->Right->entry->type->tag==_REAL || (node->First->Right->entry->type->tag==_ARRAY && node->First->Right->First && (node->First->Right->entry->type->First->tag==_INTEGER || node->First->Right->entry->type->First->tag==_REAL))){
                typeErr = 0;
            }
            else{
                typeErr = 1;
            }
        }
        else{
            typeErr = 1;
        }

        if(typeErr==1){
            *error = 1;
            node->error = 1;
            line = node;
            while(line->First)line = line->First;
            printf("Line %d: Semantic Error: Expression of basic type %s cannot undergo %s operation.\n",line->t->line_no,printType(node->First->Right),printOp(node));
            return 0;
        }
        
        check1 = typeCmp(node->First,node->First->Right);
        if(!check1){
            line = node;
            while(line->First)line = line->First;
            printf("Line %d: Semantic Error: Expression of basic type %s cannot undergo %s operation with expression of basic type %s.\n",line->t->line_no,printType(node->First),printOp(node),printType(node->First->Right));
            *error = 1;
            node->error = 1;
            return 0;
        }
        if(node->tag==_PLUS || node->tag==_MINUS || node->tag==_MUL || node->tag==_DIV){
            if(node->First->tag==_NUM){
                type = createNode(_INTEGER,NULL);
            }
            else if(node->First->tag==_RNUM){
                type = createNode(_REAL,NULL);
            }
            else if(node->First->entry->type->tag==_INTEGER){
                type = createNode(_INTEGER,NULL);
            }
            else if(node->First->entry->type->tag==_REAL){
                type = createNode(_REAL,NULL);
            }
            else if(node->First->entry->type->First->tag==_INTEGER){
                type = createNode(_INTEGER,NULL);
            }
            else{
                type = createNode(_REAL,NULL);
            }
        }   
        else{
            type = createNode(_BOOLEAN,NULL);
        }
        next = node->Right;
        node->Right = type;
        addVar(st,node,error,3);
        node->Right = next;
        //Assumption, overflow does not happen.
        return 1;
    }
    else if(node->tag==_AND || node->tag==_OR){
        check1 = expressionCheck(st,node->First,error,option,extra);
        check2 = expressionCheck(st,node->First->Right,error,option,extra);
        if(!check1||!check2){
            node->error = 1;
            return 0;
        }
        
        if(node->First->tag==_TRUE || node->First->tag==_FALSE){
            typeErr = 0;
        }
        else if(node->First->entry){
            if(node->First->entry->type->tag==_BOOLEAN || (node->First->entry->type->tag==_ARRAY && node->First->First && (node->First->entry->type->First->tag==_BOOLEAN))){
                typeErr = 0;
            }
            else{
                typeErr = 1;
            }
        }
        else{
            typeErr = 1;
        }

        if(typeErr==1){
            *error = 1;
            node->error = 1;
            line = node;
            while(line->First)line = line->First;
            printf("Line %d: Semantic Error: Expression of basic type %s cannot undergo %s operation.\n",line->t->line_no,printType(node->First),printOp(node));
            return 0;
        }

        typeErr = 0;

        if(node->First->Right->tag==_TRUE || node->First->Right->tag==_FALSE){
            typeErr = 0;
        }
        else if(node->First->Right->entry){
            if(node->First->Right->entry->type->tag==_BOOLEAN || (node->First->Right->entry->type->tag==_ARRAY && node->First->Right->First && (node->First->Right->entry->type->First->tag==_BOOLEAN))){
                typeErr = 0;
            }
            else{
                typeErr = 1;
            }
        }
        else{
            typeErr = 1;
        }

        if(typeErr==1){
            *error = 1;
            node->error = 1;
            line = node;
            while(line->First)line = line->First;
            printf("Line %d: Semantic Error: Expression of basic type %s cannot undergo %s operation.\n",line->t->line_no,printType(node->First->Right),printOp(node));
            return 0;
        }

        type = createNode(_BOOLEAN,NULL);

        next = node->Right;
        node->Right = type;
        addVar(st,node,error,3);
        node->Right = next;
        //Assumption, overflow does not happen.
        return 1;
    }
    else if(node->tag==_ID){
        if(option==1){
            check1 = typeExtract(st,node,error,0);
        }
        else if(option==2){
            check1 = typeExtract(st,node,error,0);
            if(check1){
                node->entry->whileBlock = 1;
                whileCheck* new = (whileCheck*)malloc(sizeof(whileCheck));
                new->entry = extra;
                extra->whileHas = 1;
                new->next = NULL;
                if(node->First){
                    new->index = node->First;
                    if(node->First->tag==_ID){
                        node->First->entry->whileBlock = 1;
                        whileCheck* new1 = (whileCheck*)malloc(sizeof(whileCheck));
                        new1->entry = extra;
                        new1->next = NULL;
                        new1->index = NULL;

                        if(node->First->entry->check==NULL){
                            node->First->entry->check = new1;
                        }
                        else{
                            whileCheck* iter = node->First->entry->check;
                            if(iter){
                                while(iter->next)iter = iter->next;
                                iter->next = new1;        
                            }
                            else{
                                node->First->entry->check = new1;
                            }
                        }
                    }
                }
                else{
                    new->index = NULL;
                }

                if(node->entry->check==NULL){
                    node->entry->check = new;
                }
                else{
                    whileCheck* iter = node->entry->check;
                    if(iter){
                        while(iter->next)iter = iter->next;
                        iter->next = new;
                    }
                    else{
                        node->entry->check = new;
                    }
                }
            }
        }
        else{
            printf("There is no other option.\n");
        }

        if(check1==0){
            node->error = 1;
            *error = 1;
            return 0;
        }
        else{
            return 1;
        }
    }
    else{
        return 1;
    }
}

int declCheck(hashTable* st, astNode* node, int* error){
    //The loop variables should not be allowed to be redifined.

    hashTable* ht = st;
    int index;
    while(ht->parentEntry){
        char input[22] = "$";
        //Checks if the given identifier is an input param as well.
        index = retrieve(ht,node->t->value.identifier);
        if(index>=0){
            //node->entry = ht->Table[index];
            break;
        }
        index = retrieve(ht,strcat(input,node->t->value.identifier));
        if(index>0){
            break;
        }
        ht = ht->parentEntry->partOf;
    }

    if(ht->parentEntry){
        if(ht->Table[index]->forBlock){
            printf("Line %d: Semantic Error: Loop variable %s is redeclared.\n",node->t->line_no,node->t->value.identifier);
            *error = 1;
            node->error = 1;
            return 0;
        }
    }
    return 1;
}


void statements(hashTable* st, astNode* node, int* error){
    if(!node)return;
    //Default will have empty statement.
    astNode* curr = node->First;
    while(curr){
        switch(curr->tag){
            int check;
            int index;
            astNode* temp;
            astNode* a;
            astNode* type;
            astNode* next;
            hashTable* ht;
            hashTable* new;
            case _PRINT:
                if(curr->First->tag!=_ID)break;
                check = typeExtract(st,curr->First,error,0);
                if(check==0){
                    curr->error = 1;
                    break;
                }
                break;

            case _GET_VALUE:
                //Remember that an entire array can be inputed.
                check = typeExtract(st,curr->First,error,1);
                if(check==0){
                    curr->error = 1;
                    break;
                }
                break;
            
            case _DECLARESTMT:
                temp = curr->First;
                type = temp->Right;
                temp = temp->First;
                if(type->tag==_ARRAY){
                    while(temp){
                        //Because array variables cannot be for variables or switch statement varables.
                        next = temp->Right;
                        temp->Right = type;
                        addArray(st,temp,error,0);
                        //But this will print an error for every time there is an error.
                        temp->Right = next;
                        temp = temp->Right;
                    }
                }
                else{
                    while(temp){
                        check = declCheck(st,temp,error);
                        if(check==0)continue;
                        next = temp->Right;
                        temp->Right = type;
                        addVar(st,temp,error,0);
                        temp->Right = next;
                        temp = temp->Right;
                    }
                }
                break;

            case _ASSIGNOP:
                check = typeExtract(st,curr->First,error,1);
                if(check==0){
                    curr->error = 1;
                    break;
                }

                if(curr->First->Right->tag==_U){
                    check = expressionCheck(st,curr->First->Right->First->Right,error,1,NULL);
                    if(check==0){
                        curr->First->Right->error = 1;
                        curr->error = 1;
                        break;
                    }
                    else{
                        if(!(getType(curr->First->Right->First->Right)==1||getType(curr->First->Right->First->Right)==2)){
                            printf("Line %d: Semantic Error: The expression to the right of the unary %s operator has to be arithematic.\n",curr->First->t->line_no,printOp(curr->First->Right->First));
                            curr->error = 1;
                            curr->First->Right->error = 1;
                            break;
                        }
                    }
                    check = typeCmp(curr->First,curr->First->Right->First->Right);
                    if(check==0){
                        curr->error = 1;
                        *error =1;
                        printf("Line %d: Semantic error: Identifier %s of type %s is assigned a value of type %s.\n",curr->First->t->line_no,printId(curr->First),printType(curr->First),printType(curr->First->Right->First->Right));
                    }
                    break;
                }
                else{
                    check = expressionCheck(st,curr->First->Right,error,1,NULL);
                    if(check==0){
                        curr->error = 1;
                        break;
                    }

                    check = typeCmp(curr->First,curr->First->Right);
                    if(check==0){
                        curr->error = 1;
                        *error =1;
                        printf("Line %d: Semantic error: Identifier %s of type %s is assigned a value of type %s.\n",curr->First->t->line_no,printId(curr->First),printType(curr->First),printType(curr->First->Right));
                    }
                    break;
                }
                break;

            case _MODULEREUSESTMT:
                ht = st;
                while(ht->parentEntry){
                    ht = (ht->parentEntry)->partOf;
                }
                temp = curr->First;
                if(temp->tag!=_MODID)temp = temp->Right;
                index = retrieve(ht,temp->t->value.identifier);
                if(index==-2){
                    printf("Line %d: Semantic Error: The module %s used has not been defined/declared.\n",temp->t->line_no,temp->t->value.identifier);
                    *error = 1;
                    curr->error = 1;
                    break;
                }
                else if(curr->Parent->Parent->tag==_MODULE && curr->Parent->Parent->entry==ht->Table[index]){
                    printf("Line %d: Semantic Error: The module %s is called recursively.\n",temp->t->line_no,temp->t->value.identifier);
                    *error = 1;
                    curr->error = 1;
                    //break;
                }
                else{
                    curr->entry = ht->Table[index];
                    if(ht->Table[index]->type==NULL){
                        ht->Table[index]->use = 1;
                    }
                    temp->entry = ht->Table[index];
                }

                temp = curr->First;
                if(temp->tag==_IDLIST){
                    a = temp->First;
                    while(a){
                        check = typeExtract(st,a,error,1);
                        a = a->Right;
                    }
                    temp = temp->Right;
                }

                while(temp->tag!=_IDLIST)temp = temp->Right;

                a = temp->First;
                while(a){
                    check = typeExtract(st,a,error,0);
                    a = a->Right;
                }
                break;
            
            case _FOR:
                temp = curr->First;
                check = typeExtract(st,temp,error,2);
                //This is one because if incase the loop variable is used as a loop variable in some for loop of which this for loop is a scope.
                //But this means that the for loop variable is assigned a value. In case of an output pararmeter, it would be assigned a value.


                if(check == 0){
                    curr->error = 1;
                    //break;
                    //we can still go through the for loop.
                }
                else{
                    if(temp->entry->type->tag!=_INTEGER){
                        printf("Line %d: Semantic Error: The loop variable %s is not of type Integer.\n",temp->t->line_no,temp->t->value.identifier);
                        *error = 1;
                        temp->error = 1;
                        curr->error = 1;
                    }
                    else{
                        temp->entry->forBlock = 1;
                    }
                }

                if(temp->Right->Right->t->value.integer<temp->Right->t->value.integer){
                    printf("Line %d: Semantic Error: The range %lld to %lld is decreasing.\n",temp->Right->t->line_no,temp->Right->t->value.integer,temp->Right->Right->t->value.integer);
                    *error = 1;
                    temp->Right->error = 1;
                    curr->error = 1;
                }

                char for1[22] = "for";
                char num[10];
                sprintf(num,"%d",st->nextTemp);
                st->nextTemp++;
                index = insert(st,strcat(for1,num),NULL);
                if(index==-2){
                    printf("This should not happen.\n");
                    break;
                }
                curr->entry = st->Table[index];
                curr->entry->forBlock = 1;

                if(st->first==NULL){
                    st->first = curr->entry;
                    st->current = curr->entry;
                }
                else{
                    st->current->next = curr->entry;
                    st->current = curr->entry;
                }

                curr->entry->extra = curr;
                new = initHashTable();
                curr->entry->derive = new;
                new->parentEntry = curr->entry;
                new->scope[0] = curr->scope[0];
                new->scope[1] = curr->scope[1];
                
                new->size = st->size;
                new->locSize = st->locSize;
                statements(new,temp->Right->Right->Right,error);
                st->locSize = new->locSize;                
                st->size = new->size;

                curr->entry->forBlock = 0;
                if(check==1)temp->entry->forBlock = 0;
                break;            

            case _WHILE:
                temp = curr->First;
                char while1[22] = "while";
                char num1[10];
                sprintf(num1,"%d",st->nextTemp);
                st->nextTemp++;
                index = insert(st,strcat(while1,num1),NULL);
                if(index==-2){
                    printf("This should not happen.\n");
                    break;
                }
                curr->entry = st->Table[index];
                curr->entry->whileBlock = 1;
                if(st->first==NULL){
                    st->first = curr->entry;
                    st->current = curr->entry;
                }
                else{
                    st->current->next = curr->entry;
                    st->current = curr->entry;
                }
                curr->entry->extra = curr;

                check = expressionCheck(st,curr->First,error,2,curr->entry);
                if(check==0){
                    curr->error = 1;
                    *error = 1;
                }
                else if(getType(curr->First)!=3){
                    curr->error = 1;
                    *error = 1;
                    a = curr->First;
                    while(a->First)a = a->First;
                    printf("Line %d: Semantic Error: The type of the expression in the while loop condition is not Boolean.\n",a->t->line_no);
                }

                new = initHashTable();
                curr->entry->derive = new;
                new->parentEntry = curr->entry;
                new->scope[0] = curr->scope[0];
                new->scope[1] = curr->scope[1];

                new->size = st->size;
                new->locSize = st->locSize;
                statements(new,temp->Right,error);          
                st->locSize = new->locSize;      
                st->size = new->size;

                curr->entry->whileBlock = 0;
                break;
            
            case _CONDITIONALSTMT:
                ;
                int valCheck = 1;
                temp = curr->First;
                check = typeExtract(st,temp,error,0);
                if(check == 0){
                    valCheck = 0;
                    curr->error = 1;
                    //break;
                    //we can still go through the for loop.
                }
                else if(!(getType(temp)==1||getType(temp)==3)){
                    //If the id is not of type integer/boolean.
                    printf("Line %d: Semantic Error: Identifier %s of type %s cannot be the conditional variable.\n",temp->t->line_no,temp->t->value.identifier,printType(temp));
                    curr->error = 1;
                    valCheck = 0;
                    break;
                }

                char case1[22] = "case";
                char num2[10];
                sprintf(num2,"%d",st->nextTemp);
                st->nextTemp++;
                index = insert(st,strcat(case1,num2),NULL);
                if(index==-2){
                    printf("This should not happen.\n");
                    break;
                }
                else if(index==-1){
                    printf("This code should not execute.\n");
                    break;
                }
                curr->entry = st->Table[index];
                curr->entry->extra = curr;
                if(st->first==NULL){
                    st->first = curr->entry;
                    st->current = curr->entry;
                }
                else{
                    st->current->next = curr->entry;
                    st->current = curr->entry;
                }
                new = initHashTable();
                curr->entry->derive = new;
                new->parentEntry = curr->entry;
                new->scope[0] = curr->scope[0];
                new->scope[1] = curr->scope[1];

                new->size = st->size;
                new->locSize = st->locSize;
                
                temp = temp->Right;
                while(temp!=NULL && temp->tag!=_DEFAULT){
                    if(valCheck && !typeCmp(temp,curr->First)){
                        printf("Line %d: Semantic Error: Value of type %s used instead of %s in case statement.\n",temp->t->line_no,printType(temp),printType(curr->First));
                    }
                    temp = temp->Right;

                    statements(new,temp,error);
                    temp = temp->Right;
                }

                if(temp->tag==_DEFAULT){
                    statements(new,temp->First,error);
                }

                st->locSize = new->locSize;
                st->size = new->size;
                break;

            default:
                printf("This is not supposed to happen.\n");
                break;

        }   
        curr = curr->Right;
    }
}
