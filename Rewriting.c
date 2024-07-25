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
	ASTNode* n = (ASTNode*)malloc(sizeof(ASTNode));
	ASTVariable var;
	var.id = uniqueID(t)->str;
	//printf("%s\n", var.id);
	var.expr = expr;
	var.type = NULL;
	n->var = var;
	emitVariable(t, var.id);
	return n;
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
ASTNode* rewriteID(Rewriter* rewriter, ASTNode* n){
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	ASTID id;
	id.id = n->id.id;
	node->id = id;
	node->type = ASTID_NODE_TYPE;
	return node;
}

ASTNode* rewriteAssignment(Rewriter* rewriter, ASTNode* n){
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	ASTAssignment ass;
	ass.id = n->ass.id;
	ass.expr = rewriteNode(rewriter, n->ass.expr);
	node->ass = ass;
	node->type = ASTAssignment_NODE_TYPE;
	return node;
}

ASTNode* rewriteString(Rewriter* rewriter, ASTNode* n){
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	ASTString str;
	str.str = n->str.str;
	node->str = str;
	node->type = ASTString_NODE_TYPE;
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
ASTNode* rewriteVariable(Rewriter* rewriter, ASTNode* n){
	ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
	ASTVariable var;
	var.id = n->var.id;
	var.expr = rewriteNode(rewriter, n->var.expr);
	node->var = var;
	node->type = ASTVariable_NODE_TYPE;
	emitVariable(rewriter->table, n->var.id);
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
		if(n2->type == ASTVariable_NODE_TYPE) b.variableCount++;
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
	ASTNode* b = newASTBlock(-1, NULL);
	
	//Build zero expression
	ASTNode* zero = (ASTNode*)malloc(sizeof(ASTValue));
	zero->type = ASTValue_NODE_TYPE;
	zero->value.v.type = I32_VAL;
	zero->value.v.i32 = 0;
	ASTNode* var = uniqueVariable(rewriter->table, zero);
	var->type = ASTVariable_NODE_TYPE;
	ASTNode* refToVar = (ASTNode*)malloc(sizeof(ASTNode));
	refToVar->type = ASTID_NODE_TYPE;
	ASTID local;
	local.id = var->var.id;
	refToVar->id = local;
	
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
	inc->type = ASTAssignment_NODE_TYPE;
	inc->ass.id = var->var.id;
	ASTNode* addOne = (ASTNode*)malloc(sizeof(ASTNode));
	addOne->type = ASTBinaryOP_NODE_TYPE;
	addOne->binaryOP.op = PLUS_OP;
	addOne->binaryOP.lhs = (ASTNode*)malloc(sizeof(ASTNode));
	addOne->binaryOP.rhs = (ASTNode*)malloc(sizeof(ASTNode));
	addOne->binaryOP.lhs->type = ASTID_NODE_TYPE;
	addOne->binaryOP.rhs->type = ASTValue_NODE_TYPE;
	addOne->binaryOP.lhs->id.id = var->var.id;
	addOne->binaryOP.rhs->value.v.i32 = 1;
	inc->ass.expr = addOne;
	emitNodeToBlock(inc, nLoop.block);
	emitNodeToBlock(newLoop, b);
	return b;
}

ASTNode* rewriteLoopType4(Rewriter* rewriter, ASTNode* loop){
	//for(s1; s2; s3;){...} -> {s1; for s2 {...; s3;} }
	ASTNode* b = newASTBlock(-1, NULL);
	ASTNode* x = rewriteNode(rewriter, loop->loop.n1);
	emitNodeToBlock(x, b);
	if(x->type = ASTVariable_NODE_TYPE) b->block.variableCount++;
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
		case ASTID_NODE_TYPE:{
			return rewriteID(rewriter, n);
		}
		case ASTAssignment_NODE_TYPE:{
			return rewriteAssignment(rewriter, n);
		}
		case ASTString_NODE_TYPE:{
			return rewriteString(rewriter, n);
		}
		case ASTValue_NODE_TYPE:{
			return rewriteValue(rewriter, n);
		}
		case ASTVariable_NODE_TYPE:{
			return rewriteVariable(rewriter, n);
		}
		case ASTBlock_NODE_TYPE:{
			return rewriteBlock(rewriter, n);
		}
		case ASTForLoop_NODE_TYPE:{//should be a and b for local variables
			//printf("Start\n");
			//ASTNode* n2 = rewriteForLoop(rewriter, n);
			//printASTNode(rewriter->rewrittenAST, n2);
			//printf("End\n");
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

VariableTable* newVariableTable(){
	VariableTable* table = (VariableTable*)malloc(sizeof(VariableTable));
	table->variables = (String**)malloc(sizeof(String*));
	table->count = 0;
	table->cappacity = 1;
}

Rewriter* newRewriter(AST* ast){
	Rewriter* rewriter = (Rewriter*)malloc(sizeof(Rewriter));
	rewriter->rewrittenAST = newAST();
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
}




// macros, operators, generics, rewriting feature... lots of need for a dedicated rewriter...