#include "Rewriting.h"

//Line number is deprecated and can't realistically be restored 
//Type is deprecated but can be restored

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

void emitLocal(VariableTable* t, char* id){
	t->localVariables[t->localCount] = newString(id);
	t->localCount++;
	if(t->localCount == t->localCappacity){
		t->localVariables = resizeStrings(t->localVariables, t->localCappacity);
		t->localCappacity *= 2;
	}
}

void emitGlobal(VariableTable* t, char* id){
	t->globalVariables[t->globalCount] = newString(id);
	t->globalCount++;
}

String* addNextLetter(String* str, char c){
	//I know char is A-Z, _, 0-9, a-z
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
	
	/*
	Algorithm:
		if the current pos is less than the current string, add a new letter
		if the pos == length-1, strings are same length, compare them
			- if they are the same add a letter
			- if they are different, move on to the next id
	*/
	for(int i = 0; i < t->globalCount; i++){
		
		if(t->globalVariables[i]->length > pos){
			addNextLetter(str, t->globalVariables[i]->str[pos]);
			pos+=1;
		}
		else{
			if(strcmp(t->globalVariables[i]->str, str->str)==0){
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
	for(int i = 0; i < t->localCount; i++){
		if(t->localVariables[i]->length > pos){
			addNextLetter(str, t->localVariables[i]->str[pos]);
			pos+=1;
		}
		else{
			if(strcmp(t->localVariables[i]->str, str->str)==0){
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
	ASTNode* n = (ASTNode*)malloc(sizeof(ASTNode));
	ASTLocalVariable var;
	var.id = uniqueID(t)->str;
	printf("%s\n", var.id);
	var.expr = expr;
	var.type = NULL;
	var.offset = -3;
	n->localVar = var;
	emitLocal(t, var.id);
	return n;
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

ASTNode* addMinAndMax(Rewriter* rewriter,ASTNode* loop){
	//later...
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
	node->binaryOP = op;
	node->type = ASTBinaryOP_NODE_TYPE;
	return node;
}

ASTNode* rewriteUnaryOP(Rewriter* rewriter, ASTNode* n){
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	ASTUnaryOP op;
	op.op = n->unaryOP.op;
	op.opperand = rewriteNode(rewriter, n->unaryOP.opperand);
	node->unaryOP = op;
	node->type = ASTUnaryOP_NODE_TYPE;
	return node;
}
ASTNode* rewriteGlobalID(Rewriter* rewriter, ASTNode* n){
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	ASTGlobalID id;
	id.id = n->globalID.id;
	id.index = n->globalID.index;
	node->globalID = id;
	node->type = ASTGlobalID_NODE_TYPE;
	return node;
}
ASTNode* rewriteLocalID(Rewriter* rewriter, ASTNode* n){
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	ASTLocalID id;
	id.id = n->localID.id;
	node->localID = id;
	node->type = ASTLocalID_NODE_TYPE;
	return node;
}
ASTNode* rewriteGlobalAssignment(Rewriter* rewriter, ASTNode* n){
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	ASTGlobalAssignment ass;
	ass.id = n->globalAss.id;
	ass.expr = rewriteNode(rewriter, n->globalAss.expr);
	ass.index = n->globalAss.index;
	node->globalAss = ass;
	node->type = ASTGlobalAssignment_NODE_TYPE;
	return node;
}
ASTNode* rewriteLocalAssignment(Rewriter* rewriter, ASTNode* n){
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	ASTLocalAssignment ass;
	ass.id = n->localAss.id;
	ass.expr = rewriteNode(rewriter, n->localAss.expr);
	node->localAss = ass;
	node->type = ASTLocalAssignment_NODE_TYPE;
	return node;
}
ASTNode* rewriteString(Rewriter* rewriter, ASTNode* n){
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	ASTString str;
	str.str = n->str.str;
	str.index = n->str.index;
	node->str = str;
	node->type = ASTString_NODE_TYPE;
	rewriter->rewrittenAST->stringCount++;
	return node;
}
ASTNode* rewriteValue(Rewriter* rewriter, ASTNode* n){
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	ASTValue val;
	val.v = n->value.v;
	node->value = val;
	node->type = ASTValue_NODE_TYPE;
	return node;
}
ASTNode* rewriteGlobalVariable(Rewriter* rewriter, ASTNode* n){
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	ASTGlobalVariable var;
	var.id = n->globalVar.id;
	var.expr = rewriteNode(rewriter, n->globalVar.expr);
	var.index = n->globalVar.index;
	node->globalVar = var;
	node->type = ASTGlobalVariable_NODE_TYPE;
	emitGlobal(rewriter->table, n->globalVar.id);
	return node;
}
ASTNode* rewriteLocalVariable(Rewriter* rewriter, ASTNode* n){
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	ASTLocalVariable var;
	var.id = n->localVar.id;
	var.expr = rewriteNode(rewriter, n->localVar.expr);
	node->localVar = var;
	node->type = ASTLocalVariable_NODE_TYPE;
	emitLocal(rewriter->table, var.id);
	return node;
}
ASTNode* rewriteBlock(Rewriter* rewriter, ASTNode* n){
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	ASTBlock b;
	b.variableCount = 0;
	//b.variableCount = n->block.variableCount;
	b.cappacity = 1;//new cappacity
	b.numberOfNodes = 0;//new numebr of nodes
	b.nodes = (ASTNode*)malloc(sizeof(ASTNode));
	node->block = b;
	node->type = ASTBlock_NODE_TYPE;
	for(int i = 0; i < n->block.numberOfNodes; i++) {
		ASTNode* n2 = rewriteNode(rewriter, &n->block.nodes[i]);
		if(n2->type == ASTLocalVariable_NODE_TYPE) b.variableCount++;
		emitNodeToBlock(n2, node);
	}
	return node;
}

ASTNode* rewriteLoopType1(Rewriter* rewriter,ASTNode* loop){
	//missing line and type information
	ASTLoop simpleLoop;
	ASTNode* val = (ASTNode*)malloc(sizeof(ASTNode));
	val->type = ASTValue_NODE_TYPE;
	val->value.v.type = BOOL_VAL;
	val->value.v.boolean = true;
		
	simpleLoop.expr = val;
	simpleLoop.block = rewriteBlock(rewriter, loop->loop.b);
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	node->simpleLoop = simpleLoop;
	node->simpleLoop.line = loop->loop.line;
	node->type = ASTLoop_NODE_TYPE;
	return node;
}

ASTNode* rewriteLoopType2(Rewriter* rewriter,ASTNode* loop){
	//missing line and type information
	ASTLoop nLoop;
	nLoop.expr = rewriteNode(rewriter, loop->loop.n1)->expr.expr;
	nLoop.block = rewriteNode(rewriter, loop->loop.b);
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	node->type = ASTLoop_NODE_TYPE;
	node->simpleLoop = nLoop;
	return node;
}

ASTNode* rewriteLoopType3(Rewriter* rewriter, ASTNode* loop){
	//for expr { ... } -> { let id = expr; for 0 < id { ... id = id + 1;} }
	ASTNode* b = newBlock();
	
	//Build zero expression
	ASTNode* zero = (ASTNode*)malloc(sizeof(ASTValue));
	zero->type = ASTValue_NODE_TYPE;
	zero->value.v.type = I32_VAL;
	zero->value.v.i32 = 0;
	ASTNode* var = uniqueVariable(rewriter->table, zero);
	var->type = ASTLocalVariable_NODE_TYPE;
	ASTNode* refToVar = (ASTNode*)malloc(sizeof(ASTNode));
	refToVar->type = ASTLocalID_NODE_TYPE;
	ASTLocalID local;
	local.id = var->localVar.id;
	refToVar->localID = local;
	
	emitNodeToBlock(var, b);
	b->block.variableCount++;

	ASTNode* binOP = (ASTNode*)malloc(sizeof(ASTNode));
	binOP->type = ASTBinaryOP_NODE_TYPE;
	ASTBinaryOP binOP2;
	binOP2.lhs = refToVar;
	binOP2.op = LESS_OP;
	binOP2.rhs = rewriteNode(rewriter, loop->loop.n1)->expr.expr;//ref to var
	binOP->binaryOP = binOP2;

	ASTNode* newLoop = (ASTNode*)malloc(sizeof(ASTNode));
	newLoop->type = ASTLoop_NODE_TYPE;
	ASTLoop nLoop;
	nLoop.expr = binOP;
	nLoop.block = rewriteNode(rewriter, loop->loop.b);
	newLoop->simpleLoop = nLoop;

	ASTNode* inc = (ASTNode*)malloc(sizeof(ASTNode));
	inc->type = ASTLocalAssignment_NODE_TYPE;
	inc->localAss.id = var->localVar.id;
	ASTNode* addOne = (ASTNode*)malloc(sizeof(ASTNode));
	addOne->binaryOP.lhs = (ASTNode*)malloc(sizeof(ASTNode));
	addOne->binaryOP.rhs = (ASTNode*)malloc(sizeof(ASTNode));
	addOne->binaryOP.lhs->type = ASTLocalID_NODE_TYPE;
	addOne->binaryOP.rhs->type = ASTValue_NODE_TYPE;
	addOne->binaryOP.lhs->localID.id = var->localVar.id;
	addOne->binaryOP.rhs->value.v.i32 = 1;
	inc->localAss.expr = addOne;
	emitNodeToBlock(inc, nLoop.block);
	emitNodeToBlock(newLoop, b);
	return b;
}

ASTNode* rewriteLoopType4(Rewriter* rewriter, ASTNode* loop){
	//for(s1; s2; s3;){...} -> {s1; for s2 {...; s3;} }
	ASTNode* b = newBlock();
	ASTNode* x = rewriteNode(rewriter, loop->loop.n1);
	emitNodeToBlock(x, b);
	if(x->type = ASTLocalVariable_NODE_TYPE) b->block.variableCount++;
	ASTNode* newLoop = (ASTNode*)malloc(sizeof(ASTNode));
	newLoop->type = ASTLoop_NODE_TYPE;
	ASTLoop nLoop;
	nLoop.expr = rewriteNode(rewriter, loop->loop.n2)->expr.expr;
	nLoop.block = rewriteNode(rewriter, loop->loop.b);
	emitNodeToBlock(rewriteNode(rewriter, loop->loop.n3), nLoop.block);
	newLoop->simpleLoop = nLoop;
	emitNodeToBlock(newLoop, b);
	return b;
}

ASTNode* rewriteForLoop(Rewriter* rewriter, ASTNode* loop){
	ASTNode* first = loop->loop.n1;
	if(first == NULL){
		return rewriteLoopType1(rewriter, loop);
	}
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
		case ASTForLoop_NODE_TYPE:{
			return rewriteForLoop(rewriter, n);
		}
		case ASTIf_NODE_TYPE:{
			return rewriteIf(rewriter, n);
		}
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

VariableTable* newVariableTable(int globalCount){
	VariableTable* table = (VariableTable*)malloc(sizeof(VariableTable));
	table->localVariables = (String**)malloc(sizeof(String*));
	table->globalVariables = (String**)malloc(sizeof(String*)*globalCount);
	table->localCount = 0;
	table->globalCount = 0;
}

Rewriter* newRewriter(AST* ast, int globalCount){
	Rewriter* rewriter = (Rewriter*)malloc(sizeof(Rewriter));
	rewriter->rewrittenAST = newAST();
	rewriter->ast = ast;
	rewriter->table = newVariableTable(globalCount);
	return rewriter;
}

void freeVariableTable(VariableTable* t){
	free(t->localVariables);
	free(t->globalVariables);
	free(t);
}

void freeRewriter(Rewriter* r){
	freeVariableTable(r->table);
	freeAST(r->rewrittenAST);
	free(r);
}




// macros, operators, generics, rewriting feature... lots of need for a dedicated rewriter...