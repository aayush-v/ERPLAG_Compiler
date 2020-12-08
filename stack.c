// Group Number - 25
// Name : Aayush Atul Verma         ID : 2017A7PS0061P
// Name : Aditya Ramaswamy          ID : 2017A7PS0130P
// Name : Jithin Kallukalam Sojan   ID : 2017A7PS0163P

#include<stdlib.h>
#include "parser.h"
#include "stack.h"
#define MAX_SIZE 10

// Stack initialized for parsing
stack* initStack(int sze){
    stack* stackptr;
    stackptr = (stack*)malloc(sizeof(stack));
    stackptr->top=0;
    stackptr->size=sze;
    
    ParseNode** stack;
    stack = (ParseNode**)malloc(sizeof(ParseNode*)*(stackptr->size));
    stackptr->s= stack;
    stack[0] = NULL;

    return stackptr;
}

// Function to push a Node in the created stack
void pushIt(stack* stackptr, ParseNode* toPush){
    if(stackptr->top<((stackptr->size)-1)){
        (stackptr->s)[stackptr->top+1] = toPush;
        stackptr->top = (stackptr->top)+1;
    }
    else{
        ParseNode** newstack;
        newstack = malloc(sizeof(ParseNode*)*(stackptr->size)*2);
        for(int i=0; i<stackptr->size;i++){
            newstack[i] = (stackptr->s)[i];
        }
        stackptr->size = (stackptr->size) * 2;
        stackptr->s = newstack;
        (stackptr->s)[stackptr->top+1] = toPush;
        stackptr->top = (stackptr->top)+1;
    }
    
}

// Function to pop a Node in the created stack
ParseNode* popIt(stack* stackptr){
    ParseNode* popper;

    if(stackptr->top==0){
        printf("End symbol, end of stack reached\n");
        return NULL;
    }
    popper = (stackptr->s)[stackptr->top];
    (stackptr->s)[stackptr->top]= NULL;
    stackptr->top = (stackptr->top)-1;

    return popper;
}


ParseNode* top(stack* stackptr){
    ParseNode* topper;
    
    if(stackptr->top==0){
        return NULL;
    }
    topper = (stackptr->s)[stackptr->top];

    return topper;
}

void rulePush(stack* stackptr, ParseNode* rule_list){
    ParseNode* temp;
    if(rule_list->Right!=NULL){
        temp = rule_list->Right;
        rulePush(stackptr, temp);
    }
    temp = rule_list;
    pushIt(stackptr, temp);
}