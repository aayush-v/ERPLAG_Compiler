// Group Number - 25
// Name : Aayush Atul Verma         ID : 2017A7PS0061P
// Name : Aditya Ramaswamy          ID : 2017A7PS0130P
// Name : Jithin Kallukalam Sojan   ID : 2017A7PS0163P

#include <string.h>
#include <stdlib.h>
#include<ctype.h>
#include <stdbool.h>
#include "parser.h"
#include "lexer.h"
#include "Lexer-Accessory.h"
#include "Parser-Accessory.h"
#include "stack.h"

Grammar* createGrammar(){
	//This function creates the grammar from the file that contains the grammar.
    Grammar* g_set;
    g_set = malloc(sizeof(Grammar));

    FILE *fname = fopen("grammar.txt", "r");
    if (fname == NULL){
        printf("Exit");
    }

    char str[25];
    char c = 'a';

    for(int i = 0; i<NO_OF_NT;i++){
        int count = 0;
        Cell* traverse;
        Cell* node;
        Cell* previous = NULL;

        fscanf(fname, "%s", str);
        c = fgetc(fname);
        
        while(1){
            node = malloc(sizeof(Cell));
            if(count==0){
                (*g_set)[i] = node;
            }
            else{
                previous->next = node;
            }
            fscanf(fname, "%s", str);
           
            int j;
            if(str[0] == '<'){
                for( j= 0; j<(strlen(str)-1) ; j++){
                    str[j] = str[j+1];
                }
                str[j-1]= '\0';
                node->tag = 1;
                (node->value).NT = stringToNonTerminal(str);
            }
            else if(str[0] == '|'){
                node->tag = 3;
            }
            else if(str[0] == '~'){
                node->tag = 4;
            }
            else{
                node->tag = 2;
                (node->value).T = stringToTerminal(str);
            }
            
            c = fgetc(fname);
            if( c == '\n'){
                node->next = NULL;
                break;
            }
            if(c == EOF){
				node->next = NULL;
                break;
            }
            previous = node;
            count++;
        }
    }
    return g_set;
}

void ComputeFirstAndFollowSets (Grammar* G, FirstAndFollow* F){
	//It computes the First and Follow sets for each NT in the grammar G in the structure FirstAndFollow F.
	unsigned long long int temp[NO_OF_NT];

    for(int i = 0;i<NO_OF_NT;i++){
		F->First[i] = (unsigned long long int)0;
		F->Follow[i] = (unsigned long long int)0;
		temp[i] = (unsigned long long int)0;
	}

	for (int i = 0;i<NO_OF_NT;i++){
		ComputeFirst(G,F,(Non_Terminal)i);
	}

	//Adding the $ to the set.
	F->Follow[0]|=((unsigned long long int)1<<NO_OF_T);
	temp[0]|=((unsigned long long int)1<<NO_OF_T);
	
	while(1){
		ComputeFollow(G,temp,F);
		int same = 1;
		for(int i = 0;i<NO_OF_NT;i++){
			if(temp[i]!=F->Follow[i]){
				same = 0;
				break;
			}
		}
		if(same==1)break;
		else{
			for(int j = 0;j<NO_OF_NT;j++){
				F->Follow[j]= temp[j];
			}
		}
	}
	return;
}

void ComputeFirst(Grammar* G, FirstAndFollow* F, Non_Terminal NT){
	//Calculates the First of the NonTerminal NT and places it in the corresponding position in F.
	if(F->First[NT]!=(unsigned long long int)0)return;

	int or = 0; //If this value is one, we move ahead until we find an OR or the grammar rule ends.
	Cell* previous;
	Cell* current;
	unsigned long long int temp;

	current = (*G)[NT];
	while(current!=NULL){
		if(or==1){
			if(current->tag == 3){
				or = 0;
			}
		}
		else{
			if(current->tag == 4){
				F->First[NT]|=((unsigned long long int)1<<56);
				or = 1;
			}
			else if(current->tag == 3){
				F->First[NT]|=((unsigned long long int)1<<56);
			}
			else if(current->tag == 2){
				F->First[NT]|=((unsigned long long int)1<<((current->value).T));
				or = 1;
			}
			else{
				//If the same NT is encountered, we will end up in an infinte loop. This case should ideally not happen in the first place.
				if(((current->value).NT)!=NT){
					//To show this has already been visited.
					ComputeFirst(G,F,(current->value).NT);
					if((F->First[(current->value).NT] & ((unsigned long long int)1<<NO_OF_T))==(unsigned long long int)0){
						or = 1;
						F->First[NT]|=F->First[(current->value).NT];
					}
					else F->First[NT]|=(F->First[(current->value).NT]^((unsigned long long int)1<<NO_OF_T));
				}
			}
		}
		previous = current;
		current = current->next;
	}
	if(or==0){
		F->First[NT]|=((unsigned long long int)1<<56);
	}
	return;	
}

void freeCells(Grammar* G){
	//if(P==NULL)return;
	Cell* prev;
	Cell* temp;
	for(int i = 0;i<NO_OF_NT;i++){
		//printf("Hi There\n");
		temp = (*G)[i];
		while(temp){
			prev = temp;
			temp = temp->next;
			free(prev);
		
		}	
	}
	free(G);
	return;
}

unsigned long long int ComputeFirstSet(FirstAndFollow* F, Cell* C){
	//This function computes the first of a group of terminals and non-terminals usually found in the RHS of rules.
	Cell* current = C;
	unsigned long long int first = (unsigned long long int)0;
	unsigned long long int temp;
	int empty = 1;

	while(current!=NULL){
		if(current->tag==2){
			first|=((unsigned long long int)1<<(current->value).T);
			empty = 0;
			break;
		}
		else if(current->tag==4){
			first|=((unsigned long long int)1<<NO_OF_T);	
			//The 1 has to be shifted to the number of terminals.
			empty = 0;
			break;
		}
		else{
			temp = F->First[(current->value).NT];
			if((temp & ((unsigned long long int)1<<NO_OF_T))==(unsigned long long int)0){
				//This means that the NT doesn't have empty in its first set.
				empty = 0;
				first|=temp;
				break;
			}
			else first|=(temp^((unsigned long long int)1<<NO_OF_T));
		}
		current = current->next;
	}
	if(empty){
		//This means that the entire rule is NTs with empty in it.
		first|=((unsigned long long int)1<<NO_OF_T);
	}

	return first;
}

void ComputeFollow(Grammar* G, unsigned long long int temp[], FirstAndFollow* F){
	//Computes the follow sets for each NT in the Grammar and places them in the temp array.

	for(int i = 0;i<NO_OF_NT;i++){
		Cell* current;
		Cell* after;
		Cell* dummy;
		Cell* dummy2;
		unsigned long long int first;

		current = (*G)[i];
		while(current!=NULL){
			//Calculate the follow for current.
			if(current->tag==1){
				dummy = current->next;
				if(dummy==NULL || dummy->tag==3){					
					temp[(current->value).NT]|=temp[i];
				}
				else if(dummy->tag == 2){
					temp[(current->value).NT]|=((unsigned long long int)1<<(dummy->value).T);
				}
				else if(dummy->tag==1){
					while(dummy->next!=NULL && (dummy->next)->tag!=3){
						dummy = dummy->next;
					}
					after = dummy->next;
					dummy->next = NULL;
					//Creating a lone group out of the NTs and Ts in the rule.
					first = ComputeFirstSet(F,current->next);
					dummy->next = after;
					if(first & ((unsigned long long int)1<<NO_OF_T)){
						temp[(current->value).NT]|=(first^((unsigned long long int)1<<NO_OF_T));
						temp[(current->value).NT]|=temp[i];
					}
					else{
						temp[(current->value).NT]|=first;
					}
				}
				else{
					printf("This should never happen.\n");
				}
			}
			current = current->next;
		}
	}
}

void createParseTable(Grammar* G, FirstAndFollow* F, ParseTable P, RuleNum R){
	Cell* syn;
	syn = (Cell*)malloc(sizeof(Cell));
	syn->next = NULL;
	syn->tag = 5;

	int count = 1;

	for(int i = 0;i<NO_OF_NT;i++){
		for(int j = 0;j<NO_OF_T+1;j++){
			P[i][j] = NULL;
		}
	}

	for(int i = 0;i<NO_OF_NT;i++){
		count = 1;
		Cell* current = (*G)[i];
		Cell* after;
		Cell* dummy;
		unsigned long long int first;
		unsigned long long int follow;

		while(current){
			if(current->tag==3){
				count++;
				current = current->next;
			}
			else{
				dummy = current;
				while(dummy->next!=NULL && (dummy->next)->tag!=3){
					dummy = dummy->next;
				}
				after = dummy->next;
				dummy->next = NULL;
				first = ComputeFirstSet(F,current);
				for(int j = 0;j<NO_OF_T;j++){
					if(first%2==1){
						P[i][j] = current;
						R[i][j] = count;
					}
					first = first>>1;
				}
				follow = F->Follow[i];
				for(int j = 0;j<NO_OF_T+1;j++){
					if(follow%2==1){
						if(first%2==1){
							if(P[i][j]==NULL){
								P[i][j] = current;
								R[i][j] = count;
							}
							//Ideally there shoudn't be an intersection between the two.
						}
					}
					follow = follow>>1;
				}
				current = after;
			}
		}
	}
}

void PrintFirstAndFollow(FirstAndFollow* F){
	//A function defined to print the First and Follow sets of the Non-Terminals. Basically a function used for testing purposes.
	unsigned long long int first;
	unsigned long long int follow;

	//Printing the firsts of each Non-Terminal
	printf("Firsts:\n");
	for(int i = 0;i<NO_OF_NT;i++){
		char* a = NonTerminalToString(i);
		printf("%s",a);
		printf(" = ");
		first = F->First[i];
		for(int j = 0;j<NO_OF_T;j++){
			if(first%2==1){
				char* b =TerminalToString(j);
				printf("%s",b);
				printf(" ");
			}
			first = (first>>1);
		}
		if(first%2==1){
			printf("~ ");
		}
		printf("\n");
	}

	//Printing the follow sets of each of the Non-Terminals
	printf("\n");
	printf("Follows:\n");
	for(int i = 0;i<NO_OF_NT;i++){
		char* c = NonTerminalToString(i);
		printf("%s",c);
		printf(" = ");
		follow = F->Follow[i];
		for(int j = 0;j<NO_OF_T;j++){
			if(follow%2==1){
				char* d = TerminalToString(j);
				printf("%s",d);
				printf(" ");
			}
			follow = (follow>>1);
		}
		if(follow%2==1){
			printf("$ ");
		}
		printf("\n");
	}
}



ParseNode* parseInputSourceCode(char* testcaseFile, ParseTable P, FirstAndFollow* F, RuleNum R, int* error){
	stack* Stack = initStack(50);
	ParseNode* Root;

	Cell* program = (Cell*)malloc(sizeof(Cell));
	program->tag=1;
	program->next = NULL;
	(program->value).NT = NTPROGRAM;

	Root = (ParseNode*)malloc(sizeof(ParseNode)*1);
	Root->Parent = NULL;
	Root->Node = NULL;
	Root->inh = NULL;
	Root->First = NULL;
	Root->Right = NULL;
	Root->t = NULL;
	Root->rule1 = -1;
	//This represents that the node is the root "program" node.
	Root->rule2 = 0;
	Root->c = program;
	pushIt(Stack,Root);

    FILE *input=fopen(testcaseFile,"r");
    int line=1;
	bool DDot=false;
	Token* t;
	ParseNode* curr;
	ParseNode* start;
	ParseNode* dummy;
	ParseNode* temp;
	ParseNode* previous;
	Cell* rule;
	
	int NextTok = 1;
	int NonTermError = 0;
	int err = 0;
	while(1)
	{
		if(NextTok){
			t = (Token*)malloc(sizeof(Token));
			*t=Lexer(input, &line, &DDot);
			(*t).line_no = line;
		}
		curr = NULL;
		temp = NULL;
		dummy = NULL;
		previous = NULL;
		if(t->error==true){
			if(t->lex==ID && ((t->value).identifier[0]=='_'||isalnum((t->value).identifier[0])))
			{
				printf("Line %d: Lexical error, Length of identifier exceeded.\n",(t->length));
			}
			else{
				printf("Line %d :Lexical error, unexpected character '%c' while parsing a %s\n", t->length, (t->value).identifier[0], LexemeToString(t->lex));
			}
			err = 1;
		}
		else{
			
			curr = top(Stack);
			if(NonTermError){
				int term = TokToTerm(t);
				if((F->Follow[((curr->c)->value).NT]&((unsigned long long int)1<<term))||t->lex == END_OF_INPUT_STREAM){
					NonTermError = 0;
					NextTok = 0;
					curr = popIt(Stack);
				}
				continue;
			}
			if(curr==NULL){
				if(t->lex == END_OF_INPUT_STREAM){
					printf("Parsing done.\nParse Tree created.\n");
					break;
				}
				else{
					err = 1;
					printf("Line %d: Syntactic error, unexpected ",line);
					if(t->lex==KEYWORD)
						printf("%s%s",LexemeToString(t->lex),KeywordToString(t->value.key));
					else if(t->lex==ID)
						printf("%s%s",LexemeToString(t->lex),t->value.identifier);
					else if(t->lex==NUM)
						printf("%s%lld",LexemeToString(t->lex),t->value.integer);
					else if(t->lex==RNUM)
						printf("%s%f",LexemeToString(t->lex),t->value.real);
					else
						printf("%s",LexemeToString(t->lex));
					printf("\n");
				}
			}
			else if((curr->c)->tag==2){
				if(term_cmp(t,((curr->c)->value).T)){
					//term_cmp compares terminals and lexemes.
					curr->t = t;
					curr = popIt(Stack);
					NextTok = 1;
				}
				else{
					//possibility of error
					//only prints lexeme. Expand to print keyword as well.
					err = 1;
					printf("Line %d: Syntactic error, expected ",line);
					if(TermToTok(((curr->c)->value).T)->lex==KEYWORD){
						printf("%s",LexemeToString(TermToTok(((curr->c)->value).T)->lex));
						printf("%s",KeywordToString((TermToTok(((curr->c)->value).T)->value).key));
					}
					else
						printf("%s",LexemeToString(TermToTok(((curr->c)->value).T)->lex));
					printf("\n");
					curr = popIt(Stack);
					NextTok = 0;
				}
			}
			else if((curr->c)->tag==1){
				int term = TokToTerm(t);
				//this function gives the corresponding terminal for the given token.
				rule = P[((curr->c)->value).NT][term];

				if(rule==NULL){
					//Error handling

					err = 1;
					printf("Line %d: Syntactic error, unexpected ",line);
					if(t->lex==KEYWORD)
						printf("%s%s",LexemeToString(t->lex),KeywordToString(t->value.key));
					else if(t->lex==ID)
						printf("%s%s",LexemeToString(t->lex),t->value.identifier);
					else if(t->lex==NUM)
						printf("%s%lld",LexemeToString(t->lex),t->value.integer);
					else if(t->lex==RNUM)
						printf("%s%f",LexemeToString(t->lex),t->value.real);
					else
						printf("%s",LexemeToString(t->lex));
					printf("\n");

					if(F->Follow[((curr->c)->value).NT]&((unsigned long long int)1<<term)){
						NextTok = 0;
						curr = popIt(Stack);
						NonTermError = 0;
					}
					else{
						NextTok = 1;
						NonTermError = 1;
					}
				}
				else{
					short ruleNum = (short)((curr->c)->value).NT;
					ruleNum = ruleNum<<3;
					//Moving the number in the short by 3 bits.
					ruleNum |= (short)R[((curr->c)->value).NT][term];
					while(rule){
						dummy = (ParseNode*)malloc(sizeof(ParseNode));
						dummy->t = NULL;
						dummy->c = rule;
						dummy->Node = NULL;
						dummy->inh = NULL;
						dummy->Parent = curr;
						dummy->rule1 = ruleNum;
						dummy->rule2 = 0;
						dummy->Right = NULL;
						dummy->First = NULL;
						if(previous==NULL){
							temp = dummy;
						}
						else{
							previous->Right = dummy;
						}
						previous = dummy;
						rule = rule->next;
					}
					curr->First = temp;
					//Making sure the first child is attached to the parent.
					//check wether the following works.
					curr = popIt(Stack);
					curr->rule2 = ruleNum;
					//If the rule is something like NT->e we do not have to add it to the stack.
					if((temp->c)->tag!=4)rulePush(Stack,temp);
					NextTok = 0;
				}
			}
			else if((curr->c)->tag==3 || (curr->c)->tag==5 || (curr->c)->tag==4){
				printf("This should never happen.\n");
			}
		}
	}
	if(err==0){
		printf("Input source code is syntactically correct....\n");
	}
	else{
		*error = 1;
	}
	return Root;
}

void printParseTree(ParseNode* P, int* parseTreeCount,int flag){
	// Function to print the parse tree in the file given by the path 'outfile'.
	// FILE* f1 = fopen(outfile,"w");
	// if(f1==NULL)return;
	if(flag==1){
		printf("Inorder traversal\n");
		printf("\n\n%-25s%-25s%-15s%-15s%-15s%-25s%-20s%-25s\n", "lexeme", "CurrentNode", "lineno", "tokenName", "valueIfNumber","parentNodeSymbol", "isLeafNode", "NodeSymbol" );
	}
	inorder(P,parseTreeCount,flag);
	// fclose(f1);
}


void inorder(ParseNode* P,int* parseTreeCount,int flag){
	// Recursive function for inorder traversal for printing the constructed parse tree in the file 'f1'.
	if(P==NULL)return;
	*parseTreeCount += 1;

	if(P->First)inorder(P->First,parseTreeCount,flag);
	if(flag==1){

		if((P->t)!=NULL && (P->c)!=NULL &&(P->c)->tag==2){
			if((P->t)->lex==KEYWORD)
				printf("%-25s",KeywordToString(((P->t)->value).key));
			else if((P->t)->lex==ID)
				printf("%-25s",((P->t)->value).identifier);
			else if((P->t)->lex==NUM)
				printf("%-25lld",((P->t)->value).integer);
			else if((P->t)->lex==RNUM)
				printf("%-25f",((P->t)->value).real);
			else
				printf("%-25s",LexemeToString((P->t)->lex));
			
			printf("%-25s",TerminalToString(((P->c)->value).T));
			printf("%-15d",(P->t)->line_no);
			printf("%-15s",TerminalToString(((P->c)->value).T));

			if((P->t)->lex==NUM)
				printf("%-15lld",((P->t)->value).integer);
			else if((P->t)->lex==RNUM)
				printf("%-15f",((P->t)->value).real);
			else
				printf("%-15s","---");
			
			printf("%-25s",NonTerminalToString((((P->Parent)->c)->value).NT));
			printf("%-20s","yes");
			printf("%-25s", "---");
			printf("\n");

		}
		else if((P->c)!=NULL && (P->c)->tag==1){
			printf("%-25s","---");
			printf("%-25s",NonTerminalToString(((P->c)->value).NT));
			printf("%-15s","---");
			printf("%-15s","---");
			printf("%-15s","---");

			if(P->Parent!=NULL){
				printf("%-25s",NonTerminalToString((((P->Parent)->c)->value).NT));
			}
			else
				printf("%-25s","ROOT");

			printf("%-20s", "no");
			printf("%s",NonTerminalToString(((P->c)->value).NT));
			printf("\n");

		}
		else if((P->c)!=NULL && (P->c)->tag==4){
			printf("%-25s","---");
			printf("%-25s","e(epsilon)");		
			printf("%-15s","---");
			printf("%-15s","---");
			printf("%-15s","---");
			printf("%-25s",NonTerminalToString((((P->Parent)->c)->value).NT));
			printf("%-25s", "yes");
			printf("%-25s", "---");
			printf("\n");
		}


	}
	if(P->First){
		ParseNode* curr = (P->First)->Right;
		while(curr){
			inorder(curr,parseTreeCount,flag);
			curr = curr->Right;
		}
	}
	return;
}

ParseNode* creatGramAndParse(char path1[], char path2[], int flag, Grammar* G, int* error, int* parseTreeCount){
	// This function creates the grammar by reading the file 'grammar.txt'	
	//Grammar* G = createGrammar();
	// First and Follow sets are created using the above generated grammar.
	FirstAndFollow* F = (FirstAndFollow*)malloc(sizeof(FirstAndFollow));
	ComputeFirstAndFollowSets (G, F);
	// Parse table is created using the First and Follow sets.
	ParseTable P;
	RuleNum R;
	createParseTable(G,F,P,R);
	//free(G);
	//PrintFirstAndFollow(F)
	// Parse tree is created for the code given in the file located at 'path1' and is printed into the file located at 'path2'.
	ParseNode* start = parseInputSourceCode(path1, P,F,R, error);
	if(flag<2){
		printParseTree(start,parseTreeCount,flag);
	}
	free(F);
	//freeing the first and follow sets.
	return start;
}
