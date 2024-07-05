#ifndef REWRITER_H
#define REWRITER_H

#include "Parsing.h"
#include "Analyzer.h"

typedef struct{
	char* str;
	int length;
	int size;
}String;

typedef struct{
	String** localVariables;
	String** globalVariables;
	int localCount;
	int localCappacity;
	int globalCount;
}VariableTable;

typedef struct{
	VariableTable* table;
	AST* ast;
	AST* rewrittenAST;
	String** localVariables;
	String** globalVariables;
}Rewriter;

Rewriter* newRewriter(AST* ast, int globalCount);
ASTNode* rewriteNode(Rewriter* rewriter, ASTNode* n);
AST* rewrite(Rewriter* rewriter, AST* ast);

#endif