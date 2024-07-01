#include "Rewriting.h"

//1.) for {...} -> for true { ... } 
//2.) for int_expr -> {let count = 0; while count < int_expr{ LOOP_BODY } count += 1;} 
//3.) for n1; n2; n3 { ... } 

bool intExpression(ASTNode* expr){
	return ;
}

bool boolExpression(ASTNode* expr){
	return false;
}

ASTNode rewriteLoop(ASTNode loop){
	ASTNode* nLoop = (ASTNode*)malloc(sizeof(ASTNode));
	nLoop->simpleLoop.expr = NULL;
	nLoop->simpleLoop.block = NULL;
	if(loop.loop.n1 == NULL){//for {...} --> for true { ... } 
		ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
		node->type = ASTValue_NODE_TYPE;
		Value v;
		v.type = BOOL_VAL;
		v.boolean = true;
		ASTValue val = {v, loop.loop.line};
		node->value = val;
		nLoop->simpleLoop.expr = node;
		nLoop->simpleLoop.block = loop.loop.b;
	}
	else {
		if(loop.loop.n1->type == ASTExpression_NODE_TYPE){
		
		}
		//else{
	}
	ASTNode n;
	return n;
}

AST* rewrite(AST* ast){
	for(int i = 0; i < ast->numberOfNodes; i++){
		if(ast->nodes[i].type != ASTForLoop_NODE_TYPE) ast->nodes[i] = ast->nodes[i];
		else ast->nodes[i] = rewriteLoop(ast->nodes[i]);
	}
	return ast;
}