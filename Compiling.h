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
	long cappacity;
	long valueCount;
} ValueArray;

typedef struct {
	ValueArray values;
	OPArray ops;
} Program;

void compileASTNode();
void printValues(ValueArray* vals);
void printOPS(Program* p);
void printProgram(Program* p);
void freeProgram(Program* p);


Program compile(AST* ast);

#endif