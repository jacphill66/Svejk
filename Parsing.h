#ifndef PARSING_H_ 
#define PARSING_H_

#include "Common.h"
#include "Lexing.h"
#include "Table.h"
#include "ScopeChain.h"
#include "Memory.h"

typedef struct ASTNode ASTNode;

typedef struct{
	ASTNode* lhs;
	OPCode op;
	ASTNode* rhs;
	long line;
}ASTBinaryOP;

typedef struct{
	OPCode op;
	ASTNode* opperand;
	long line;
}ASTUnaryOP;

typedef struct{
	//hold args?
	ASTNode* opperand;	
	long line;
}ASTCallOP;

typedef struct {
	Value v;
	long line;
}ASTValue;

typedef struct {
	char* id;
	long index;
	long line;
}ASTLocalID;

typedef struct {
	char* id;
	long index;
	long line;
}ASTGlobalID;

typedef struct {
	char* id;
	ASTNode* expr;
	long offset;
	long line;
}ASTLocalAssignment;

typedef struct {
	char* id;
	ASTNode* expr;
	long index;
	long line;
}ASTGlobalAssignment;

typedef struct {
	char* id;
	Type type;
	ASTNode* expr;
	long index;
	long line;
}ASTGlobalVariable;

typedef struct {
	char* id;
	Type type;
	ASTNode* expr;
	long offset;
	long line;
}ASTLocalVariable;

typedef struct {
	ASTNode* expr;
	long line;
}ASTExpression;

typedef struct{
	ASTNode* expr;
	long line;
}ASTPrint;

typedef struct {
	ASTNode* nodes;
	long variableCount;
	long cappacity;
	long numberOfNodes;
	long line;
} ASTBlock;

typedef struct{
	char* str;
	long index;
	long line;
}ASTString;

typedef struct{
	ASTNode* n1;
	ASTNode* n2;
	ASTNode* n3;
	ASTNode* min; 
	ASTNode* max;
	ASTNode* b;
}ASTForLoop;

typedef enum{
	ASTBinaryOP_NODE_TYPE,
	ASTUnaryOP_NODE_TYPE,
	ASTCallOP_NODE_TYPE,
	ASTLocalID_NODE_TYPE,
	ASTGlobalID_NODE_TYPE,
	ASTValue_NODE_TYPE,
	ASTString_NODE_TYPE,
	ASTExpression_NODE_TYPE,
	ASTPrint_NODE_TYPE,
	ASTGlobalVariable_NODE_TYPE,
	ASTLocalVariable_NODE_TYPE,
	ASTBlock_NODE_TYPE,
	ASTLocalAssignment_NODE_TYPE,
	ASTGlobalAssignment_NODE_TYPE,
	ASTForLoop_NODE_TYPE,
}ASTNodeType;

struct ASTNode {
	ASTNodeType type;
	union{
		ASTBinaryOP binaryOP;
		ASTUnaryOP unaryOP;
		ASTCallOP callOP;
		ASTLocalID localID;
		ASTGlobalID globalID;
		ASTValue value;
		ASTString str;
		ASTExpression expr;
		ASTPrint print;
		ASTGlobalVariable globalVar;
		ASTLocalVariable localVar;
		ASTGlobalAssignment globalAss;
		ASTLocalAssignment localAss;
		ASTBlock block;
		ASTForLoop loop;
	};
} ;

typedef struct {
	ASTNode* nodes;
	long cappacity;
	long numberOfNodes;
} AST;

typedef struct {
	AST* ast;
	Table* globalVariables;
	ScopeChain* scopes;
	RedBlackTree* strings;
	long globalCount;
	long scopeDepth;
	long stringCount;	
} Parser;


ASTNode* split(Parser* parser, TokenArray* tokens, int prec);

AST* newAST();
Parser* newParser();

ASTNode* parseStatement(Parser* parser, TokenArray* tokens, ASTNode* b);
void parseLocal(Parser* parser, ASTNode* b, TokenArray* tokens);
void parse(Parser* parser, TokenArray* tokens);

void printASTNode(AST* ast, ASTNode* node);
void printAST(AST* ast);

void freeAST(AST* ast);
void freeParser(Parser* parser);

#endif