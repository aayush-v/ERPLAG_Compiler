// Group Number - 25
// Name : Aayush Atul Verma         ID : 2017A7PS0061P
// Name : Aditya Ramaswamy          ID : 2017A7PS0130P
// Name : Jithin Kallukalam Sojan   ID : 2017A7PS0163P

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include<ctype.h>
#include <stdbool.h>
#include "asmGen.h"
#include "astDef.h"
#include "hashTableDef.h"
#include "codeGen.h"
#include "Semantic-Accessory.h"

void asmCreate(IrCode* node,char path[]){
    int currTemp = 1;
    FILE* f1 = fopen(path,"w");
	if(f1==NULL)return;
    fprintf(f1,"%-20s\n","global main");
    fprintf(f1,"%-20s\n%-20s\n","extern printf","extern scanf");
    fprintf(f1,"\n");
    fprintf(f1,"%-20s\n","section .data");
    fprintf(f1,"%-20s\n","integer1:");
    fprintf(f1,"\t\t%-20s\n","dd 0");
    fprintf(f1,"%-20s\n","boolean1:");
    fprintf(f1,"\t\t%-20s\n","dw 0");
    fprintf(f1,"%-20s\n","float1:");
    fprintf(f1,"\t\t%-20s\n","dq 0");
    fprintf(f1,"\n");
    fprintf(f1,"%-20s\n","section .text");
    fprintf(f1,"formatint:\n\t\tdb  \"%%d\",10,0\n");
    fprintf(f1,"formatintarr:\n\t\tdb  \"%%d \",0\n");
    fprintf(f1,"formatfloat:\n\t\tdb  \"%%lf\",10,0\n");
    fprintf(f1,"formatfloatarr:\n\t\tdb  \"%%lf \",0\n");
    fprintf(f1,"formatbool:\n\t\tdb  \"%%s\",10,0\n");
    fprintf(f1,"formatboolarr:\n\t\tdb  \"%%s \",0\n");
    fprintf(f1,"formatenter:\n\t\tdb 10,0\n");
    fprintf(f1,"formatprint:\n\t\tdb \"$>Output: \",0\n");
    fprintf(f1,"formaterror1:\n\t\tdb  \"RUN TIME ERROR: Array index out of bounds in line %%d\",10,0\n");
    fprintf(f1,"formaterror2:\n\t\tdb  \"RUN TIME ERROR: Array ranges not compatible in line %%d\",10,0\n");
    fprintf(f1,"formaterror3:\n\t\tdb  \"RUN TIME ERROR: Array range not correctly defined in line %%d\",10,0\n");
    fprintf(f1,"formaterror4:\n\t\tdb  \"RUN TIME ERROR: Array range not compatible to input parameter in line %%d\",10,0\n");
    fprintf(f1,"formatzero:\n\t\tdb  \"RUN TIME ERROR: Div by zero error in line %%d\",10,0\n");
    fprintf(f1,"true:\n\t\tdb  \"true\",0\n");
    fprintf(f1,"false:\n\t\tdb  \"false\",0\n");
    fprintf(f1,"formatgetval:\n\t\tdb  \"Enter an %%s value\",10,0\n");
    fprintf(f1,"formatgetvalarr1:\n\t\tdb  \"Enter %%d array elements of %%s\",0\n");
    fprintf(f1,"formatgetvalarr2:\n\t\tdb \" type for range %%d to %%d\",10,0\n");
    fprintf(f1,"formatdollar:\n\t\tdb  \"$>\",0\n");
    fprintf(f1,"formatinttext:\n\t\tdb  \"integer\",0\n");
    fprintf(f1,"formatfloattext:\n\t\tdb  \"float\",0\n");
    fprintf(f1,"formatbooltext:\n\t\tdb  \"boolean\",0\n");
    fprintf(f1,"formatinbool:\n\t\tdb  \"%%d\",0\n");
    fprintf(f1,"formatinint:\n\t\tdb  \"%%d\",0\n");
    fprintf(f1,"formatinfloat:\n\t\tdb  \"%%lf\",0\n");

    IrCode* curr = node;
    while(curr){
        fprintf(f1,"\n");
        if(curr->label!=0){
            fprintf(f1,"%s%d:\n","L",curr->label);
        }

        if(curr->Op==ONULLOP){
            fprintf(f1,"\n");
        }
        else if(curr->Op==ODRIVER){
            fprintf(f1,"main:\n");
            fprintf(f1,"push rcx\n");
            fprintf(f1,"mov rbp,rsp\n");
        }
        else if(curr->Op==DRIVERRET){
            fprintf(f1,"mov rsp,rbp\n");
            fprintf(f1,"pop rcx\n");
            fprintf(f1,"ret\n");
        }
        else if(curr->Op==CREATE){
            fprintf(f1,"%s:\n",curr->op1.a->t->value.identifier);
        }
        else if(curr->Op==RET){
            fprintf(f1,"ret\n");
        }
        else if(curr->Op==EXIT){
            fprintf(f1,"mov al,1\n");
            fprintf(f1,"mov ebx,0\n");
            fprintf(f1,"int 80h\n");
        }
        else if(curr->Op ==PRINTERROR1){
            fprintf(f1,"mov edi,formaterror1\n");
            fprintf(f1,"xor ebx,ebx\n");
            fprintf(f1,"mov ebx,%d\n",curr->op1.num);
            fprintf(f1,"mov esi,ebx\n");
            fprintf(f1,"xor rax,rax\n");
            fprintf(f1,"call printf\n");
        }
        else if(curr->Op ==PRINTERROR2){
            fprintf(f1,"mov edi,formaterror2\n");
            fprintf(f1,"xor ebx,ebx\n");
            fprintf(f1,"mov ebx,%d\n",curr->op1.num);
            fprintf(f1,"mov esi,ebx\n");
            fprintf(f1,"xor rax,rax\n");
            fprintf(f1,"call printf\n");
        }
        else if(curr->Op ==PRINTERROR3){
            fprintf(f1,"mov edi,formaterror3\n");
            fprintf(f1,"xor ebx,ebx\n");
            fprintf(f1,"mov ebx,%d\n",curr->op1.num);
            fprintf(f1,"mov esi,ebx\n");
            fprintf(f1,"xor rax,rax\n");
            fprintf(f1,"call printf\n");
        }
        else if(curr->Op ==PRINTERROR4){
            fprintf(f1,"mov edi,formaterror4\n");
            fprintf(f1,"xor ebx,ebx\n");
            fprintf(f1,"mov ebx,%d\n",curr->op1.num);
            fprintf(f1,"mov esi,ebx\n");
            fprintf(f1,"xor rax,rax\n");
            fprintf(f1,"call printf\n");
        }
        else if(curr->Op==ZEROERROR){
            fprintf(f1,"mov edi,formatzero\n");
            fprintf(f1,"xor ebx,ebx\n");
            fprintf(f1,"mov ebx,%d\n",curr->op1.num);
            fprintf(f1,"mov esi,ebx\n");
            fprintf(f1,"xor rax,rax\n");
            fprintf(f1,"call printf\n");
        }
        else if(curr->Op==JMP){
            fprintf(f1,"jmp %s%d\n","L",curr->jump);
        }
        else if(curr->Op==CALL){
            fprintf(f1,"call %s\n",curr->op1.a->t->value.identifier);
        }
        else if(curr->Op==JLE || curr->Op==JL || curr->Op==JGE || curr->Op==JG || curr->Op==JEQ || curr->Op==JNEQ || curr->Op==JNEQW){
            if(curr->Op!=JNEQW){
                genOperands(curr,1,4,1,1,f1,0);
                genOperands(curr,2,4,2,1,f1,0);
                fprintf(f1,"cmp eax,ebx\n");
                switch(curr->Op){
                    case JLE:
                        fprintf(f1,"jle");
                        break;
                    case JL:
                        fprintf(f1,"jl");
                        break;
                    case JGE:
                        fprintf(f1,"jge");
                        break;
                    case JG:
                        fprintf(f1,"jg");
                        break;
                    case JEQ:
                        fprintf(f1,"je");
                        break;
                    case JNEQ:
                        fprintf(f1,"jne");
                        break;
                }
                fprintf(f1," %s%d\n","L",curr->jump);
            }
            else{
                genOperands(curr,1,2,1,1,f1,0);
                genOperands(curr,2,2,2,1,f1,0);
                fprintf(f1,"cmp ax,bx\n");
                fprintf(f1,"jne %s%d\n","L",curr->jump);
            }
        }
        else if(curr->Op==OAND || curr->Op==OOR){
            genOperands(curr,2,2,1,1,f1,0);
            genOperands(curr,3,2,2,1,f1,0);
            if(curr->Op==OAND){
                fprintf(f1,"and");
            }
            else{
                fprintf(f1,"or");
            }

            fprintf(f1," ax,bx\n");
            genOperands(curr,1,2,1,0,f1,0);
        }
        else if(curr->Op==OLE || curr->Op==OLT || curr->Op==OGE || curr->Op==OGT || curr->Op==OEQ || curr->Op==ONE){
            genOperands(curr,2,4,1,1,f1,0);
            genOperands(curr,3,4,2,1,f1,0);
            fprintf(f1,"cmp eax,ebx\n");
            switch(curr->Op){
                case OLE:
                    fprintf(f1,"jle");
                    break;
                case OLT:
                    fprintf(f1,"jl");
                    break;
                case OGE:
                    fprintf(f1,"jge");
                    break;
                case OGT:
                    fprintf(f1,"jg");
                    break;
                case OEQ:
                    fprintf(f1,"je");
                    break;
                case ONE:
                    fprintf(f1,"jne");
                    break;
            }
            fprintf(f1," %s%d\n","T",currTemp);
            currTemp++;
            fprintf(f1,"xor eax,eax\n");
            fprintf(f1,"mov ax,0\n");
            fprintf(f1,"jmp %s%d\n","T",currTemp);
            currTemp++;
            currTemp-=2;
            fprintf(f1,"%s%d: ","T",currTemp);
            currTemp+=2;
            fprintf(f1,"xor eax,eax\n");
            fprintf(f1,"mov ax,1\n");
            currTemp-=1;
            fprintf(f1,"%s%d: ","T",currTemp);
            currTemp+=1;
            genOperands(curr,1,2,1,0,f1,0);
        }
        else if(curr->Op==OADD || curr->Op==OSUB || curr->Op==OMUL || curr->Op==ODIV || curr->Op==OSUBQ){
            if(curr->Op!=OSUBQ){
                genOperands(curr,2,4,1,1,f1,0);
                genOperands(curr,3,4,2,1,f1,0);
                if(curr->Op==OADD){
                    fprintf(f1,"add eax,ebx\n");
                    genOperands(curr,1,4,1,0,f1,0);
                }
                else if(curr->Op==OSUB){
                    fprintf(f1,"sub eax,ebx\n");
                    genOperands(curr,1,4,1,0,f1,0);
                }
                else if(curr->Op==OMUL){
                    fprintf(f1,"mov r12,rdx\n");
                    fprintf(f1,"mul ebx\n");
                    genOperands(curr,1,4,1,0,f1,0);
                    if(!(curr->tag1==3 && curr->op1.R==EDX)){
                        fprintf(f1,"mov rdx,r12\n");
                    }
                }
                else{
                    fprintf(f1,"mov r12,rdx\n");
                    fprintf(f1,"mov edx,0\n");
                    fprintf(f1,"div ebx\n");
                    genOperands(curr,1,4,1,0,f1,0);
                    if(!(curr->tag1==3 && curr->op1.R==EDX)){
                        fprintf(f1,"mov rdx,r12\n");
                    }
                }
            }
            else{
                genOperands(curr,2,8,1,1,f1,0);
                genOperands(curr,3,8,2,1,f1,0);
                fprintf(f1,"sub rax,rbx\n");
                genOperands(curr,1,8,1,0,f1,0);
            }
        }
        else if(curr->Op==MOVQ){
            genOperands(curr,2,8,1,1,f1,0);
            genOperands(curr,1,8,1,0,f1,0);
        }
        else if(curr->Op==MOVD){
            genOperands(curr,2,4,1,1,f1,0);
            genOperands(curr,1,4,1,0,f1,0);
        }
        else if(curr->Op==MOVW){
            genOperands(curr,2,2,1,1,f1,0);
            genOperands(curr,1,2,1,0,f1,0);
        }
        else if(curr->Op==OPRINT){
            fprintf(f1,"mov rdi,formatprint\n");
            fprintf(f1,"xor rax,rax\n");
            fprintf(f1,"call printf\n");
            int type = getType(curr->op1.a);
            if(type==1){
                if(curr->op1.a->entry){
                    genOperands(curr,1,4,1,1,f1,0);
                }
                else{
                    curr->tag1 = 2;
                    curr->op1.num = (int)curr->op1.a->t->value.integer;
                    genOperands(curr,1,4,1,1,f1,0);
                }
                
                fprintf(f1,"mov edi,formatint\n");
                fprintf(f1,"mov esi,eax\n");
                fprintf(f1,"xor rax,rax\n");
                fprintf(f1,"call printf\n");    
            }
            else if(type==2){
                if(curr->op1.a->entry){
                    genOperands(curr,1,8,1,1,f1,0);
                }
                else{
                    curr->tag1 = 2;
                    curr->op1.real = curr->op1.a->t->value.real;
                    genOperands(curr,1,8,1,1,f1,0);
                }
                fprintf(f1,"mov rdi,formatfloat\n");
                fprintf(f1,"mov rsi,rax\n");
                fprintf(f1,"xor rax,rax\n");
                fprintf(f1,"call printf\n");
            }
            else if(type==3){
                fprintf(f1,"xor eax,eax\n");
                if(curr->op1.a->entry){
                    genOperands(curr,1,2,1,1,f1,0);
                }
                else{
                    curr->tag1 = 2;
                    if(curr->op1.a->tag==_TRUE){
                        curr->op1.boolVal = 1;
                    }
                    else{
                        curr->op1.boolVal = 0;
                    }
                    genOperands(curr,1,2,1,1,f1,0);
                }
                fprintf(f1,"mov rdi,formatbool\n");
                fprintf(f1,"cmp ax,0\n");
                fprintf(f1,"je printfalse%d\n",currTemp);
                fprintf(f1,"printtrue%d: mov rsi,true\n",currTemp);
                fprintf(f1,"jmp next%d\n",currTemp);
                fprintf(f1,"printfalse%d: mov rsi,false\n",currTemp);
                fprintf(f1,"next%d:\n",currTemp);
                currTemp++;
                fprintf(f1,"xor rax,rax\n");
                fprintf(f1,"call printf\n");
            }
            else{

                if(curr->op1.a->entry->type->First->Right->tag==_NUM){
                    fprintf(f1,"mov ecx,%d\n",(int)curr->op1.a->entry->type->First->Right->t->value.integer);
                }
                else{
                    IrCode* i1 = createInst();
                    i1->tag1 = 1;
                    i1->op1.a = curr->op1.a->entry->type->First->Right;
                    genOperands(i1,1,4,1,1,f1,0);
                    fprintf(f1,"mov ecx,eax\n");
                }
                if(curr->op1.a->entry->type->First->Right->Right->tag==_NUM){
                    fprintf(f1,"mov edx,%d\n",(int)curr->op1.a->entry->type->First->Right->Right->t->value.integer);
                }
                else{
                    IrCode* i1 = createInst();
                    i1->tag1 = 1;
                    i1->op1.a = curr->op1.a->entry->type->First->Right->Right;
                    genOperands(i1,1,4,1,1,f1,0);
                    fprintf(f1,"mov edx,eax\n");
                }

                fprintf(f1,"%s%d:\n","T",currTemp);
                int label = currTemp;
                currTemp++;

                fprintf(f1,"mov r14,rcx\n");
                fprintf(f1,"mov r15,rdx\n");

                if(type==4){
                    genOperands(curr,1,4,1,1,f1,1);
                    fprintf(f1,"mov edi,formatintarr\n");
                    fprintf(f1,"mov esi,eax\n");
                    //fprintf(f1,"xor rax,rax\n");
                    fprintf(f1,"xor rax,rax\n");
                    fprintf(f1,"call printf\n");
                }
                else if(type==5){
                    genOperands(curr,1,8,1,1,f1,1);
                    fprintf(f1,"mov rdi,formatfloatarr\n");
                    fprintf(f1,"mov rsi,rax\n");
                    //fprintf(f1,"xor rax,rax\n");
                    fprintf(f1,"xor rax,rax\n");
                    fprintf(f1,"call printf\n");
                }
                else{
                    fprintf(f1,"xor eax,eax\n");
                    genOperands(curr,1,2,1,1,f1,1);
                    fprintf(f1,"mov rdi,formatboolarr\n");
                    fprintf(f1,"cmp ax,0\n");
                    fprintf(f1,"je printfalse%d\n",currTemp);
                    fprintf(f1,"printtrue%d: mov rsi,true\n",currTemp);
                    fprintf(f1,"jmp next%d\n",currTemp);
                    fprintf(f1,"printfalse%d: mov rsi,false\n",currTemp);
                    fprintf(f1,"next%d:\n",currTemp);
                    currTemp++;
                    fprintf(f1,"xor rax,rax\n");
                    fprintf(f1,"call printf\n");
                }

                fprintf(f1,"mov rdx,r15\n");
                fprintf(f1,"mov rcx,r14\n");

                fprintf(f1,"inc ecx\n");
                fprintf(f1,"cmp ecx,edx\n");
                fprintf(f1,"jle T%d\n",label);
                fprintf(f1,"mov rdi,formatenter\n");
                fprintf(f1,"xor rax,rax\n");
                fprintf(f1,"call printf\n");
            }
        }
        else if(curr->Op==OGETVALUE){
            int type = getType(curr->op1.a);
            if(type==1){
                fprintf(f1,"mov rdi,formatgetval\n");
                fprintf(f1,"mov rsi,formatinttext\n");
                fprintf(f1,"xor rax,rax\n");
                fprintf(f1,"call printf\n");

                    fprintf(f1,"mov rdi,formatdollar\n");
                    fprintf(f1,"xor rax,rax\n");
                    fprintf(f1,"call printf\n");

                    fprintf(f1,"lea rdi,[formatinint]\n");
                    fprintf(f1,"lea rsi,[integer1]\n");
                    fprintf(f1,"xor rax,rax\n");
                    fprintf(f1,"call scanf\n");
                    fprintf(f1,"mov eax,[integer1]\n");
                    genOperands(curr,1,4,1,0,f1,0);
                    
            }
            else if(type==2){
                    fprintf(f1,"mov rdi,formatgetval\n");
                    fprintf(f1,"mov rsi,formatfloattext\n");
                    fprintf(f1,"xor rax,rax\n");
                    fprintf(f1,"call printf\n");

                    fprintf(f1,"mov rdi,formatdollar\n");
                    fprintf(f1,"xor rax,rax\n");
                    fprintf(f1,"call printf\n");

                    fprintf(f1,"lea rdi,[formatinfloat]\n");
                    fprintf(f1,"lea rsi,[float1]\n");
                    fprintf(f1,"xor rax,rax\n");
                    fprintf(f1,"call scanf\n");
                    fprintf(f1,"mov rax,[float1]\n");
                    genOperands(curr,1,8,1,0,f1,0);
            }
            else if(type==3){
                    fprintf(f1,"mov rdi,formatgetval\n");
                    fprintf(f1,"mov rsi,formatbooltext\n");
                    fprintf(f1,"xor rax,rax\n");
                    fprintf(f1,"call printf\n");

                    fprintf(f1,"mov rdi,formatdollar\n");
                    fprintf(f1,"xor rax,rax\n");
                    fprintf(f1,"call printf\n");

                    fprintf(f1,"lea rdi,[formatinbool]\n");
                    fprintf(f1,"lea rsi,[boolean1]\n");
                    fprintf(f1,"xor rax,rax\n");
                    fprintf(f1,"call scanf\n");
                    fprintf(f1,"mov ax,[boolean1]\n");
                    genOperands(curr,1,2,1,0,f1,0);
            }
            else{
                if(curr->op1.a->entry->type->First->Right->tag==_NUM){
                    fprintf(f1,"mov ecx,%d\n",(int)curr->op1.a->entry->type->First->Right->t->value.integer);
                }
                else{
                    IrCode* i1 = createInst();
                    i1->tag1 = 1;
                    i1->op1.a = curr->op1.a->entry->type->First->Right;
                    genOperands(i1,1,4,1,1,f1,0);
                    fprintf(f1,"mov ecx,eax\n");
                }
                if(curr->op1.a->entry->type->First->Right->Right->tag==_NUM){
                    fprintf(f1,"mov edx,%d\n",(int)curr->op1.a->entry->type->First->Right->Right->t->value.integer);
                }
                else{
                    IrCode* i1 = createInst();
                    i1->tag1 = 1;
                    i1->op1.a = curr->op1.a->entry->type->First->Right->Right;
                    genOperands(i1,1,4,1,1,f1,0);
                    fprintf(f1,"mov edx,eax\n");
                }
                fprintf(f1,"sub edx,ecx\n");
                fprintf(f1,"inc edx\n");

                fprintf(f1,"mov edi,formatgetvalarr1\n");
                //Should it be rdi only??
                fprintf(f1,"mov esi,edx\n");
                if(type==4){
                    fprintf(f1,"mov edx,formatinttext\n");
                }
                else if(type==5){
                    fprintf(f1,"mov edx,formatfloattext\n");
                }
                else{
                    fprintf(f1,"mov edx,formatbooltext\n");
                }
                fprintf(f1,"xor rax,rax\n");
                fprintf(f1,"call printf\n");


                if(curr->op1.a->entry->type->First->Right->tag==_NUM){
                    fprintf(f1,"mov ecx,%d\n",(int)curr->op1.a->entry->type->First->Right->t->value.integer);
                }
                else{
                    IrCode* i1 = createInst();
                    i1->tag1 = 1;
                    i1->op1.a = curr->op1.a->entry->type->First->Right;
                    genOperands(i1,1,4,1,1,f1,0);
                    fprintf(f1,"mov ecx,eax\n");
                }
                if(curr->op1.a->entry->type->First->Right->Right->tag==_NUM){
                    fprintf(f1,"mov edx,%d\n",(int)curr->op1.a->entry->type->First->Right->Right->t->value.integer);
                }
                else{
                    IrCode* i1 = createInst();
                    i1->tag1 = 1;
                    i1->op1.a = curr->op1.a->entry->type->First->Right->Right;
                    genOperands(i1,1,4,1,1,f1,0);
                    fprintf(f1,"mov edx,eax\n");
                }

                fprintf(f1,"mov edi,formatgetvalarr2\n");
                fprintf(f1,"mov esi,ecx\n");
                fprintf(f1,"mov edx,edx\n");
                //is the 2nd param edx itself?
                //fprintf(f1,"xor rax,rax\n");
                fprintf(f1,"xor rax,rax\n");
                fprintf(f1,"call printf\n");

                if(curr->op1.a->entry->type->First->Right->tag==_NUM){
                    fprintf(f1,"mov ecx,%d\n",(int)curr->op1.a->entry->type->First->Right->t->value.integer);
                }
                else{
                    IrCode* i1 = createInst();
                    i1->tag1 = 1;
                    i1->op1.a = curr->op1.a->entry->type->First->Right;
                    genOperands(i1,1,4,1,1,f1,0);
                    fprintf(f1,"mov ecx,eax\n");
                }
                if(curr->op1.a->entry->type->First->Right->Right->tag==_NUM){
                    fprintf(f1,"mov edx,%d\n",(int)curr->op1.a->entry->type->First->Right->Right->t->value.integer);
                }
                else{
                    IrCode* i1 = createInst();
                    i1->tag1 = 1;
                    i1->op1.a = curr->op1.a->entry->type->First->Right->Right;
                    genOperands(i1,1,4,1,1,f1,0);
                    fprintf(f1,"mov edx,eax\n");
                }

                fprintf(f1,"T%d:",currTemp);
                int label = currTemp;
                currTemp++;

                fprintf(f1,"mov r14,rcx\n");
                fprintf(f1,"mov r15,rdx\n");                

                if(type==4){
                    fprintf(f1,"mov rdi,formatdollar\n");
                    fprintf(f1,"xor rax,rax\n");
                    fprintf(f1,"call printf\n");

                    fprintf(f1,"lea rdi,[formatinint]\n");
                    fprintf(f1,"lea rsi,[integer1]\n");
                    fprintf(f1,"xor rax,rax\n");
                    fprintf(f1,"call scanf\n");
                    fprintf(f1,"mov eax,[integer1]\n");
                    fprintf(f1,"mov rdx,r15\n");
                    fprintf(f1,"mov rcx,r14\n");
                    genOperands(curr,1,4,1,0,f1,1);
                }
                else if(type==5){
                    fprintf(f1,"mov rdi,formatdollar\n");
                    fprintf(f1,"xor rax,rax\n");
                    fprintf(f1,"call printf\n");

                    fprintf(f1,"lea rdi,[formatinfloat]\n");
                    fprintf(f1,"lea rsi,[float1]\n");
                    fprintf(f1,"xor rax,rax\n");
                    fprintf(f1,"call scanf\n");
                    fprintf(f1,"mov rax,[float1]\n");
                    fprintf(f1,"mov rdx,r15\n");
                    fprintf(f1,"mov rcx,r14\n");
                    genOperands(curr,1,8,1,0,f1,1);    
                }
                else{
                    fprintf(f1,"mov rdi,formatdollar\n");
                    fprintf(f1,"xor rax,rax\n");
                    fprintf(f1,"call printf\n");

                    fprintf(f1,"lea rdi,[formatinbool]\n");
                    fprintf(f1,"lea rsi,[boolean1]\n");
                    fprintf(f1,"xor rax,rax\n");
                    fprintf(f1,"call scanf\n");
                    fprintf(f1,"mov ax,[boolean1]\n");
                    fprintf(f1,"mov rdx,r15\n");
                    fprintf(f1,"mov rcx,r14\n");
                    genOperands(curr,1,2,1,0,f1,1);
                }

                fprintf(f1,"mov rdx,r15\n");
                fprintf(f1,"mov rcx,r14\n");

                fprintf(f1,"inc ecx\n");
                fprintf(f1,"cmp ecx,edx\n");
                fprintf(f1,"jle T%d\n",label);

            }
        }
        else{
            printf("Floating point instructions not taken care of.\n");
            fprintf(f1,";Floating point instructions not taken care of.\n");
        }
        
        curr = curr->next;
    }
    fprintf(f1,"\n");
    fclose(f1);
}

void genOperands(IrCode* code,int op,int size,int reg1,int dir,FILE* f1,int option){
    fprintf(f1,"mov ");
    int tag=0;
    astNode* node=NULL;
    enum reg R;
    int offset=0;
    double real;
    int num = 0;
    bool boolVal = 0;
    int flag = 0;

    if(dir==1){
        if(size==4){
            fprintf(f1,"e");
        }
        else if(size==8){
            fprintf(f1,"r");
        }

        if(reg1==1){
            fprintf(f1,"ax");
        }
        else if(reg1==2){
            fprintf(f1,"bx");
        }
        else if(reg1==3){
            fprintf(f1,"cx");
        }
        else if(reg1==4){
            fprintf(f1,"dx");
        }

        fprintf(f1,",");

        if(op==1){
            tag = code->tag1;
            if(tag==1){
                node = code->op1.a;
            }
            else if(tag==2){
                if(size==2){
                    boolVal = code->op1.boolVal;
                }
                else if(size==8){
                    real = code->op1.real;
                }
                else{
                    num = code->op1.num;
                }
            }
            else if(tag==4 || tag==5){
                offset = code->op1.offset;
            }
            else if(tag==3){
                R = code->op1.R;
            }
            else{
                offset = code->op1.offset;
            }
        }
        else if(op==2){
            tag = code->tag2;
            if(tag==1){
                node = code->op2.a;
            }
            else if(tag==2){
                if(size==2){
                    boolVal = code->op2.boolVal;
                }
                else if(size==8){
                    real = code->op1.real;
                }
                else{
                    num = code->op2.num;
                }
            }
            else if(tag==4 || tag==5){
                offset = code->op2.offset;
            }
            else if(tag==3){
                R = code->op2.R;
            }
            else{
                offset = code->op2.offset;
            }
        }
        else if(op==3){
            tag = code->tag3;
            if(tag==1){
                node = code->op3.a;
            }
            else if(tag==2){
                if(size==2){
                    boolVal = code->op3.boolVal;
                }
                else if(size==8){
                    real = code->op1.real;
                }
                else{
                    num = code->op3.num;
                }
            }
            else if(tag==4 || tag==5){
                offset = code->op3.offset;
            }
            else if(tag==3){
                R = code->op3.R;
            }
            else{
                offset = code->op3.offset;
            }
        }

        if(tag==3){
            switch(R){
                case RSP:
                    fprintf(f1,"rsp");
                    break;
                case RBP:
                    fprintf(f1,"rbp");
                    break;
                case EDX:
                    fprintf(f1,"edx");
                    break;
                case RDX:
                    fprintf(f1,"rdx");
                    break;
                case RCX:
                    fprintf(f1,"rcx");
                    break;
                case EAX:
                    fprintf(f1,"eax");
                    break;
            }
        }
        else if(tag==4 || tag==5){
            fprintf(f1,"[");
            if(tag==4){
                fprintf(f1,"rsp");
            }
            else{
                fprintf(f1,"rbp");
            }
            if(offset>=0){
                fprintf(f1,"+%d",offset);
            }
            else{
                fprintf(f1,"%d",offset);
            }
            fprintf(f1,"]");
        }
        else if(tag==2){
            if(size==2){
                fprintf(f1,"%d",boolVal);
            }
            else{
                fprintf(f1,"%d",num);
            }    
        }
        else if(tag==6){
            fprintf(f1,"%d",offset);
        }
        else if(tag==1){
            if((node->tag==_ID && option==1) || (node->tag==_ID && node->First)){
                fprintf(f1,"0\n");
                fprintf(f1,"mov r10,rax\n");
                fprintf(f1,"mov r9,rsp\n");

                fprintf(f1,"xor rax,rax\n");

                fprintf(f1,"mov eax,");
                if(option==1){
                    fprintf(f1,"ecx");
                }
                else if(node->First->tag==_NUM){
                    fprintf(f1,"%d",(int)node->First->t->value.integer);
                }
                else{
                    fprintf(f1,"[rbp");
                    if(node->First->entry->io==1){
                        fprintf(f1,"+%d",SIZE_AFTER_BP+node->First->entry->offset);
                    }
                    else{
                        fprintf(f1,"-%d",node->First->entry->offset+SIZE_OF_INTEGER);
                    }
                    fprintf(f1,"]");
                }
                fprintf(f1,"\n");

                fprintf(f1,"sub eax,");
                if(node->entry->type->First->Right->tag==_NUM){
                    fprintf(f1,"%d",(int)node->entry->type->First->Right->t->value.integer);
                }
                else{
                    fprintf(f1,"[rbp");
                    if(node->entry->type->First->Right->entry->io==1){
                        fprintf(f1,"+%d",SIZE_AFTER_BP+node->entry->type->First->Right->entry->offset);
                    }
                    else{
                        fprintf(f1,"-%d",node->entry->type->First->Right->entry->offset+SIZE_OF_INTEGER);
                    }
                    fprintf(f1,"]");
                }
                fprintf(f1,"\n");

                //fprintf(f1,"xor rbx,rbx\n");
                fprintf(f1,"mov r13,rbx\n");
                
                fprintf(f1,"mov ebx,");
                if(node->entry->type->First->tag==_INTEGER){
                    fprintf(f1,"%d",SIZE_OF_INTEGER);
                }
                else if(node->entry->type->First->tag==_BOOLEAN){
                    fprintf(f1,"%d",SIZE_OF_BOOLEAN);
                }
                else{
                    fprintf(f1,"%d",SIZE_OF_REAL);
                }
                fprintf(f1,"\n");

                fprintf(f1,"mul ebx\n");

                fprintf(f1,"mov rbx,r13\n");

                fprintf(f1,"mov rsp,");
                fprintf(f1,"[rbp");
                if(node->entry->io==1){
                    fprintf(f1,"+%d]",SIZE_AFTER_BP+node->entry->offset);
                }
                else{
                    fprintf(f1,"-%d]",node->entry->offset+SIZE_OF_POINTER);
                }
                fprintf(f1,"\n");

                fprintf(f1,"sub rsp,rax\n");

                fprintf(f1,"sub rsp,");
                if(node->entry->type->First->tag==_INTEGER){
                    fprintf(f1,"%d",SIZE_OF_INTEGER);
                }
                else if(node->entry->type->First->tag==_BOOLEAN){
                    fprintf(f1,"%d",SIZE_OF_BOOLEAN);
                }
                else{
                    fprintf(f1,"%d",SIZE_OF_REAL);
                }
                fprintf(f1,"\n");

                fprintf(f1,"mov rax,r10\n");

                fprintf(f1,"mov ");

                if(size==4){
                    fprintf(f1,"e");
                }
                else if(size==8){
                    fprintf(f1,"r");
                }

                if(reg1==1){
                    fprintf(f1,"ax");
                }
                else if(reg1==2){
                    fprintf(f1,"bx");
                }
                else if(reg1==3){
                    fprintf(f1,"cx");
                }
                else if(reg1==4){
                    fprintf(f1,"dx");
                }

                fprintf(f1,",");

                fprintf(f1,"[rsp]\n");

                fprintf(f1,"mov rsp,r9\n");
            }
            else{
                fprintf(f1,"[rbp");
                if(node->entry->io==1){
                    fprintf(f1,"+%d",SIZE_AFTER_BP+node->entry->offset);
                }
                else{
                    fprintf(f1,"-%d",node->entry->offset+size);
                }
                fprintf(f1,"]");
            }
        }
        fprintf(f1,"\n");
    }
    else if(dir==0){
        if(op==1){
            tag = code->tag1;
            if(tag==1){
                node = code->op1.a;
            }
            else if(tag==2){
                if(size==2){
                    boolVal = code->op1.boolVal;
                }
                else if(size==8){
                    real = code->op1.real;
                }
                else{
                    num = code->op1.num;
                }
            }
            else if(tag==4 || tag==5){
                offset = code->op1.offset;
            }
            else if(tag==3){
                R = code->op1.R;
            }
            else{
                offset = code->op1.offset;
            }
        }
        else if(op==2){
            tag = code->tag2;
            if(tag==1){
                node = code->op2.a;
            }
            else if(tag==2){
                if(size==2){
                    boolVal = code->op2.boolVal;
                }
                else if(size==8){
                    real = code->op1.real;
                }
                else{
                    num = code->op2.num;
                }
            }
            else if(tag==4 || tag==5){
                offset = code->op2.offset;
            }
            else if(tag==3){
                R = code->op2.R;
            }
            else{
                offset = code->op2.offset;
            }
        }
        else if(op==3){
            tag = code->tag3;
            if(tag==1){
                node = code->op3.a;
            }
            else if(tag==2){
                if(size==2){
                    boolVal = code->op3.boolVal;
                }
                else if(size==8){
                    real = code->op1.real;
                }
                else{
                    num = code->op3.num;
                }
            }
            else if(tag==4 || tag==5){
                offset = code->op3.offset;
            }
            else if(tag==3){
                R = code->op3.R;
            }
            else{
                offset = code->op3.offset;
            }
        }

        if(tag==3){
            switch(R){
                case RSP:
                    fprintf(f1,"rsp");
                    break;
                case RBP:
                    fprintf(f1,"rbp");
                    break;
                case EAX:
                    fprintf(f1,"eax");
                    break;
                case RCX:
                    fprintf(f1,"rcx");
                    break;
                case RDX:
                    fprintf(f1,"rdx");
                    break;
                case EDX:
                    fprintf(f1,"edx");
                    break;
            }
        }
        else if(tag==4 || tag==5){
            fprintf(f1,"[");
            if(tag==4){
                fprintf(f1,"rsp");
            }
            else{
                fprintf(f1,"rbp");
            }
            if(offset>=0){
                fprintf(f1,"+%d",offset);
            }
            else{
                fprintf(f1,"%d",offset);
            }
            fprintf(f1,"]");
        }
        else if(tag==2){
            if(size==2){
                fprintf(f1,"%d",boolVal);
            }
            else{
                fprintf(f1,"%d",num);
            }    
        }
        else if(tag==6){
            fprintf(f1,"%d",offset);
        }
        else if(tag==1){
            if((node->tag==_ID && option==1) || (node->tag==_ID && node->First)){
                fprintf(f1,"rax,rax\n");
                flag = 1;
                fprintf(f1,"mov r10,rax\n");
                fprintf(f1,"mov r9,rsp\n");

                //fprintf(f1,"xor rax,rax\n");

                fprintf(f1,"mov eax,");
                if(option==1){
                    fprintf(f1,"ecx");
                }
                else if(node->First->tag==_NUM){
                    fprintf(f1,"%d",(int)node->First->t->value.integer);
                }
                else{
                    fprintf(f1,"[rbp");
                    if(node->First->entry->io==1){
                        fprintf(f1,"+%d",SIZE_AFTER_BP+node->First->entry->offset);
                    }
                    else{
                        fprintf(f1,"-%d",node->First->entry->offset+SIZE_OF_INTEGER);
                    }
                    fprintf(f1,"]");
                }
                fprintf(f1,"\n");

                fprintf(f1,"sub eax,");
                if(node->entry->type->First->Right->tag==_NUM){
                    fprintf(f1,"%d",(int)node->entry->type->First->Right->t->value.integer);
                }
                else{
                    fprintf(f1,"[rbp");
                    if(node->entry->type->First->Right->entry->io==1){
                        fprintf(f1,"+%d",SIZE_AFTER_BP+node->entry->type->First->Right->entry->offset);
                    }
                    else{
                        fprintf(f1,"-%d",node->entry->type->First->Right->entry->offset+SIZE_OF_INTEGER);
                    }
                    fprintf(f1,"]");
                }
                fprintf(f1,"\n");

                //fprintf(f1,"xor rbx,rbx\n");
                fprintf(f1,"mov r13,rbx\n");
                
                fprintf(f1,"mov ebx,");
                if(node->entry->type->First->tag==_INTEGER){
                    fprintf(f1,"%d",SIZE_OF_INTEGER);
                }
                else if(node->entry->type->First->tag==_BOOLEAN){
                    fprintf(f1,"%d",SIZE_OF_BOOLEAN);
                }
                else{
                    fprintf(f1,"%d",SIZE_OF_REAL);
                }
                fprintf(f1,"\n");

                fprintf(f1,"mul ebx\n");

                fprintf(f1,"mov rbx,r13\n");

                fprintf(f1,"mov rsp,");
                fprintf(f1,"[rbp");
                if(node->entry->io==1){
                    fprintf(f1,"+%d]",SIZE_AFTER_BP+node->entry->offset);
                }
                else{
                    fprintf(f1,"-%d]",node->entry->offset+SIZE_OF_POINTER);
                }
                fprintf(f1,"\n");

                fprintf(f1,"sub rsp,rax\n");

                fprintf(f1,"sub rsp,");
                if(node->entry->type->First->tag==_INTEGER){
                    fprintf(f1,"%d",SIZE_OF_INTEGER);
                }
                else if(node->entry->type->First->tag==_BOOLEAN){
                    fprintf(f1,"%d",SIZE_OF_BOOLEAN);
                }
                else{
                    fprintf(f1,"%d",SIZE_OF_REAL);
                }
                fprintf(f1,"\n");

                fprintf(f1,"mov rax,r10\n");

                fprintf(f1,"mov ");

                fprintf(f1,"[rsp]");

                fprintf(f1,",");

                if(size==4){
                    fprintf(f1,"e");
                }
                else if(size==8){
                    fprintf(f1,"r");
                }

                if(reg1==1){
                    fprintf(f1,"ax");
                }
                else if(reg1==2){
                    fprintf(f1,"bx");
                }
                else if(reg1==3){
                    fprintf(f1,"cx");
                }
                else if(reg1==4){
                    fprintf(f1,"dx");
                }

                fprintf(f1,"\n");

                fprintf(f1,"mov rsp,r9\n");
            }
            else{
                fprintf(f1,"[rbp");
                if(node->entry->io==1){
                    fprintf(f1,"+%d",SIZE_AFTER_BP+node->entry->offset);
                }
                else{
                    fprintf(f1,"-%d",node->entry->offset+size);
                }
                fprintf(f1,"]");
            }
        }
        
        if(flag==1)return;

        fprintf(f1,",");
    
        if(size==4){
            fprintf(f1,"e");
        }
        else if(size==8){
            fprintf(f1,"r");
        }

        if(reg1==1){
            fprintf(f1,"ax\n");
        }
        else if(reg1==2){
            fprintf(f1,"bx\n");
        }
        else if(reg1==3){
            fprintf(f1,"cx\n");
        }
        else if(reg1==4){
            fprintf(f1,"dx\n");
        }

        fprintf(f1,"\n");

    }

}