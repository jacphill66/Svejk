#ifndef Compiling_H_ 
#define Compiling_H_

#include "Lexing.h"
#include "Common.h"
#include "Parsing.h"
#include "ScopeChain.h"
#include "Analyzer.h"
#include "ASTObject.h"

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

typedef struct {
	AST* ast;
	Program* prog;
	ScopeChain* scopes;
	Analysis* analysis;
	int scopeDepth;
	int opPos;
} Compiler;

Program* newProgram(int stringCount);
Compiler* newCompiler(AST* ast, Analysis* a);
void compileASTNode(Compiler* c, ASTNode* node);
void printValues(ValueArray* vals);
void printOPS(Program* p);
void printProgram(Program* p);
void freeProgram(Program* p);
void freeCompiler(Compiler* compiler);


Program* compile(Compiler* c, AST* ast);

#endif