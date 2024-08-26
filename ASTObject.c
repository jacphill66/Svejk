#include "ASTObject.h"

/* Freeing Functions for ASTObjects */

void freeASTErrorNode(ASTNode* node){
	free(node);
}

char* freeASTForm(ASTNode* node, bool typeFreeing){
	if(node->form.nodes == NULL) return newString("Attempting to Free NULL Nodes in a Form");
	for(int i = 0; i < node->form.size; i++){
		char* res = freeASTNode(node->form.nodes[i], typeFreeing);
		if(res != NULL) return res;
	}
	if(typeFreeing){
		char* res = freeType(node->form.t);
		if(res!=NULL) return res;
	}
	char* res = freeTemplate(node->form.temp);
	if(res != NULL) return res;
	return NULL;
}

char* freeASTBinaryOP(ASTNode* node, bool typeFreeing){
	if(node->binaryOP.lhs != NULL) {
		char* res = freeASTNode(node->binaryOP.lhs, typeFreeing);
		free(node->binaryOP.lhs);
		if(res != NULL) return res;
	}
	else return newString("Invalid Freeing of Binary OP");
	if(node->binaryOP.rhs != NULL){
		char* res = freeASTNode(node->binaryOP.rhs, typeFreeing);
		free(node->binaryOP.rhs);
		if(res != NULL) return res;
	}
	else return newString("Invalid Freeing of Binary OP");
	if(typeFreeing) {
		char* res = freeType(node->binaryOP.t);
		if(res != NULL) return res;
	}
	if(node->binaryOP.op != NULL) free(node->binaryOP.op);
	else return newString("Invalid Freeing of Binary OP");
	return NULL;
}

char* freeASTString(ASTNode* node, bool typeFreeing){
	if(typeFreeing) {
		char* res = freeType(node->str.t);
		if(res != NULL) return res;
	}
	free(node->str.str);
	return NULL;
}

char* freeASTNumber(ASTNode* node, bool typeFreeing){
	if(typeFreeing) {
		char* res = freeType(node->num.t);
		if(res != NULL) return res;
	}
	if(node->num.num != NULL) free(node->num.num);
	else return newString("Invalid Freeing of Binary OP");
}

char* freeASTUnaryOP(ASTNode* node, bool typeFreeing){
	if(node->unaryOP.opperand != NULL) {
		char* res = freeASTNode(node->unaryOP.opperand, typeFreeing);
		free(node->unaryOP.opperand);
		if(res != NULL) return res;
	}
	else return newString("Invalid Freeing of Unary OP");
	if(typeFreeing) {
		char* res = freeType(node->unaryOP.t);
		if(res != NULL) return res;
	}
	if(node->unaryOP.op != NULL) free(node->unaryOP.op);
	else return newString("Invalid Freeing of Unary OP");
	return NULL;
}

char* freeASTID(ASTNode* node, bool typeFreeing){
	if(typeFreeing) {
		char* res = freeType(node->id.t);
		if(res != NULL) return res;
	}	
	if(node->id.id != NULL) free(node->id.id);
	else return newString("Invalid Freeing of ID");
	return NULL;
}

char* freeASTValue(ASTNode* node, bool typeFreeing){
	if(typeFreeing) {
		char* res = freeType(node->value.t);
		if(res != NULL) return res;
	}	
	return NULL;
}

char* freeASTExpression(ASTNode* node, bool typeFreeing){
	if(node->expr.expr != NULL){
		char* res = freeASTNode(node->expr.expr, typeFreeing);
		free(node->expr.expr);
		if(res != NULL) return res;
	}
	else return newString("Invalid Freeing of Expression");
	if(typeFreeing) {
		char* res = freeType(node->expr.t);
		if(res != NULL) return res;
	}
	return NULL;
}

char* freeASTWhile(ASTNode* node, bool typeFreeing){
	if(node->whileExpr.expr1 != NULL){
		char* res = freeASTNode(node->whileExpr.expr1, typeFreeing);
		free(node->whileExpr.expr1);
		if(res != NULL) return res;
	}
	else return newString("Invalid Freeing of While Loop");
	if(node->whileExpr.expr2 != NULL) {
		char* res = freeASTNode(node->whileExpr.expr2, typeFreeing);
		free(node->whileExpr.expr2);
		if(res!=NULL) return res;
	}
	else return newString("Invalid Freeing of While Loop");
	if(typeFreeing) {
		char* res = freeType(node->whileExpr.t);
		if(res != NULL) return res;
	}	
	return NULL;
}

char* freeASTBlock(ASTNode* node, bool typeFreeing){
	if(node->block.nodes != NULL){
		for(long i = 0; i < node->block.numberOfNodes; i++)	{
			if(&node->block.nodes[i] != NULL){
				char* res = freeASTNode(&node->block.nodes[i], typeFreeing);
				if(res != NULL) {
					free(node->block.nodes);
					return res;
				}
			}
			else return newString("Invalid Freeing of Block Expression");
		}
		free(node->block.nodes);
	}
	else return newString("Invalid Freeing of Block Expression");
	if(typeFreeing) {
		char* res = freeType(node->block.t);
		if(res != NULL) return res;
	}	
	return NULL;
}

char* freeASTVariable(ASTNode* node, bool typeFreeing){
	if(node->var.expr != NULL) {
		char* res = freeASTNode(node->var.expr, typeFreeing);
		free(node->var.expr);
		if(res != NULL) return res;
	}
	else return newString("Invalid Freeing of Variable");
	if(typeFreeing) {
		char* res = freeType(node->var.t);
		if(res != NULL) return res;
	}	 
	if(node->var.id != NULL) free(node->var.id);
	else return newString("Invalid Freeing of Variable");
	return NULL;
}

char* freeASTAssignment(ASTNode* node, bool typeFreeing){
	if(node->ass.expr != NULL) {
		char* res = freeASTNode(node->ass.expr, typeFreeing);
		free(node->ass.expr);
		if(res != NULL) return res;
	}
	else return newString("Invalid Freeing of Assignment");
	if(typeFreeing) {
		char* res = freeType(node->ass.t);
		if(res != NULL) return res;
	}	 
	if(node->ass.id != NULL) free(node->ass.id);
	else return newString("Invalid Freeing of Assignment");
	return NULL;	
}

char* freeASTElse(ASTNode* node, bool typeFreeing){
	if(node->elseExpr.expr !=  NULL){
		char* res = freeASTNode(node->elseExpr.expr, typeFreeing);
		free(node->elseExpr.expr);
		if(res != NULL) return res;
	}
	else return newString("Invalid Freeing of Else");
	if(typeFreeing) {
		char* res = freeType(node->elseExpr.t);
		if(res != NULL) return res;
	}	
	return NULL;	
}

char* freeASTIf(ASTNode* node, bool typeFreeing){
	if(node->ifExpr.expr1 !=  NULL){
		char* res = freeASTNode(node->ifExpr.expr1, typeFreeing);
		free(node->ifExpr.expr1);
		if(res != NULL) return res;
	}
	else return newString("Invalid Freeing of If");
	if(node->ifExpr.expr2 != NULL){
		char* res = freeASTNode(node->ifExpr.expr2, typeFreeing);
		free(node->ifExpr.expr2);
		if(res != NULL) return res;
	}
	else return newString("Invalid Freeing of If");
	if(node->ifExpr.elseExpr != NULL) {
		char* res = freeASTNode(node->ifExpr.elseExpr, typeFreeing);
		free(node->ifExpr.elseExpr);
		if(res != NULL) return res;
	}
	if(typeFreeing) {
		char* res = freeType(node->ifExpr.t);
		if(res != NULL) return res;
	}	
	return NULL;
}

char* freeASTNode(ASTNode* node, bool typeFreeing){
	switch(node->type){
		case ASTBinaryOP_NODE_TYPE : return freeASTBinaryOP(node, typeFreeing);
		case ASTString_NODE_TYPE: return freeASTUnaryOP(node, typeFreeing);
		case ASTUnaryOP_NODE_TYPE : return freeASTUnaryOP(node, typeFreeing);
		case ASTID_NODE_TYPE: return freeASTID(node, typeFreeing);
		case ASTValue_NODE_TYPE: return freeASTValue(node, typeFreeing);
		case ASTExpression_NODE_TYPE: return freeASTExpression(node, typeFreeing);
		case ASTWhile_NODE_TYPE: return freeASTWhile(node, typeFreeing);
		case ASTBlock_NODE_TYPE: return freeASTBlock(node, typeFreeing);
		case ASTVariable_NODE_TYPE: return freeASTVariable(node, typeFreeing);
		case ASTAssignment_NODE_TYPE: return freeASTAssignment(node, typeFreeing);
		case ASTIf_NODE_TYPE: return freeASTIf(node, typeFreeing);
		case ASTElse_NODE_TYPE: return freeASTElse(node, typeFreeing);
		case ASTNumber_NODE_TYPE: return freeASTNumber(node, typeFreeing);
		default: return newString("Attempting to Free Node of Invalid Type");
	}
}

char* freeAST(AST* ast, bool freeType){
	for(long i = 0; i < ast->numberOfNodes; i++) {
		char* res = freeASTNode(&ast->nodes[i], freeType);
		if(res != NULL) {
			free(ast);
			return res;
		}
	}
	free(ast);
}

/* Initialization Functions for ASTObjects */

ASTNode* newASTNode(ASTNodeType t){
	ASTNode* n = (ASTNode*)malloc(sizeof(ASTNode));
	n->type = t;
	return n;
}

ASTNode* newASTBinaryOP(ASTNode* lhs, char* op, ASTNode* rhs, int line, Type* t){
	ASTNode* n = newASTNode(ASTBinaryOP_NODE_TYPE);
	n->binaryOP.lhs = lhs;
	n->binaryOP.op = op;
	n->binaryOP.rhs = rhs;
	n->binaryOP.line = line;
	n->binaryOP.t = t;
	return n;
}

ASTNode* newASTUnaryOP(ASTNode* opperand, char* op, int line, Type* t){
	ASTNode* n = newASTNode(ASTUnaryOP_NODE_TYPE);
	n->unaryOP.opperand = opperand;
	n->unaryOP.op = op;
	n->unaryOP.line = line;
	n->unaryOP.t = t;
	return n;
}

ASTNode* newASTValue(Value v, int line, Type* t){
	ASTNode* n = newASTNode(ASTValue_NODE_TYPE);
	n->value.v = v;
	n->value.line = line;
	n->value.t = t;
	return n;
}

ASTNode* newASTExpression(ASTNode* expr, bool statement, int line, Type* t){
	ASTNode* n = newASTNode(ASTExpression_NODE_TYPE);
	n->expr.expr = expr;
	n->expr.statement = statement;
	n->expr.line = line;
	n->expr.t = t;
	return n;
}

ASTNode* newASTID(char* id, int line, Type* t){
	ASTNode* n = newASTNode(ASTID_NODE_TYPE);
	n->id.id = id;
	n->id.line = line;
	n->id.t = t;
	return n;
}

ASTNode* newASTAssignment(char* id, ASTNode* expr, int line, Type* t){
	ASTNode* n = newASTNode(ASTAssignment_NODE_TYPE);
	n->ass.id = id;
	n->ass.expr = expr;
	n->ass.line = line;
	n->ass.t = t;
	return n;
}

ASTNode* newASTVariable(char* id, ASTNode* expr, Type* type, int line, Type* t){
	ASTNode* n = newASTNode(ASTVariable_NODE_TYPE);
	n->var.id = id;
	n->var.expr = expr;
	n->var.type = type;
	n->var.line = line;
	n->var.t = t;
	return n;
}

ASTNode* newASTPrint(ASTNode* expr, int line, Type* t){
	ASTNode* n = newASTNode(ASTPrint_NODE_TYPE);
	n->print.expr = expr;
	n->print.line = line;
	n->print.t = t;
	return n;
}

ASTNode* newASTBlock(int line, Type* t){
	ASTNode* n = newASTNode(ASTBlock_NODE_TYPE);
	n->block.nodes = (ASTNode*)malloc(sizeof(ASTNode));
	n->block.cappacity = 1;
	n->block.numberOfNodes = 0;
	n->block.variableCount = 0;
	n->block.line = line;
	n->block.t = t;
	return n;
}

ASTNode* newASTString(char* str, int line, Type* t){
	ASTNode* n = newASTNode(ASTString_NODE_TYPE);
	n->str.str = str;
	n->str.line = line;
	n->str.t = t;
	return n;
}

ASTNode* newASTForLoop(int line, Type* t){
	ASTNode* n = newASTNode(ASTForLoop_NODE_TYPE);
	n->loop.n1 = NULL;
	n->loop.n2 = NULL;
	n->loop.n3 = NULL;
	n->loop.min = NULL;
	n->loop.max = NULL;
	n->loop.b = NULL;
	n->loop.line = line;
	n->loop.t = t;
	return n;
}

ASTNode* newASTSimpleLoop(ASTNode* expr, ASTNode* block, int line, Type* t){
	ASTNode* n = newASTNode(ASTLoop_NODE_TYPE);
	n->simpleLoop.expr = expr;
	n->simpleLoop.block = block;
	n->simpleLoop.line = line;
	n->simpleLoop.t = t;
	return n;
}

ASTNode* newASTElse(ASTNode* expr, int line, Type* t){
	ASTNode* n = newASTNode(ASTElse_NODE_TYPE);
	n->elseExpr.expr = expr;
	n->elseExpr.line = line;
	n->elseExpr.t = t;
	return n;
}

ASTNode* newASTIf(ASTNode* expr1, ASTNode* expr2, ASTNode* elseExpr, int line, Type* t){
	ASTNode* n = newASTNode(ASTIf_NODE_TYPE);
	n->ifExpr.expr1 = expr1;
	n->ifExpr.expr2 = expr2;
	n->ifExpr.elseExpr = elseExpr;
	n->ifExpr.line = line;
	n->ifExpr.t = t;
	return n;
}

ASTNode* newASTTable(TableType type, int line, Type* t){
	ASTNode* n = newASTNode(ASTTable_NODE_TYPE);
	n->table.nodes = (ASTNode*)malloc(sizeof(ASTNode));
	n->table.keys = (char**) malloc(sizeof(char*));
	n->table.type = type;
	n->table.cappacity = 1;
	n->table.numberOfNodes = 0;
	n->table.line = line;
	n->table.t = t;
	return n;
}

ASTNode* newASTWhile(ASTNode* expr1, ASTNode* expr2, int line, Type* t){
	ASTNode* n = newASTNode(ASTWhile_NODE_TYPE);
	n->whileExpr.expr1 = expr1;
	n->whileExpr.expr2 = expr2;
	n->whileExpr.line = line;
	n->whileExpr.t = t;
	return n;
}

ASTNode* newASTError(const char* message, int line){
	ASTNode* errorNode = newASTNode(ASTError_NODE_TYPE);
	errorNode->error.msg = message;
	errorNode->error.line = line;
	return errorNode;
}

ASTNode* newASTNumber(char* number, bool integer, int line, Type* t){
	ASTNode* num = newASTNode(ASTNumber_NODE_TYPE);
	num->num.num = number;
	num->num.integer = integer;
	num->num.t = t;
	return num;
}

ASTNode* newASTForm(ASTNode** nodes, int size, int line, Type* t, Template* temp){
	ASTNode* form = newASTNode(ASTForm_NODE_TYPE);
	form->form.nodes = nodes;
	form->form.size = size;
	form->form.t = t;
	form->form.temp = temp;
	return form;
}

/* Emitting Functions */

ASTNode* resizeNodes(ASTNode* nodes, long cappacity){
	ASTNode* newNodes = (ASTNode*)realloc(nodes, 2*cappacity*sizeof(ASTNode));
	if(newNodes == NULL) return NULL;
	return newNodes;
}

char* emitNode(ASTNode* node, AST* ast){
	ast->nodes[ast->numberOfNodes] = *node;
	ast->numberOfNodes++;
	if(ast->numberOfNodes == ast->cappacity){
		ast->nodes = resizeNodes(ast->nodes, ast->cappacity);
		if(ast->nodes == NULL) return newString("Invalid Resizing");
		ast->cappacity *= 2;
	}
	return NULL;
}

char* emitNodeToBlock(ASTNode* node, ASTNode* b){
	b->block.nodes[b->block.numberOfNodes] = *node;
	b->block.numberOfNodes++;
	if(node != NULL && node->type == ASTVariable_NODE_TYPE) b->block.variableCount++;
	if(b->block.numberOfNodes == b->block.cappacity){
		b->block.nodes = resizeNodes(b->block.nodes, b->block.cappacity);
		if(b->block.nodes != NULL) return newString("Invalid Resizing");
		b->block.cappacity *= 2;
	}
	return NULL;
}

/* Printing Functions */

char* printASTBinaryOP(ASTNode* node){
	if(node->binaryOP.lhs != NULL) {
		printf("(");
		char* res = printASTNode(node->binaryOP.lhs);
		if(res != NULL) return res;
	}
	else return newString("Invalid Printing of Binary OP");
	if(node->binaryOP.op != NULL) printf("%s", node->binaryOP.op);
	else newString("Attempting to Print Invalid Operator");
	if(node->binaryOP.rhs != NULL) {
		char* res = printASTNode(node->binaryOP.rhs);
		if(res != NULL) return res;
		printf(")");
	}
	else return newString("Invalid Printing of Binary OP");
	return NULL;
}

char* printASTString(ASTNode* node){
	if(node->str.str != NULL) {
		printf("str[%s]", node->str.str);
		return NULL;
	}
	return newString("Invalid Printing of String");
}

char* printASTUnaryOP(ASTNode* node){
	if(node->unaryOP.opperand != NULL) {
		printf("(");
		char* res = printASTNode(node->unaryOP.opperand);
		printf(")");
		if(res != NULL) return res;
	}
	else return newString("Invalid Printing of Unary OP");
	if(node->unaryOP.op != NULL) printf("%s", node->unaryOP.op);
	else return newString("Invalid Printing of Unary OP");
	return NULL;
}

char* printASTID(ASTNode* node){
	if(node->id.id != NULL) printf("ID[%s]", node->id.id);
	else return newString("Invalid Printing of ID");
	return NULL;
}

char* printASTValue(ASTNode* node){
	char* res = printValue(node->value.v);
	if(res != NULL) return res;
	return NULL;
}

char* printASTExpression(ASTNode* node){
	if(node->expr.expr != NULL) {
		printf("Expr[");
		char* res = printASTNode(node->expr.expr);
		if(res != NULL) return res;
		printf("]");
		return NULL;
	}
	else return newString("Invalid Printing of Expression");
	return NULL;
}

char* printASTWhile(ASTNode* node){
	printf("while [");
	if(node->whileExpr.expr1 != NULL){
		char* res = printASTNode(node->whileExpr.expr1);
		if(res != NULL) panic("Invalid Printing of While Loop1");
	}
	else panic("Invalid Printing of While Loop2");
	printf("] [");
	if(node->whileExpr.expr2 != NULL) {
		char*res = printASTNode(node->whileExpr.expr2);
		if(res != NULL) panic("Invalid Printing of While Loop3");
	}
	else panic("Invalid Printing of While Loop4");
	printf("]");
	return NULL;
}

char* printASTBlock(ASTNode* node){
	if(node->block.nodes != NULL){
		printf("{");
		for(long i = 0; i < node->block.numberOfNodes; i++)	{
			if(&node->block.nodes[i] != NULL){
				char* res = printASTNode(&node->block.nodes[i]);
				if(res != NULL) return res;
			}
			else return newString("Invalid Printing of Block Expression");
			printf("\n");
		}
		printf("}");
	}
	else return newString("Invalid Printing of Block Expression");
	return NULL;
}

char* printASTVariable(ASTNode* node){
	if(node->var.id != NULL) printf("let %s: ", node->var.id);
	else return newString("Invalid Printing of Variable");
	char* res = printType(node->var.type);
	if(res != NULL) return res;
	if(node->var.expr != NULL) {
		char* res = printASTNode(node->var.expr);
		if(res != NULL) return res;
	}
	else return newString("Invalid Printing of Variable");
	return NULL;
}

char* printASTAssignment(ASTNode* node){
	if(node->ass.expr != NULL){
		char* res = printASTNode(node->ass.expr);
		if(res != NULL) return res;
	}
	else return newString("Invalid Printing of Assignment");
	return NULL;
}

char* printASTElse(ASTNode* node){
	if(node->elseExpr.expr !=  NULL) {
		printf("else [");
		char* res = printASTNode(node->elseExpr.expr);
		printf("]");
		if(res != NULL) return res;
	}
	else return newString("Invalid Printing of Else");
	return NULL;
}

char* printASTIf(ASTNode* node){
	printf("if [");
	if(node->ifExpr.expr1 !=  NULL) {
		char* res = printASTNode(node->ifExpr.expr1);
		if(res != NULL) return res;
	}
	else return newString("Invalid Freeing of If");
	printf("] [");
	if(node->ifExpr.expr2 != NULL) {
		char* res = printASTNode(node->ifExpr.expr2);
		if(res != NULL) return res;
	}
	else return newString("Invalid Freeing of If");
	printf("] ");
	if(node->ifExpr.elseExpr != NULL){
		char* res = printASTNode(node->ifExpr.elseExpr);
		if(res != NULL) return res;
	}
	return NULL;
}

char* printASTError(ASTNode* node){
	printf("Parsing Error[%s] on line[%d]", node->error.msg, node->error.line),1;
	return NULL;
}

char* printASTNumber(ASTNode* node){
	if(node->num.integer){
		if(node->num.num!=NULL) printf("INT[%s]", node->num.num);
		else return newString("Invalid Ingeger");
	}
	else{
		if(node->num.num!=NULL) printf("FLOAT[%s]", node->num.num);
		else return newString("Invalid Ingeger");
	}
	return NULL;
}

char* printASTTemplate(ASTNode* node){
	printf("Form[\n");
	for(int i = 0; i < node->form.size; i++){
		char* res = printASTNode(node->form.nodes[i]);
		if(res != NULL) return res;
	}
	printf("]\n");
	return NULL;
}

char* printASTNode(ASTNode* node){
	switch(node->type){
		case ASTBinaryOP_NODE_TYPE: return printASTBinaryOP(node);
		case ASTUnaryOP_NODE_TYPE : return printASTUnaryOP(node);
		case ASTString_NODE_TYPE : return printASTString(node);
		case ASTID_NODE_TYPE: return printASTID(node);
		case ASTNumber_NODE_TYPE: return printASTNumber(node);
		case ASTValue_NODE_TYPE: return printASTValue(node);
		case ASTExpression_NODE_TYPE: return printASTExpression(node);
		case ASTVariable_NODE_TYPE: return printASTVariable(node);
		case ASTAssignment_NODE_TYPE: return printASTAssignment(node);
		case ASTBlock_NODE_TYPE: return printASTBlock(node);
		case ASTIf_NODE_TYPE: return printASTIf(node);
		case ASTElse_NODE_TYPE: return printASTElse(node);
		case ASTWhile_NODE_TYPE: return printASTWhile(node);
		case ASTError_NODE_TYPE: return printASTError(node);
		default : return newString("Attempting to Print Invalid Node Type");
	}
}

void printAST(AST* ast){
	printf("AST:\n");
	for(long i = 0; i < ast->numberOfNodes; i++){
		printASTNode(&ast->nodes[i]); printf("\n");
	}
	printf("\n");
}


/*

char** resizeKeys(char** keys, long cappacity){
	char** newKeys = (char**)realloc(keys, 2*cappacity*sizeof(char*));
	if(newKeys == NULL){
		printf("could not resize AST-Nodes!\n");
		free(keys);
		exit(1);
	}
	return newKeys;
}

void emitNodeToTable(char* key, ASTNode* node, ASTNode* t){
	t->table.nodes[t->table.numberOfNodes] = *node;
	if(key != NULL) t->table.keys[t->table.numberOfNodes] = key;
	t->table.numberOfNodes++;
	if(t->table.numberOfNodes == t->table.cappacity){
		t->table.nodes = resizeNodes(t->table.nodes, t->table.cappacity);
		if(key != NULL) t->table.keys = resizeKeys(t->table.keys, t->table.cappacity);
		t->table.cappacity *= 2;
	}

}

*/