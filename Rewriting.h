#ifndef REWRITER_H
#define REWRITER_H

#include "Parsing.h"
#include "Analyzer.h"

typedef struct{
	char** localVariables;
	char** globalVariables;
	int localVarIndex;
	int globalVarIndex;
	int cappacity;
}VariableTable;

typedef struct{
	VariableTable* table;
	AST* ast;
	AST* rewrittenAST;

}Rewriter;

Rewriter* newRewriter(Parser* p);
ASTNode* rewriteNode(Rewriter* rewriter, ASTNode* n);
AST* rewrite(Rewriter* rewriter, AST* ast);

#endif