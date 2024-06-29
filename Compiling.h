#ifndef Compiling_H_ 
#define Compiling_H_

#include "Lexing.h"
#include "Common.h"
#include "Parsing.h"

typedef struct {
	OPCode* ops;
	long cappacity;
	long opCount;
} OPArray;

typedef struct {
	Value* values;
	char** strings;
	//objects 
	
	long stringCount;
	long stringCappacity;
	
	long cappacity;
	long valueCount;
} ValueArray;

typedef struct {
	ValueArray* values;
	OPArray* ops;
	long globalCount;
	char** strings;
} Program;

Program* newProgram(int stringCount);
void compileASTNode();
void printValues(ValueArray* vals);
void printOPS(Program* p);
void printProgram(Program* p);
void freeProgram(Program* p);


Program* compile(Program* p, AST* ast);

#endif