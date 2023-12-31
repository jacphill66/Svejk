#ifndef PARSING_H_ 
#define PARSING_H_

#include "Common.h"
#include "Lexing.h"

typedef struct ASTNode ASTNode;

typedef struct{
	ASTNode* lhs;
	OPCode op;
	ASTNode* rhs;
}ASTBinaryOP;


typedef struct{
	OPCode op;
	ASTNode* opperand;
}ASTUnaryOP;

typedef struct{
	//hold args?
	ASTNode* opperand;
}ASTCallOP;

typedef struct {
	Value v;
}ASTValue;

typedef struct {
	char* id;
}ASTID;

typedef struct {
	ASTNode* expr;
}ASTExpression;

typedef struct{
	ASTNode* expr;
}ASTPrint;

typedef enum{
	ASTBinaryOP_NODE_TYPE,
	ASTUnaryOP_NODE_TYPE,
	ASTCallOP_NODE_TYPE,
	ASTID_NODE_TYPE,
	ASTValue_NODE_TYPE,
	ASTExpression_NODE_TYPE,
	ASTPrint_NODE_TYPE,
}ASTNodeType;

struct ASTNode {
	ASTNodeType type;
	union{
		ASTBinaryOP binaryOP;
		ASTUnaryOP unaryOP;
		ASTCallOP callOP;
		ASTID id;
		ASTValue value;
		ASTExpression expr;
		ASTPrint print;
	};
} ;

typedef struct {
	ASTNode* nodes;
	long cappacity;
	long numberOfNodes;
} AST;


ASTNode* split(TokenArray* tokens, int prec);

AST parse(TokenArray* tokens);

void printASTNode(ASTNode* node);
void printAST(AST* ast);
void freeAST(AST* ast);

#endif