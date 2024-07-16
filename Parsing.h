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
	Type t;
}ASTBinaryOP;

typedef struct{
	OPCode op;
	ASTNode* opperand;
	long line;
	Type t;
}ASTUnaryOP;

typedef struct{
	//hold args?
	ASTNode* opperand;	
	long line;
	Type t;
}ASTCallOP;

typedef struct {
	Value v;
	long line;
	Type t;
}ASTValue;

typedef struct {
	char* id;
	long index;
	long line;
	Type t;
}ASTLocalID;

typedef struct {
	char* id;
	long index;
	long line;
	Type t;
}ASTGlobalID;

typedef struct {
	char* id;
	ASTNode* expr;
	long offset;
	long line;
	Type t;
}ASTLocalAssignment;

typedef struct {
	char* id;
	ASTNode* expr;
	long index;
	long line;
	Type t;
}ASTGlobalAssignment;

typedef struct {
	char* id;
	Type type;
	ASTNode* expr;
	long index;
	long line;
	Type t;
}ASTGlobalVariable;

typedef struct {
	char* id;
	Type type;
	ASTNode* expr;
	long offset;
	long line;
	Type t;
}ASTLocalVariable;

typedef struct {
	ASTNode* expr;
	long line;
	Type t;
}ASTExpression;

typedef struct{
	ASTNode* expr;
	long line;
	Type t;
}ASTPrint;

typedef struct {
	ASTNode* nodes;
	long variableCount;
	long cappacity;
	long numberOfNodes;
	long line;
	Type t;
} ASTBlock;

typedef struct{
	char* str;
	long index;
	long line;
	Type t;
}ASTString;

typedef struct{
	ASTNode* n1;
	ASTNode* n2;
	ASTNode* n3;
	ASTNode* min; 
	ASTNode* max;
	ASTNode* b;
	long line;
	Type t;
}ASTForLoop;

typedef struct{
	ASTNode* expr;
	ASTNode* block;
	long line;
	Type t;
}ASTLoop;

typedef struct{
	ASTNode* s;
	long line;
	Type t;
}ASTElse;

typedef struct{
	ASTNode* expr;
	ASTNode* s;
	ASTNode* elseS;
	long line;
	Type t;
}ASTIf;

typedef struct{
	Value* initalElements;
	char** keys; 
	int size;
	int cappacity;
}PrimStruct;

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
	ASTLoop_NODE_TYPE,
	ASTElse_NODE_TYPE,
	ASTIf_NODE_TYPE,
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
		ASTLoop simpleLoop;
		ASTIf ifS;
		ASTElse elseS;
	};
} ;

typedef struct {
	ASTNode* nodes;
	long cappacity;
	long numberOfNodes;
	int stringCount;
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

ASTNode* newBlock();
void emitNodeToBlock(ASTNode* node, ASTNode* b);

ASTNode* newLoop();
void emitNode(ASTNode* node, AST* ast);

ASTNode* split(Parser* parser, TokenArray* tokens, int prec);

AST* newAST();
Parser* newParser();

ASTNode* parseStatement(Parser* parser, TokenArray* tokens, ASTNode* b);
void parseLocal(Parser* parser, ASTNode* b, TokenArray* tokens);
void parse(Parser* parser, TokenArray* tokens);

void printASTNode(AST* ast, ASTNode* node);
void printAST(AST* ast);

void freeASTNode(ASTNode* node);
void freeAST(AST* ast);
void freeParser(Parser* parser);

#endif