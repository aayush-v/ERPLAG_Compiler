#Set the C compiler to gcc.
CC=gcc

#If any external libraries need to be included as flags add them to the following variable.
ILIBS=-lm

#All include files are located in the following directory. Change the value to . if they are in the same directory as the source code.
INCDIR=.

#All Object files are located in the following directory. Change the value to . if they are in the same directory as the source code.
OBJDIR=.

#All accessory files are in the following directory. These files have no place in the final compiler. 
ACCDIR=.

#A list of all object files for the final target.
_OBJ = driver.o Lexer-Accessory.o lexer.o Parser.o Parser-Accessory.o stack.o ast.o hashtable.o type.o semantic.o Semantic-Accessory.o Ast-Accessory.o codeGen.o asmGen.o

#Append the object directory ahead of the object files in the final target
OBJ=$(_OBJ:%.o=$(OBJDIR)/%.o)


#Lexer
#All header files that are required for the lexer
_LEXINCLUDES= lexer.h lexerDef.h
LEXINCLUDES=$(_LEXINCLUDES:%.h=$(INCDIR)/%.h)

compiler: $(OBJ)
	gcc -o $@ $^ $(ILIBS)

$(OBJDIR)/driver.o: driver.c $(INCDIR)/type.h
	gcc -c -o $@ driver.c

$(OBJDIR)/Lexer-Accessory.o: $(ACCDIR)/Lexer-Accessory.c $(INCDIR)/Lexer-Accessory.h $(LEXINCLUDES)
	gcc -c -o $@ $(ACCDIR)/Lexer-Accessory.c

$(OBJDIR)/lexer.o: lexer.c $(LEXINCLUDES)
	gcc -c -o $@ lexer.c

$(OBJDIR)/Parser.o: Parser.c $(INCDIR)/parser.h $(INCDIR)/Lexer-Accessory.h $(INCDIR)/stack.h
	gcc -c -o $@ Parser.c

$(OBJDIR)/Parser-Accessory.o: $(ACCDIR)/Parser-Accessory.c $(INCDIR)/parser.h
	gcc -c -o $@ $(ACCDIR)/Parser-Accessory.c

$(OBJDIR)/stack.o: stack.c $(INCDIR)/stack.h $(INCDIR)/parser.h
	gcc -c -o $@ stack.c

$(OBJDIR)/ast.o: ast.c $(INCDIR)/ast.h $(INCDIR)/stack.h
	gcc -c -o $@ ast.c

$(OBJDIR)/hashtable.o: hashtable.c $(INCDIR)/hashtable.h 
	gcc -c -o $@ hashtable.c

$(OBJDIR)/type.o: type.c $(INCDIR)/type.h
	gcc -c -o $@ type.c

$(OBJDIR)/semantic.o: semantic.c $(INCDIR)/semantic.h
	gcc -c -o $@ semantic.c

$(OBJDIR)/Semantic-Accessory.o: $(ACCDIR)/Semantic-Accessory.c $(INCDIR)/Semantic-Accessory.h
	gcc -c -o $@ $(ACCDIR)/Semantic-Accessory.c

$(OBJDIR)/Ast-Accessory.o: $(ACCDIR)/Ast-Accessory.c $(INCDIR)/Ast-Accessory.h
	gcc -c -o $@ $(ACCDIR)/Ast-Accessory.c

$(OBJDIR)/codeGen.o: codeGen.c $(INCDIR)/codeGen.h
	gcc -c -o $@ codeGen.c

$(OBJDIR)/asmGen.o: asmGen.c $(INCDIR)/asmGen.h
	gcc -c -o $@ asmGen.c