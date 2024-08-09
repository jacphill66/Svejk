#include "Rewriting.h"

/*
void append(String* str, char chr){
	if(str->length+2 > str->size){
		str->str = realloc(str->str, str->size*2);
		str->size*=2;
	}
	str->str[str->length] = chr;
	str->str[str->length+1] = '\0';
	str->length++;
}

String* newString(char* id){
	String* s = malloc(sizeof(String));
	s->length = strlen(id);
	s->size = s->length+1;
	s->str = id;
	return s;
}

String** resizeStrings(String** strings, int size){
	String** newStrings = (String**)realloc(strings, 2*size*sizeof(String*));
	if(newStrings == NULL){
		printf("could not resize strings!");
		free(newStrings);
		exit(1);
	}
	return newStrings;
}

String** resizeVariables(String** variables, int cappacity){
	String** newVariables = (String**)realloc(variables, 2*cappacity*sizeof(String*));
	if(newVariables == NULL){
		printf("could not resize variables!\n");
		free(variables);
		exit(1);
	}
	return newVariables;
}

void emitVariable(VariableTable* t, char* id){
	t->variables[t->count] = newString(id);
	t->count++;
	if(t->count == t->cappacity){
		t->variables = resizeVariables(t->variables, t->cappacity);
		t->cappacity *= 2;
	}
}

String* addNextLetter(String* str, char c){
	//a char is A-Z, _, 0-9, a-z
	if((c != 'Z') && (c != '9') && (c != '_') && (c != 'z')) append(str, c+1);
	else append(str, 'A');
	return str;
}

String* uniqueID(VariableTable* t){
	int pos = 0;
	String* str = (String*)malloc(sizeof(String));
	str->str = (char*)malloc(sizeof(char));
	str->str[0] = '\0';
	str->size = 1;
	str->length = 0;
	for(int i = 0; i < t->count; i++){
		if(t->variables[i]->length > pos){
			addNextLetter(str, t->variables[i]->str[pos]);
			pos+=1;
		}
		else{
			if(strcmp(t->variables[i]->str, str->str)==0){
				char c = str->str[str->length-1];
				if((c != 'Z') && (c != '9') && (c != '_') && (c != 'z')) str->str[str->length-1] = c+1;
				else {
					append(str, 'A');
					pos += 1;
				}
			}
			else continue;
		}
	}
	if(str->length < 1){
		append(str, 'A');
	}
	return str;
}

ASTNode* uniqueVariable(VariableTable* t, ASTNode* expr){
	ASTNode* var = newASTVariable(uniqueID(t)->str, expr, NULL, expr->expr.line, NULL);
	emitVariable(t, var->var.id);
	return var;
}

ASTNode* addMinAndMax(Rewriter* rewriter,ASTNode* loop){
	//later...
}

ASTNode* rewritePrint(Rewriter* rewriter, ASTNode* n){
	return newASTPrint(rewriteNode(rewriter, n->print.expr), n->print.line, NULL);
}

ASTNode* rewriteExpression(Rewriter* rewriter, ASTNode* n){
	return newASTExpression(rewriteNode(rewriter, n->expr.expr), n->expr.statement, n->expr.line, NULL);
}

ASTNode* rewriteBinaryOP(Rewriter* rewriter, ASTNode* n){
	return newASTBinaryOP(rewriteNode(rewriter, n->binaryOP.lhs), n->binaryOP.op, rewriteNode(rewriter, n->binaryOP.rhs), n->binaryOP.line, NULL);
}

ASTNode* rewriteUnaryOP(Rewriter* rewriter, ASTNode* n){
	return newASTUnaryOP(rewriteNode(rewriter, n->unaryOP.opperand), n->unaryOP.op, n->unaryOP.line, NULL);
}

ASTNode* rewriteID(Rewriter* rewriter, ASTNode* n){
	return newASTID(n->id.id, n->id.line, NULL);
}

ASTNode* rewriteAssignment(Rewriter* rewriter, ASTNode* n){
	return newASTAssignment(n->ass.id, rewriteNode(rewriter, n->ass.expr), n->ass.line, NULL);
}

ASTNode* rewriteString(Rewriter* rewriter, ASTNode* n){
	return newASTString(n->str.str, n->str.line, NULL);
}

ASTNode* rewriteValue(Rewriter* rewriter, ASTNode* n){
	return newASTValue(n->value.v, n->value.line, NULL);
}

ASTNode* rewriteVariable(Rewriter* rewriter, ASTNode* n){
	emitVariable(rewriter->table, n->var.id);
	return newASTVariable(n->var.id, rewriteNode(rewriter, n->var.expr), n->var.type, n->var.line, NULL);
}

ASTNode* rewriteBlock(Rewriter* rewriter, ASTNode* n){
	ASTNode* b = newASTBlock(n->block.line, n->block.t);
	for(int i = 0; i < n->block.numberOfNodes; i++) {
		ASTNode* n2 = rewriteNode(rewriter, &n->block.nodes[i]);
		emitNodeToBlock(n2, b);
	}
	return b;
}

ASTNode* rewriteLoopType1(Rewriter* rewriter,ASTNode* loop){
	int line = loop->loop.line;
	return newASTSimpleLoop(newASTValue(newBool(true), line, NULL), rewriteBlock(rewriter, loop->loop.b), line, NULL);
}

ASTNode* rewriteLoopType2(Rewriter* rewriter, ASTNode* loop){
	int line = loop->loop.line;
	return newASTSimpleLoop(rewriteNode(rewriter, loop->loop.n1)->expr.expr, rewriteBlock(rewriter, loop->loop.b), line, NULL);
}

ASTNode* rewriteLoopType3(Rewriter* rewriter, ASTNode* loop){
	int line = loop->loop.line;
	ASTNode* b = newASTBlock(line, NULL);
	ASTNode* var = uniqueVariable(rewriter->table, newASTValue(newI32(0), line, NULL));
	emitNodeToBlock(var, b);
	ASTNode* refToVar = newASTID(var->var.id, line+1, NULL);	
	ASTNode* newLoop = NULL;//newASTSimpleLoop(newASTBinaryOP(refToVar, LESS_OP, rewriteNode(rewriter, loop->loop.n1)->expr.expr, line+1, NULL), rewriteNode(rewriter, loop->loop.b), line+1, NULL);
	int newLine = newLoop->simpleLoop.block->block.numberOfNodes+1;
	ASTNode* inc = newASTAssignment(var->var.id, newASTBinaryOP(newASTID(var->var.id, newLine, NULL), PLUS_OP, newASTValue(newI32(1), newLine, NULL), newLine, NULL), newLine, NULL);
	emitNodeToBlock(inc, newLoop->simpleLoop.block);
	emitNodeToBlock(newLoop, b);
	return b;
}

ASTNode* rewriteLoopType4(Rewriter* rewriter, ASTNode* loop){
	int line = loop->loop.line;
	ASTNode* b = newASTBlock(line, NULL);
	ASTNode* n1 = rewriteNode(rewriter, loop->loop.n1);
	emitNodeToBlock(n1, b);
	if(n1->type = ASTVariable_NODE_TYPE) b->block.variableCount++;
	ASTNode* newLoop = newASTSimpleLoop(rewriteNode(rewriter, loop->loop.n2)->expr.expr, rewriteNode(rewriter, loop->loop.b), line+1, NULL);
	emitNodeToBlock(rewriteNode(rewriter, loop->loop.n3), newLoop->simpleLoop.block);
	emitNodeToBlock(newLoop, b);
	return b;
}

ASTNode* rewriteForLoop(Rewriter* rewriter, ASTNode* loop){
	ASTNode* first = loop->loop.n1;
	if(first == NULL)return rewriteLoopType1(rewriter, loop);
	else {
		if(loop->loop.n2 == NULL){
			if((first->type == ASTExpression_NODE_TYPE) && (getTrivialType(first->expr.t) == I32_TYPE))return rewriteLoopType3(rewriter, loop);
			else return rewriteLoopType2(rewriter, loop);
		}
		else return rewriteLoopType4(rewriter, loop);
		//else{
	}
}

ASTNode* rewriteElse(Rewriter* rewriter, ASTNode* elseS){
	ASTNode* e = (ASTNode*)malloc(sizeof(ASTNode));
	e->type = ASTElse_NODE_TYPE;
	e->elseS.s = rewriteNode(rewriter, elseS->elseS.s);
	return e;
}

ASTNode* rewriteIf(Rewriter* rewriter, ASTNode* ifS){
	ASTNode* i = (ASTNode*)malloc(sizeof(ASTNode));
	i->type = ASTIf_NODE_TYPE;
	i->ifS.expr = rewriteNode(rewriter, ifS->ifS.expr);
	i->ifS.s = rewriteNode(rewriter, ifS->ifS.s);
	if(ifS->ifS.elseS != NULL) i->ifS.elseS = rewriteElse(rewriter, ifS->ifS.elseS);
	return i;
}

ASTNode* rewriteNode(Rewriter* rewriter, ASTNode* n){
	switch(n->type){
		case ASTPrint_NODE_TYPE: return rewritePrint(rewriter, n);
		case ASTExpression_NODE_TYPE:return rewriteExpression(rewriter, n);		
		case ASTBinaryOP_NODE_TYPE:	return rewriteBinaryOP(rewriter, n);		
		case ASTUnaryOP_NODE_TYPE:return rewriteUnaryOP(rewriter, n);				
		case ASTID_NODE_TYPE:return rewriteID(rewriter, n);
		case ASTAssignment_NODE_TYPE:return rewriteAssignment(rewriter, n);
		case ASTString_NODE_TYPE:return rewriteString(rewriter, n);
		case ASTValue_NODE_TYPE:return rewriteValue(rewriter, n);
		case ASTVariable_NODE_TYPE:	return rewriteVariable(rewriter, n);
		case ASTBlock_NODE_TYPE:return rewriteBlock(rewriter, n);
		case ASTForLoop_NODE_TYPE:return rewriteForLoop(rewriter, n);
		case ASTIf_NODE_TYPE:return rewriteIf(rewriter, n);
		default:{
			printf("Cannot rewrite node of given type\n");
			printf("%d\n", n->type);
			exit(1);
		}
	}
}

//make a new ast, free the old one
AST* rewrite(Rewriter* rewriter, AST* ast){
	for(int i = 0; i < ast->numberOfNodes; i++){
		emitNode(rewriteNode(rewriter, &ast->nodes[i]), rewriter->rewrittenAST);
	}
	return rewriter->rewrittenAST;
}

VariableTable* newVariableTable(){
	VariableTable* table = (VariableTable*)malloc(sizeof(VariableTable));
	table->variables = (String**)malloc(sizeof(String*));
	table->count = 0;
	table->cappacity = 1;
}

Rewriter* newRewriter(AST* ast){
	Rewriter* rewriter = (Rewriter*)malloc(sizeof(Rewriter));
	//rewriter->rewrittenAST = newAST();
	rewriter->ast = ast;
	rewriter->table = newVariableTable();
	return rewriter;
}

void freeVariableTable(VariableTable* t){
	free(t->variables);
	free(t);
}

void freeRewriter(Rewriter* r){
	freeVariableTable(r->table);
	freeAST(r->rewrittenAST, false);
	free(r);
}*/