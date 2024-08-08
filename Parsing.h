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

typedef struct {
	AST* ast;
	TemplateRedBlackTree* t;
} Parser;

ASTNode* newBlock();
ASTNode* newLoop();

ASTNode* split(Parser* parser, TokenArray* tokens, int prec);

AST* newAST();

Parser* newParser();
ASTNode* parseBlockOrTable(TokenArray* tokens, Parser* p);
ASTNode* parseStatement(Parser* parser, TokenArray* tokens);
void parseLocal(Parser* parser, ASTNode* b, TokenArray* tokens);
void parse(Parser* parser, TokenArray* tokens);

void freeParser(Parser* parser);


#endif