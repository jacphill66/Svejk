#include "Rewriting.h"

//1.) for {...} -> for true { ... } 
//2.) for int_expr -> {let count = 0; while count < int_expr{ LOOP_BODY } count += 1;} 
//3.) for n1; n2; n3 { ... } 

//growing, shrinking table...

//need a string length function
//won't this generate bigger strings?
//a->b, b->a
//special protocol for strings that are shorter, just check if its consistent (one character differs), if it is, cool, if not add an a
/*
apples
jack
chicken
cheese

Generated String: bbaa

If:
bbaa


*/

//resize a string function...

void addChar(char* str, char* currentString, int index){
	if(strlen(str) <= strlen(currentString)){
		//if char at index is not a -> add b-
		//else add b
	}
	else if(strlen(str) == strlen(currentString)){
		//check with the string if it has one different character, keep the string
		//else add a
	}
}

char* uniqueID(VariableTable* table){
	//probably need helpers...
	
	
	//go through globals
	//go through locals
	//
	return NULL;
}

char** resizeVariables(char** variables, int cappacity){
	char** newVariables = (char**)realloc(variables, 2*cappacity*sizeof(char*));
	if(newVariables == NULL){
		printf("could not resize variables!\n");
		free(variables);
		exit(1);
	}
	return newVariables;
}

void emitVariable(VariableTable* table, char* id){
	table->localVariables[table->localVarIndex] = id;
	table->localVarIndex++;
	if(table->localVarIndex == table->cappacity){
		table->localVariables = resizeVariables(table->localVariables, table->cappacity);
		table->cappacity *= 2;
	}
}

void rewriteLoopType1(ASTNode* loop){
	ASTNode nLoop;
	nLoop.simpleLoop.expr = NULL;
	nLoop.simpleLoop.block = NULL;
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	/*node->type = ASTValue_NODE_TYPE;
	Value v;
	v.type = BOOL_VAL;
	v.boolean = true;
	node->value.v = v;
	node->value.line = loop.loop.line;
	nLoop.simpleLoop.expr = node;
	nLoop.simpleLoop.block = loop.loop.b;*/
}

void rewriteLoop(ASTNode* loop){
	ASTNode* first = loop->loop.n1;
	if(first == NULL){
		return rewriteLoopType1(loop);
	}
	else {
		if((first->type == ASTExpression_NODE_TYPE) && (first->expr.t == I32_TYPE)){
			ASTBlock b;

		}
		//else{
	}
	ASTNode n;
}

ASTNode* rewritePrint(Rewriter* rewriter, ASTNode* n){
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	ASTPrint p;
	p.expr = rewriteNode(rewriter, n->print.expr);
	node->print = p;
	node->type = ASTPrint_NODE_TYPE;
	return node;
}

ASTNode* rewriteExpression(Rewriter* rewriter, ASTNode* n){
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	ASTExpression e;
	e.expr = rewriteNode(rewriter, n->expr.expr);
	node->expr = e;
	node->type = ASTExpression_NODE_TYPE;
	return node;
}

ASTNode* rewriteBinaryOP(Rewriter* rewriter, ASTNode* n){
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	ASTBinaryOP op;
	op.lhs = rewriteNode(rewriter, n->binaryOP.lhs);
	op.rhs = rewriteNode(rewriter, n->binaryOP.rhs);
	op.op = n->binaryOP.op;
	op.line = n->binaryOP.line;
	node->binaryOP.t = n->binaryOP.t;
	node->binaryOP = op;
	node->type = ASTBinaryOP_NODE_TYPE;
	return node;
}

ASTNode* rewriteUnaryOP(Rewriter* rewriter, ASTNode* n){
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	ASTUnaryOP op;
	op.opperand = rewriteNode(rewriter, n->unaryOP.opperand);
	node->unaryOP = op;
	node->type = ASTUnaryOP_NODE_TYPE;
	return node;
}
ASTNode* rewriteGlobalID(Rewriter* rewriter, ASTNode* n){
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	ASTGlobalID id;
	id.id = n->globalID.id;
	id.t = n->globalID.t;
	id.line = n->globalID.line;
	node->globalID = id;
	node->type = ASTGlobalID_NODE_TYPE;
	return node;
}
ASTNode* rewriteLocalID(Rewriter* rewriter, ASTNode* n){
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	ASTLocalID id;
	id.id = n->localID.id;
	id.t = n->localID.t;
	id.line = n->localID.line;
	node->localID = id;
	node->type = ASTLocalID_NODE_TYPE;
	return node;
}
ASTNode* rewriteGlobalAssignment(Rewriter* rewriter, ASTNode* n){
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	ASTGlobalAssignment ass;
	ass.id = n->globalAss.id;
	ass.expr = rewriteNode(rewriter, n->globalAss.expr);
	ass.t = n->globalAss.t;
	ass.line = n->globalAss.line;
	node->globalAss = ass;
	node->type = ASTGlobalAssignment_NODE_TYPE;
	return node;
}
ASTNode* rewriteLocalAssignment(Rewriter* rewriter, ASTNode* n){
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	ASTLocalAssignment ass;
	ass.id = n->localAss.id;
	ass.expr = rewriteNode(rewriter, n->localAss.expr);
	ass.t = n->localAss.t;
	ass.line = n->localAss.line;
	node->localAss = ass;
	node->type = ASTLocalAssignment_NODE_TYPE;
	return node;
}
ASTNode* rewriteString(Rewriter* rewriter, ASTNode* n){
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	ASTString str;
	str.str = n->str.str;
	str.index = n->str.index;
	str.t = n->str.t;
	str.line = n->str.line;
	node->str = str;
	node->type = ASTString_NODE_TYPE;
	return node;
}
ASTNode* rewriteValue(Rewriter* rewriter, ASTNode* n){
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	ASTValue val;
	val.v = n->value.v;
	val.line = n->value.line;
	val.t = n->value.t;
	node->value = val;
	node->type = ASTValue_NODE_TYPE;
	return node;
}
ASTNode* rewriteGlobalVariable(Rewriter* rewriter, ASTNode* n){
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	ASTGlobalVariable var;
	var.id = n->globalVar.id;
	var.expr = rewriteNode(rewriter, n->globalVar.expr);
	var.line = n->globalVar.line;
	var.t = n->globalVar.t;
	node->globalVar = var;
	node->type = ASTGlobalVariable_NODE_TYPE;
	rewriter->table->globalVariables[rewriter->table->globalVarIndex] = var.id;
	rewriter->table->globalVarIndex += 1;
	return node;
}
ASTNode* rewriteLocalVariable(Rewriter* rewriter, ASTNode* n){
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	ASTLocalVariable var;
	var.id = n->localVar.id;
	var.expr = rewriteNode(rewriter, n->localVar.expr);
	var.line = n->localVar.line;
	var.t = n->localVar.t;
	node->localVar = var;
	node->type = ASTLocalVariable_NODE_TYPE;
	emitVariable(rewriter->table, var.id);
	return node;
}
ASTNode* rewriteBlock(Rewriter* rewriter, ASTNode* n){
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	ASTBlock b;
	b.variableCount = n->block.variableCount;
	b.cappacity = n->block.cappacity;
	b.numberOfNodes = n->block.numberOfNodes;
	b.nodes = (ASTNode*)malloc(sizeof(ASTNode));
	b.t = n->block.t;
	b.line = n->block.line;
	node->block = b;
	node->type = ASTBlock_NODE_TYPE;
	for(int i = 0; i < n->block.numberOfNodes; i++){
		emitNodeToBlock(rewriteNode(rewriter, &n->block.nodes[i]), node);
	}
	rewriter->table->localVarIndex -= n->block.variableCount;
	return node;
}

ASTNode* rewriteNode(Rewriter* rewriter, ASTNode* n){
	switch(n->type){
		case ASTPrint_NODE_TYPE:{
			return rewritePrint(rewriter, n);
		}
		case ASTExpression_NODE_TYPE:{
			return rewriteExpression(rewriter, n);		
		}
		case ASTBinaryOP_NODE_TYPE:{
			return rewriteBinaryOP(rewriter, n);		
		}
		case ASTUnaryOP_NODE_TYPE:{
			return rewriteUnaryOP(rewriter, n);				
		}
		case ASTGlobalID_NODE_TYPE:{
			return rewriteGlobalID(rewriter,n);					
		}
		case ASTLocalID_NODE_TYPE:{
			return rewriteLocalID(rewriter, n);
		}
		case ASTGlobalAssignment_NODE_TYPE:{
			return rewriteGlobalAssignment(rewriter, n);
		}
		case ASTLocalAssignment_NODE_TYPE:{
			return rewriteLocalAssignment(rewriter, n);
		}
		case ASTString_NODE_TYPE:{
			return rewriteString(rewriter, n);
		}
		case ASTValue_NODE_TYPE:{
			return rewriteValue(rewriter, n);
		}
		case ASTGlobalVariable_NODE_TYPE:{
			return rewriteGlobalVariable(rewriter, n);
		}
		case ASTLocalVariable_NODE_TYPE:{
			return rewriteLocalVariable(rewriter, n);
		}
		case ASTBlock_NODE_TYPE:{
			return rewriteBlock(rewriter, n);
		}
		default:{
			printf("Cannot rewrite node of given type\n");
			exit(1);
		}
	}
}

//make a new ast, free the old one
AST* rewrite(Rewriter* rewriter, AST* ast){
	for(int i = 0; i < ast->numberOfNodes; i++){
		emitNode(rewriteNode(rewriter, &ast->nodes[i]), rewriter->rewrittenAST);
	}
	return ast;
}

VariableTable* newVariableTable(int globalCount){
	VariableTable* table = (VariableTable*)malloc(sizeof(VariableTable));
	table->localVariables = (char**)malloc(sizeof(char*));
	table->globalVariables = (char**)malloc(sizeof(char*)*globalCount);
}

Rewriter* newRewriter(Parser* parser){
	Rewriter* rewriter = (Rewriter*)malloc(sizeof(Rewriter));
	rewriter->rewrittenAST = newAST();
	rewriter->ast = parser->ast;
	rewriter->table = newVariableTable(parser->globalCount);
	return rewriter;
}

void freeVariableTable(VariableTable* t){
	free(t->localVariables);
	free(t->globalVariables);
	free(t);
}

void freeRewriter(Rewriter* r){
	freeVariableTable(r->table);
	free(r);
}




// macros, operators, generics, rewriting feature... lots of need for a dedicated rewriter...