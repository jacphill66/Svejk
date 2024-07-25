#ifndef PARSING_H_ 
#define PARSING_H_

#include "Common.h"
#include "Lexing.h"
#include "Table.h"
#include "ScopeChain.h"
#include "Memory.h"
#include "ASTObject.h"

typedef struct {
	AST* ast;
} Parser;

ASTNode* newBlock();
ASTNode* newLoop();

ASTNode* split(Parser* parser, TokenArray* tokens, int prec);

AST* newAST();

Parser* newParser();

ASTNode* parseStatement(Parser* parser, TokenArray* tokens, ASTNode* b);
void parseLocal(Parser* parser, ASTNode* b, TokenArray* tokens);
void parse(Parser* parser, TokenArray* tokens);

void freeParser(Parser* parser);


#endif