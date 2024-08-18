#ifndef ASTOBJECT_H_ 
#define ASTOBJECT_H_

#include "Common.h"
#include "Template.h"

typedef struct ASTNode ASTNode;

typedef struct{
	ASTNode* lhs;
	char* op;
	ASTNode* rhs;
	int line;
	Type* t;
}ASTBinaryOP;

typedef struct{
	char* op;
	ASTNode* opperand;
	int line;
	OperatorType type;
	Type* t;
}ASTUnaryOP;

typedef struct {
	char** words;
	//fill in
}ASTProc;

typedef struct{
	//hold args?
	ASTNode* opperand;	
	int line;
	Type* t;
}ASTCallOP;

typedef struct {
	ASTNode* nodes;
	long variableCount;
	long cappacity;
	long numberOfNodes;
	int line;
	Type* t;
} ASTBlock;

typedef struct {
	ASTNode* expr1;
	ASTNode* expr2;
	int line;
	Type* t;
}ASTWhile;

typedef struct {
	Value v;
	int line;
	Type* t;
}ASTValue;

typedef struct {
	char* id;
	int line;
	Type* t;
}ASTID;

typedef struct {
	char* id;
	ASTNode* expr;
	int line;
	Type* t;
}ASTAssignment;

typedef struct {
	char* id;
	Type* type;
	ASTNode* expr;
	int line;
	Type* t;
}ASTVariable;

typedef struct {
	ASTNode* expr;
	int line;
	Type* t;
	bool statement;
}ASTExpression;

typedef struct{
	ASTNode* expr;
	int line;
	Type* t;
}ASTElse;

typedef struct{
	ASTNode* expr1;
	ASTNode* expr2;
	ASTNode* elseExpr;
	int line;
	Type* t;
}ASTIf;

typedef struct{
	char* str;
	int line;
	Type* t;
}ASTString;

//remove the rest...

typedef struct{
	ASTNode* expr;
	int line;
	Type* t;
}ASTPrint;

typedef struct{
	ASTNode* n1;
	ASTNode* n2;
	ASTNode* n3;
	ASTNode* min; 
	ASTNode* max;
	ASTNode* b;
	int line;
	Type* t;
}ASTForLoop;

typedef struct{
	ASTNode* expr;
	ASTNode* block;
	int line;
	Type* t;
}ASTLoop;

typedef enum {
	//1-4 * a-b
	TABLE_1A,
	TABLE_1B,
	
}TableType;

typedef struct{
	ASTNode* nodes;
	char** keys; 
	TableType type;
	int numberOfNodes;
	int cappacity;
	int line;
	Type* t;
}ASTTable;

typedef enum{
	ASTBinaryOP_NODE_TYPE,
	ASTUnaryOP_NODE_TYPE,
	ASTCallOP_NODE_TYPE,
	ASTID_NODE_TYPE,
	ASTValue_NODE_TYPE,
	ASTString_NODE_TYPE,
	ASTExpression_NODE_TYPE,
	ASTPrint_NODE_TYPE,
	ASTVariable_NODE_TYPE,
	ASTBlock_NODE_TYPE,
	ASTAssignment_NODE_TYPE,
	ASTForLoop_NODE_TYPE,
	ASTLoop_NODE_TYPE,
	ASTElse_NODE_TYPE,
	ASTIf_NODE_TYPE,
	ASTTable_NODE_TYPE,
	ASTWhile_NODE_TYPE,
}ASTNodeType;

struct ASTNode {
	ASTNodeType type;
	union{
		ASTBinaryOP binaryOP;
		ASTUnaryOP unaryOP;
		ASTCallOP callOP;
		ASTValue value;
		ASTString str;
		ASTExpression expr;
		ASTPrint print;
		ASTID id;
		ASTVariable var;
		ASTAssignment ass;
		ASTBlock block;
		ASTForLoop loop;
		ASTLoop simpleLoop;
		ASTIf ifExpr;
		ASTElse elseExpr;
		ASTTable table;
		ASTWhile whileExpr;
	};
} ;

typedef struct {
	ASTNode* nodes;
	long cappacity;
	long numberOfNodes;
} AST;

ASTNode* newASTNode();

ASTNode* newASTBinaryOP(ASTNode* lhs, char* op, ASTNode* rhs, int line, Type* t);

ASTNode* newASTUnaryOP(ASTNode* opperand, char* op, int line, Type* t);
ASTNode* newASTValue(Value v, int line, Type* t);
ASTNode* newASTExpression(ASTNode* expr, bool statement, int line, Type* t);
ASTNode* newASTID(char* id, int line, Type* t);
ASTNode* newASTAssignment(char* id, ASTNode* expr, int line, Type* t);
ASTNode* newASTVariable(char* id, ASTNode* expr, Type* type, int line, Type* t);
ASTNode* newASTPrint(ASTNode* expr, int line, Type* t);
ASTNode* newASTString(char* str, int line, Type* t);
ASTNode* newASTElse(ASTNode* s, int line, Type* t);
ASTNode* newASTIf(ASTNode* expr1, ASTNode* expr2, ASTNode* elseExpr, int line, Type* t);
ASTNode* newASTSimpleLoop(ASTNode* expr, ASTNode* block, int line, Type* t);
ASTNode* newASTBlock(int line, Type* t);
ASTNode* newASTForLoop(int line, Type* t);
ASTNode* newASTWhile(ASTNode* expr, ASTNode* b, int line, Type* t);

//Just parsing
ASTNode* newASTTable(TableType type, int line, Type* t);

void freeASTNode(ASTNode* node, bool freeType);
void freeAST(AST* ast, bool freeType);

void printASTNode(AST* ast, ASTNode* node);
void printAST(AST* ast);

void emitNodeToBlock(ASTNode* node, ASTNode* b);
void emitNodeToTable(char* key, ASTNode* node, ASTNode* t);
void emitNode(ASTNode* node, AST* ast);

#endif