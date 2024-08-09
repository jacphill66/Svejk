#include "ASTObject.h"

void freeASTNode(ASTNode* node, bool typeFreeing){
	ASTNodeType type = node->type;
	switch(node->type){
		case ASTBinaryOP_NODE_TYPE :{
			freeASTNode(node->binaryOP.lhs, typeFreeing);
			freeASTNode(node->binaryOP.rhs, typeFreeing);
			free(node->binaryOP.lhs);
			free(node->binaryOP.rhs);
			if(typeFreeing) freeType(node->binaryOP.t);
			break;
		}
		case ASTString_NODE_TYPE:{
			if(typeFreeing) freeType(node->str.t);
			break;
		}
		case ASTUnaryOP_NODE_TYPE : {
			freeASTNode(node->unaryOP.opperand, typeFreeing);
			free(node->unaryOP.opperand);
			if(typeFreeing) freeType(node->unaryOP.t);
			break;
		}
		case ASTCallOP_NODE_TYPE:{
			freeASTNode(node->callOP.opperand, typeFreeing);
			free(node->callOP.opperand);
			if(typeFreeing) freeType(node->callOP.t);
			break;
		}
		case ASTID_NODE_TYPE:{
			if(typeFreeing) freeType(node->id.t);
			break;
		}
		case ASTValue_NODE_TYPE:{
			if(typeFreeing) freeType(node->value.t);
			break;
		}
		case ASTExpression_NODE_TYPE:{
			freeASTNode(node->expr.expr, typeFreeing);
			free(node->expr.expr);
			if(typeFreeing) freeType(node->expr.t);
			break;
		}
		case ASTPrint_NODE_TYPE:{
			freeASTNode(node->print.expr, typeFreeing);
			free(node->print.expr);
			if(typeFreeing) freeType(node->print.t);
			break;
		}
		case ASTBlock_NODE_TYPE:{
			for(long i = 0; i < node->block.numberOfNodes; i++)	freeASTNode(&node->block.nodes[i], typeFreeing);
			if(typeFreeing) freeType(node->block.t);
			break;
		}
		case ASTVariable_NODE_TYPE:{
			freeASTNode(node->var.expr, typeFreeing);
			free(node->var.expr);
			if(typeFreeing) freeType(node->var.t);
			break;
		}
		case ASTAssignment_NODE_TYPE:{
			freeASTNode(node->ass.expr, typeFreeing);
			free(node->ass.expr);
			if(typeFreeing) freeType(node->ass.t);
			break;			
		}
		case ASTForLoop_NODE_TYPE:{
			if(node->loop.n1 != NULL) {
				freeASTNode(node->loop.n1, typeFreeing);
				free(node->loop.n1);
			}
			if(node->loop.n2 != NULL){
				freeASTNode(node->loop.n2, typeFreeing);
				free(node->loop.n2);
			}
			if(node->loop.n3 != NULL) {
				freeASTNode(node->loop.n3, typeFreeing);
				free(node->loop.n3);
			}
			if(node->loop.max != NULL) {
				freeASTNode(node->loop.min, typeFreeing);
				free(node->loop.min);
			}
			if(node->loop.max != NULL) {
				freeASTNode(node->loop.max, typeFreeing);
			}
			freeASTNode(node->loop.b, typeFreeing);
			if(typeFreeing) freeType(node->loop.t);
			break;
		}
		case ASTIf_NODE_TYPE:{
			freeASTNode(node->ifS.expr, typeFreeing);
			free(node->ifS.expr);
			freeASTNode(node->ifS.s, typeFreeing);
			free(node->ifS.s);
			if(node->ifS.elseS != NULL){
				freeASTNode(node->ifS.elseS, typeFreeing);
				free(node->ifS.elseS);
			}
			if(typeFreeing) freeType(node->ifS.t);
			break;
		}
		case ASTElse_NODE_TYPE:{
			freeASTNode(node->elseS.s, typeFreeing);
			free(node->elseS.s);
			if(typeFreeing) freeType(node->elseS.t);
			break;
		}
		case ASTLoop_NODE_TYPE:{
			freeASTNode(node->simpleLoop.expr, typeFreeing);
			free(node->simpleLoop.expr);
			freeASTNode(node->simpleLoop.block, typeFreeing);
			free(node->simpleLoop.block);
			if(typeFreeing) freeType(node->simpleLoop.t);
			break;
		}
		default : {
			printf("Cannot free node of given type"); 
			printf("%d", type);
			exit(1);
		}
	}
	//free(node);
}

void freeAST(AST* ast, bool freeType){
	for(long i = 0; i < ast->numberOfNodes; i++) freeASTNode(&ast->nodes[i], freeType);
	free(ast);
}

ASTNode* newASTNode(){
	return (ASTNode*)malloc(sizeof(ASTNode));
}

ASTNode* newASTBinaryOP(ASTNode* lhs, char* op, ASTNode* rhs, long line, Type* t){
	ASTNode* n = newASTNode();
	n->type = ASTBinaryOP_NODE_TYPE;
	n->binaryOP.lhs = lhs;
	n->binaryOP.op = op;
	n->binaryOP.rhs = rhs;
	n->binaryOP.line = line;
	n->binaryOP.t = t;
	return n;
}

ASTNode* newASTUnaryOP(ASTNode* opperand, char* op, long line, Type* t){
	ASTNode* n = newASTNode();
	n->type = ASTUnaryOP_NODE_TYPE;
	n->unaryOP.opperand = opperand;
	n->unaryOP.op = op;
	n->unaryOP.line = line;
	n->unaryOP.t = t;
	return n;
}

ASTNode* newASTValue(Value v, long line, Type* t){
	ASTNode* n = newASTNode();
	n->type = ASTValue_NODE_TYPE;
	n->value.v = v;
	n->value.line = line;
	n->value.t = t;
	return n;
}

ASTNode* newASTExpression(ASTNode* expr, bool statement, long line, Type* t){
	ASTNode* n = newASTNode();
	n->type = ASTExpression_NODE_TYPE;
	n->expr.expr = expr;
	n->expr.statement = statement;
	n->expr.line = line;
	n->expr.t = t;
	return n;
}


ASTNode* newASTID(char* id, long line, Type* t){
	ASTNode* n = newASTNode();
	n->type = ASTID_NODE_TYPE;
	n->id.id = id;
	n->id.line = line;
	n->id.t = t;
	return n;
}

ASTNode* newASTAssignment(char* id, ASTNode* expr, long line, Type* t){
	ASTNode* n = newASTNode();
	n->type = ASTAssignment_NODE_TYPE;
	n->ass.id = id;
	n->ass.expr = expr;
	n->ass.line = line;
	n->ass.t = t;
	return n;
}

ASTNode* newASTVariable(char* id, ASTNode* expr, Type* type, long line, Type* t){
	ASTNode* n = newASTNode();
	n->type = ASTVariable_NODE_TYPE;
	n->var.id = id;
	n->var.expr = expr;
	n->var.type = type;
	n->var.line = line;
	n->var.t = t;
	return n;
}

ASTNode* newASTPrint(ASTNode* expr, long line, Type* t){
	ASTNode* n = newASTNode();
	n->type = ASTPrint_NODE_TYPE;
	n->print.expr = expr;
	n->print.line = line;
	n->print.t = t;
	return n;
}

ASTNode* newASTBlock(long line, Type* t){
	ASTNode* n = newASTNode();
	n->type = ASTBlock_NODE_TYPE;
	n->block.nodes = (ASTNode*)malloc(sizeof(ASTNode));
	n->block.cappacity = 1;
	n->block.numberOfNodes = 0;
	n->block.variableCount = 0;
	n->block.line = line;
	n->block.t = t;
	return n;
}

ASTNode* newASTString(char* str, long line, Type* t){
	ASTNode* n = newASTNode();
	n->type = ASTString_NODE_TYPE;
	n->str.str = str;
	n->str.line = line;
	n->str.t = t;
	return n;
}

ASTNode* newASTForLoop(long line, Type* t){
	ASTNode* n = newASTNode();
	n->type = ASTForLoop_NODE_TYPE;
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

ASTNode* newASTSimpleLoop(ASTNode* expr, ASTNode* block, long line, Type* t){
	ASTNode* n = newASTNode();
	n->type = ASTLoop_NODE_TYPE;
	n->simpleLoop.expr = expr;
	n->simpleLoop.block = block;
	n->simpleLoop.line = line;
	n->simpleLoop.t = t;
	return n;
}

ASTNode* newASTElse(ASTNode* s, long line, Type* t){
	ASTNode* n = newASTNode();
	n->type = ASTElse_NODE_TYPE;
	n->elseS.s = s;
	n->elseS.line = line;
	n->elseS.t = t;
	return n;
}

ASTNode* newASTIf(ASTNode* expr, ASTNode* s, ASTNode* elseS, long line, Type* t){
	ASTNode* n = newASTNode();
	n->type = ASTIf_NODE_TYPE;
	n->ifS.expr = expr;
	n->ifS.s = s;
	n->ifS.elseS = elseS;
	n->ifS.line = line;
	n->ifS.t = t;
	return n;
}

ASTNode* newASTTable(TableType type, int line, Type* t){
	ASTNode* n = newASTNode();
	n->type = ASTTable_NODE_TYPE;
	n->table.nodes = (ASTNode*)malloc(sizeof(ASTNode));
	n->table.keys = (char**) malloc(sizeof(char*));
	n->table.type = type;
	n->table.cappacity = 1;
	n->table.numberOfNodes = 0;
	n->table.line = line;
	n->table.t = t;
	return n;
}

ASTNode* resizeNodes(ASTNode* nodes, long cappacity){
	ASTNode* newNodes = (ASTNode*)realloc(nodes, 2*cappacity*sizeof(ASTNode));
	if(newNodes == NULL){
		printf("could not resize AST-Nodes!\n");
		free(nodes);
		exit(1);
	}
	return newNodes;
}

void emitNode(ASTNode* node, AST* ast){
	ast->nodes[ast->numberOfNodes] = *node;
	ast->numberOfNodes++;
	if(ast->numberOfNodes == ast->cappacity){
		ast->nodes = resizeNodes(ast->nodes, ast->cappacity);
		ast->cappacity *= 2;
	}
}

void emitNodeToBlock(ASTNode* node, ASTNode* b){
	b->block.nodes[b->block.numberOfNodes] = *node;
	b->block.numberOfNodes++;
	if(node != NULL && node->type == ASTVariable_NODE_TYPE) b->block.variableCount++;
	if(b->block.numberOfNodes == b->block.cappacity){
		b->block.nodes = resizeNodes(b->block.nodes, b->block.cappacity);
		b->block.cappacity *= 2;
	}
}

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

void printOP(OPCode op){
	switch(op){
		case PLUS_OP:{
			printf("+");
			break;
		}
		case SUB_OP:{
			printf("-");
			break;
		}
		case MULT_OP:{
			printf("*");
			break;
		}
		case DIV_OP:{
			printf("/");
			break;
		}
		case REM_OP:{
			printf("%c", '%');
			break;
		}
		case EXP_OP:{
			printf("^");
			break;
		}
		case FACT_OP:{
			printf("!");
			break;
		}
		case AND_OP:{
			printf("and");
			break;
		}
		case OR_OP:{
			printf("or");
			break;
		}
		case NOT_OP:{
			printf("not");
			break;
		}
		case UNARY_PLUS_OP:{
			printf("+");
			break;
		}
		case UNARY_MINUS_OP:{
			printf("-");
			break;
		}
		case EQUAL_OP:{
			printf("==");
			break;
		}
		case LESS_OP:{
			printf("<");
			break;
		}
		case GREATER_OP:{
			printf(">");
			break;
		}
		case LOE_OP:{
			printf("<=");
			break;
		}
		case GOE_OP:{
			printf("==");
			break;
		}

	}
}

void printASTNode(AST* ast, ASTNode* node){
	ASTNodeType type = node->type;
	switch(type){
		case ASTBinaryOP_NODE_TYPE :{
			printf("(");
			printASTNode(ast, node->binaryOP.lhs);
			printf("%s", node->binaryOP.op);
			printASTNode(ast, node->binaryOP.rhs);
			printf(")");
			break;
		}
		case ASTUnaryOP_NODE_TYPE : {
			printf("(");
			if(node->unaryOP.type == POSTFIX_OP){
				printASTNode(ast, node->unaryOP.opperand);
				printf("%s", node->unaryOP.op);
			}
			else{
				printf("%s", node->unaryOP.op);
				printASTNode(ast, node->unaryOP.opperand);
			}
			printf(")");
			break;
		}
		case ASTString_NODE_TYPE : {
			printf("String[");
			printf("%s", node->str.str);
			printf("]");
			break;
		}
		case ASTCallOP_NODE_TYPE:{
			printASTNode(ast, node->callOP.opperand);
			printf("()");
			break;
		}
		case ASTID_NODE_TYPE:{
			printf("ID[%s]", node->id.id);
			break;
		}
		case ASTValue_NODE_TYPE:{
			switch(node->value.v.type){
				case I32_VAL :{
					printf("%d", node->value.v.i32);
					break;
				}
				case F32_VAL:{
					printf("%f", node->value.v.f32);
					break;
				}
				case BOOL_VAL:{
					if(node->value.v.boolean){
						printf("true");
					}
					else{ 
						printf("false");
					}
					break;
				}
				case STR_VAL:{
					//printf("%s", ast->strings[node->value.v.i32]);
					break;
				}
				default : {
					printf("%d\n", node->type);
					printf("Cannot print value of given type 1");
					exit(1);
				}
			}
			break;
		}
		case ASTExpression_NODE_TYPE:{
			printf("Expression[");
			printASTNode(ast, node->expr.expr);
			printf("]");
			break;
		}
		case ASTPrint_NODE_TYPE:{
			printf("Print[");
			printASTNode(ast, node->print.expr);
			printf("]");
			break;
		}
		case ASTVariable_NODE_TYPE:{
			printf("Variable[");
			printf("Id:%s, Expression:", node->var.id);
			printASTNode(ast, node->var.expr);
			printf("]");
			break;
		}
		case ASTAssignment_NODE_TYPE:{
			printf("Assignment[");
			printf("Id:%s, Expression:", node->ass.id);
			printASTNode(ast, node->ass.expr);
			printf("]");
			break;
		}
		case ASTBlock_NODE_TYPE:{
			printf("Block[\n");
			for(int i = 0; i < node->block.numberOfNodes; i++) {
				printASTNode(ast, &node->block.nodes[i]);
				printf("\n");
			}
			printf("]\n");
			break;
		}
		case ASTForLoop_NODE_TYPE:{
			printf("for ");
			if(node->loop.n1 != NULL){
				printf("NODE1=[");
				printASTNode(ast, node->loop.n1);
				printf("]");
			}
			if(node->loop.n2 != NULL) {
				if(node->loop.n3 == NULL){
					/*// work this out...
					printf(" in ");
					printf("COLLECTION=[");
					printASTNode(ast, node->loop.n2);
					printf("] ");*/
				}
				else{
					printf("NODE2=[");
					printASTNode(ast, node->loop.n2);
					printf("] ");
					if(node->loop.n3 != NULL){
						printf("NODE3=[");
						printASTNode(ast, node->loop.n3);
						printf("]");
					}
				}
			}
			printf("[");
			if(node->loop.min != NULL){
				printf("MIN=[");
				printASTNode(ast, node->loop.min);
				printf("]");
			}
			printf(", ");
			if(node->loop.max != NULL) {
				printf("MAX=[");
				printASTNode(ast, node->loop.max);
				printf("]");
			}
			printf("]");
			printf("{\n");
			printASTNode(ast, node->loop.b);
			printf("}");
			break;
		}
		case ASTLoop_NODE_TYPE:{
			printf("for ");
			printASTNode(ast, node->simpleLoop.expr);
			printf("{\n");
			printASTNode(ast, node->simpleLoop.block);
			printf("}");
			break;
		}
		case ASTIf_NODE_TYPE:{
			printf("if ");
			printASTNode(ast, node->ifS.expr);
			printASTNode(ast, node->ifS.s);
			printf("\n");
			if(node->ifS.elseS != NULL) {
				printf("( "); printASTNode(ast, node->ifS.elseS); printf(" )");
			}
			break;
		}
		case ASTElse_NODE_TYPE:{
			printf("else ");
			printASTNode(ast, node->elseS.s);
			break;
		}
		case ASTTable_NODE_TYPE:{
			printf("Table-%d[", node->table.type);
			for(int i = 0; i < node->table.numberOfNodes; i++) {
				printASTNode(ast, &node->table.nodes[i]);
				printf(",");
			}
			printf("]");
			break;
		}
		default : {
			printf("Cannot print node of given type\n"); 
			printf("%d",type);
			exit(1);
		}
	}
}

void printAST(AST* ast){
	printf("AST:\n");
	for(long i = 0; i < ast->numberOfNodes; i++){
		printASTNode(ast, &ast->nodes[i]); printf("\n");
	}
	printf("\n");
}
