// Group Number - 25
// Name : Aayush Atul Verma         ID : 2017A7PS0061P
// Name : Aditya Ramaswamy          ID : 2017A7PS0130P
// Name : Jithin Kallukalam Sojan   ID : 2017A7PS0163P

#ifndef PARSER_ACCESSORY
#define PARSER_ACCESSORY
Terminal stringToTerminal(char* str);
Non_Terminal stringToNonTerminal(char* str);
char* NonTerminalToString(Non_Terminal NT);
char* TerminalToString(Terminal T);
Token* TermToTok(Terminal T);
int TokToTerm(Token* Tkn);
int term_cmp(Token* Tkn, Terminal T);
#endif