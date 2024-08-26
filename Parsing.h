#ifndef PARSING_H_ 
#define PARSING_H_

#include "Common.h"
#include "Lexing.h"
#include "Table.h"
#include "ScopeChain.h"
#include "Memory.h"
#include "ASTObject.h"
#include "Template.h"
#include "TemplateTree.h"
#include "TemplateTable.h"

typedef struct {
	int tokenIndex;
	AST* ast;
	TemplateRedBlackTree* t;
	TemplateTable* table;
} Parser;


ASTNode* split(Parser* parser, TokenArray* tokens, int prec);

AST* newAST();

Parser* newParser(int formCount);
ASTNode* parseBlockOrTable(TokenArray* tokens, Parser* p);
ASTNode* parseStatement(Parser* parser, TokenArray* tokens);
ASTNode* parse(Parser* parser, TokenArray* tokens);

void freeParser(Parser* parser);


#endif