#ifndef REWRITER_H
#define REWRITER_H

#include "Parsing.h"
#include "Analyzer.h"
#include "ASTObject.h"

typedef struct{
	char* str;
	int length;
	int size;
}String;

typedef struct{
	String** variables;
	int count;
	int cappacity;
}VariableTable;

typedef struct{
	VariableTable* table;
	AST* ast;
	AST* rewrittenAST;
}Rewriter;

Rewriter* newRewriter(AST* ast);
void freeRewriter(Rewriter* r);
ASTNode* rewriteNode(Rewriter* rewriter, ASTNode* n);
AST* rewrite(Rewriter* rewriter, AST* ast);

#endif