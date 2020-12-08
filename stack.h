// Group Number - 25
// Name : Aayush Atul Verma         ID : 2017A7PS0061P
// Name : Aditya Ramaswamy          ID : 2017A7PS0130P
// Name : Jithin Kallukalam Sojan   ID : 2017A7PS0163P

#ifndef STACK_H
#define STACK_H
typedef struct stack{
    int size;
    int top;
    ParseNode** s;
}stack;
void rulePush(stack* stackptr, ParseNode* rule_list);
ParseNode* popIt(stack* stackptr);
void pushIt(stack* stackptr, ParseNode* toPush);
stack* initStack(int sze);
ParseNode* top(stack* stackptr);
#endif