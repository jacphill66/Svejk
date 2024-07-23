#include "Compiling.h"

Value* resizeValues(Value* values, long size){
	Value* newValues = (Value*)realloc(values, 2*size*sizeof(Value));
	if(newValues == NULL){
		printf("could not resize values!, %ld", 2*size);
		free(values);
		free(newValues);
		exit(1);
	}
	return newValues;
}//`

OPCode* resizeOPs(OPCode* ops, long size){
	OPCode* newOps = (OPCode*)realloc(ops, 2*size*sizeof(OPCode));
	if(newOps == NULL){
		printf("could not resize ops!, %ld", 2*size);
		free(ops);
		free(newOps);
		exit(1);
	}
	return newOps;
}

void emitOP(Compiler* c, OPCode op){
	OPArray* ops = c->prog->ops;
	ops->ops[ops->opCount] = op;
	ops->opCount++;
	c->opPos++;
	if(ops->opCount == ops->cappacity){
		ops->ops = resizeOPs(ops->ops, ops->cappacity);
		ops->cappacity *= 2;
	}
}

void emitValue(Compiler* c, Value v){
	ValueArray* values = c->prog->values;
	//printf("\n B-Type: %d\n", v.type);
	values->values[values->valueCount] = v;
	values->valueCount++;
	if(values->valueCount == values->cappacity){
		values->values = resizeValues(values->values, values->cappacity);
		values->cappacity *= 2;
	}
}

Program* newProgram(int stringCount){
	//refactor this mess
	Program* p = (Program*)malloc(sizeof(Program));
	OPCode* ops = (OPCode*)malloc(sizeof(OPCode));
	OPArray* o = malloc(sizeof(OPArray));
	o->ops = ops;
	o->cappacity = 1;
    o->opCount = 0;
	Value* values = (Value*)malloc(sizeof(Value));
	ValueArray* v = (ValueArray*)malloc(sizeof(ValueArray));
	v->values = values;
	v->cappacity = 1;
	v->valueCount = 0;
	//Program p = {v, o};
	p->globalCount = 0;
	p->values = v;
	p->ops = o;
	p->strings = (char**)malloc(sizeof(char*)*stringCount);
	return p;
}

Compiler* newCompiler(AST* ast, Analysis* a){
	Compiler* c = (Compiler*)malloc(sizeof(Compiler));
	c->prog = newProgram(a->stringCount);
	c->ast = ast;
	c->scopes = newScopeChain();//redo local indexing because of rewriting
	newScope(c->scopes);
	c->scopeDepth = 0;
	c->opPos = 0;
	c->analysis = a;
	return c;
}

void compileASTCallNode(ASTCallOP* node, Program* p){
	//fill in later
}

void compileASTIDNode(Program* p){
	//fill in later
}

void compileASTBinaryNode(Compiler* c, ASTBinaryOP* node){
	compileASTNode(c, node->lhs);
	compileASTNode(c, node->rhs);
	emitOP(c, node->op);
}

void compileASTUnaryNode(Compiler* c, ASTUnaryOP* node){
	compileASTNode(c, node->opperand);
	emitOP(c, node->op);
}

void compileASTValueNode(Compiler* c, ASTValue* value){
	//printf("\n A-Type: %d\n", value->v.type);
	//exit(1);
	emitValue(c, value->v);
	emitOP(c, VAL_OP);
	//turn the int into 4 bytes and emit them
	emitOP(c, c->prog->values->valueCount-1);
}

void compileASTExpressionNode(Compiler* c, ASTExpression* expr){
	compileASTNode(c, expr->expr);
	emitOP(c, POP_OP);
}

void compileASTPrint(Compiler* c, ASTPrint* print){
	compileASTNode(c, print->expr);
	emitOP(c, PRINT_OP);
	emitOP(c, POP_OP);
}
void compileASTVariable(Compiler* c, ASTVariable* var){
	compileASTNode(c, var->expr);
	addToCurrentScope(c->scopes, var->id, -1, -1);
	if(c->scopeDepth < 1) {
		emitOP(c, SET_GLOBAL_VAR_OP);
		emitOP(c, c->prog->globalCount++);
	}
}

void compileASTReference(Compiler* c, ASTID* id){
	int i = searchScopes(c->scopes, id->id);
	if(i < 0) {
		printf("Compiler Error!");
		exit(1);
	}
	if(searchScope(c->scopes->head, id->id) != -1) emitOP(c, GET_GLOBAL_VAR_OP);
	else emitOP(c, GET_LOCAL_VAR_OP);
	emitOP(c, i);
}

void compileASTAssignment(Compiler* c, ASTAssignment* ass){
	int i = searchScopes(c->scopes, ass->id);
	if(i < 0) {
		printf("Compiler Error");
		exit(1);
	}
	compileASTNode(c, ass->expr);
	if(searchScope(c->scopes->head, ass->id) != -1) emitOP(c, SET_GLOBAL_VAR_OP);
	else emitOP(c, SET_LOCAL_VAR_OP); 
	emitOP(c, i);
}

void compileASTBlock(Compiler* c, ASTBlock* block){
	c->scopeDepth++;
	newScope(c->scopes);
	/*printf("%d\n", c->scopes->tail->offset);
	exit(1);
	printf("%d\n", c->scopes->tail->offset);
	exit(1);*/
	for(int i = 0; i < block->numberOfNodes; i++) compileASTNode(c, &block->nodes[i]);
	for(int i = 0; i < block->variableCount; i++) emitOP(c, POP_OP);
	closeScope(c->scopes);
	c->scopeDepth--;
}


void compileASTString(Compiler* c, ASTString* str){
	emitOP(c, STR_VAL_OP);
	int i = search(c->analysis->strings, str->str).i32;
	emitOP(c, i);
	c->prog->strings[i] = str->str;
}

void compileASTLoop(Compiler* c, ASTLoop* loop){
	int startPos = c->opPos;
	compileASTNode(c, loop->expr);
	emitOP(c, JMP_ON_FALSE_OP);
	emitOP(c, -1);
	int index1 = c->opPos;
	compileASTNode(c, loop->block);
	int endPos = c->opPos;
	//emitOP(c, POP_OP);
	emitOP(c, JMP_BACK_OP);
	emitOP(c, c->opPos-startPos);//c->opPos-startPos
	c->prog->ops->ops[index1-1] = (c->opPos-index1)+1;
	//emitOP(c, POP_OP);
}

void compileASTIf(Compiler* c, ASTIf* ifS){
	compileASTNode(c, ifS->expr);
	emitOP(c, JMP_ON_FALSE_OP);
	emitOP(c, -1);
	int pos1 = c->opPos-1;
	compileASTNode(c, ifS->s);
	emitOP(c, JMP_FORWARD_OP);
	emitOP(c, -1);
	int pos2 = c->opPos-1;
	c->prog->ops->ops[pos1] = (c->opPos - pos1);
	if(ifS->elseS != NULL) compileASTNode(c, ifS->elseS->elseS.s);
	c->prog->ops->ops[pos2] = (c->opPos - pos2);
}

void compileASTNode(Compiler* c, ASTNode* node){
	switch(node->type){
		case ASTPrint_NODE_TYPE:{
			compileASTPrint(c, &node->print);
			break;
		}
		case ASTExpression_NODE_TYPE:{
			compileASTExpressionNode(c, &node->expr);
			break;
		}
		case ASTBinaryOP_NODE_TYPE :{
			compileASTBinaryNode(c, &node->binaryOP);
			break;
		}
		case ASTUnaryOP_NODE_TYPE : {
			compileASTUnaryNode(c, &node->unaryOP);
			break;
		}
		case ASTCallOP_NODE_TYPE:{
			//compileASTCallNode(c, &node->callOP);
			break;
		}
		case ASTValue_NODE_TYPE:{
			compileASTValueNode(c, &node->value);
			break;
		}
		case ASTID_NODE_TYPE:{
			compileASTReference(c, &node->id);
			break;
		}
		case ASTVariable_NODE_TYPE:{
			compileASTVariable(c, &node->var);
			break;
		}
		case ASTAssignment_NODE_TYPE:{
			compileASTAssignment(c, &node->ass);
			break;
		}
		case ASTBlock_NODE_TYPE:{
			compileASTBlock(c, &node->block);
			break;
		}
		case ASTString_NODE_TYPE:{
			compileASTString(c, &node->str);
			break;
		}
		case ASTLoop_NODE_TYPE:{
			compileASTLoop(c, &node->simpleLoop);
			break;
		}
		case ASTIf_NODE_TYPE:{
			compileASTIf(c, &node->ifS);
			break;
		}
		default : {
			printf("Cannot compile node of given type: %d", node->type); 
			exit(1);
		}
	}
}

void printVal(Value* v){
	if(v->type == I32_VAL){
		printf("%d", v->i32);
	}
	else if(v->type == F32_VAL){
		printf("%f", v->f32);
	}
	else if(v->type == BOOL_VAL){
		if(v->boolean){
			printf("true");
		}
		else{ 
			printf("false");
		}
	}
	else{
		printf("Error printing value");
		exit(1);
	}
}

void printValues(ValueArray* vals){
	printf("values: ");
	for(int i = 0; i < vals->valueCount; i++){
		printVal(&vals->values[i]);
		printf(" at: %d, ", i);
	}
}

void printOPS(Program* p){
	printf("Ops: ");
	for(int i = 0; i < p->ops->opCount; i++){
		switch (p->ops->ops[i]) {
			case PLUS_OP: {
				printf("+");
				break;
			}
			case SUB_OP: {
				printf("-");
				break;
			}
			case MULT_OP: {
				printf("*");
				break;
			}
			case DIV_OP: {
				printf("/");
				break;
			}
			case REM_OP: {
				printf("%c", '%');
				break;
			}
			case EXP_OP: {
				printf("^");
				break;
			}
			case FACT_OP: {
				printf("!");
				break;
			}
			case AND_OP: {
				printf("and");
				break;
			}
			case OR_OP: {
				printf("or");
				break;
			}
			case NOT_OP: {
				printf("not");
				break;
			}
			case UNARY_PLUS_OP: {
				printf("U[+] ");
				break;
			}
			case UNARY_MINUS_OP: {
				printf("U[-]");
				break;
			}
			case EQUAL_OP: {
				printf("==");
				break;
			}
			case LESS_OP: {
				printf("<");
				break;
			}
			case GREATER_OP: {
				printf(">");
				break;
			}
			case LOE_OP: {
				printf("<=");
				break;
			}
			case GOE_OP: {
				printf(">=");
				break;
			}
			case CALL_OP: {
				printf("()");
				break;
			}
			case GET_GLOBAL_VAR_OP: {
				i += 1;
				printf("GetGlobal[");
				printf("%d", p->ops->ops[i]);
				printf("]");
				break;
			}
			case SET_GLOBAL_VAR_OP: {
				i += 1;
				printf("SetGlobal[");
				printf("%d", p->ops->ops[i]);
				printf("]");
				break;
			}
			case GET_LOCAL_VAR_OP: {
				i += 1;
				printf("GetLocal[");
				printf("%d", p->ops->ops[i]);
				printf("]");
				break;
			}
			case SET_LOCAL_VAR_OP: {
				i += 1;
				printf("SetLocal[");
				printf("%d", p->ops->ops[i]);
				printf("]");
				break;
			}
			case POP_OP:{
				printf("POP");
				break;
			}
			case PRINT_OP:{
				printf("print");
				break;
			}
			case STR_VAL_OP:{
				i += 1;
				printf("str[");
				printf("%s", p->strings[p->ops->ops[i]]);
				printf("]");
				break;
			}
			case VAL_OP:{
				i += 1;
				if(p->values->values[p->ops->ops[i]].type == I32_VAL){
					printf("val:i32[");
					printf("%d", p->values->values[p->ops->ops[i]].i32);
					printf(" at: " );
					printf("%d", p->ops->ops[i]);
					printf("]");
				}
				else if(p->values->values[p->ops->ops[i]].type == F32_VAL){
					printf("val:f32[");
					printf("%f", p->values->values[p->ops->ops[i]].f32);
					printf(" at: " );
					printf("%d", p->ops->ops[i]);
					printf("]");
				}
				else if(p->values->values[p->ops->ops[i]].type == BOOL_VAL){
					printf("val:bool[");
					if(p->values->values[p->ops->ops[i]].boolean){
						printf("true");
					}
					else{ 
						printf("false");
					}
					printf(" at: " );
					printf("%d", p->ops->ops[i]);
					printf("]");
				}
				else{
					printf("Error printing value");
					exit(1);
				}
				break;
			}
			case HALT_OP: {
				printf("HALT");
				break;
			}
			case JMP_FORWARD_OP:{
				printf("JMP_FORWARD[");
				i += 1;
				printf("%d]", p->ops->ops[i]);
				break;			
			}
			case JMP_BACK_OP:{
				printf("JMP_BACK[");
				i += 1;
				printf("%d]", p->ops->ops[i]);
				break;
			}
			case JMP_ON_FALSE_OP:{
				printf("JMP_ON_FALSE[");
				i += 1;
				printf("%d]", p->ops->ops[i]);
				break;
			}
			case DUMP_OP:{
				printf("dump");
				break;
			}
			default: {
				printf("Can't Print OP\n");
				exit(1);
				break;
			}
		}
		printf(", ");
	}
	printf("\n\n");
}

void printBytes(Program* p){
	printf("ops: ");
	for(int i = 0; i < p->ops->opCount; i++){
		printf("%d, ", p->ops->ops[i]);
	}
}

void printProgram(Program* p){
	printOPS(p);
	//printValues(&p->values);
	//printBytes(p);
}

void freeOPs(OPArray* arr){
	free(arr->ops);
}

void freeValues(ValueArray* arr){
	free(arr->values);
	free(arr);
}

void freeProgram(Program* p){
	freeOPs(p->ops);
	freeValues(p->values);
	free(p);
}

void freeCompiler(Compiler* c){
	closeScope(c->scopes);
	free(c->prog);
	free(c->scopes);
	free(c);
}

Program* compile(Compiler* c, AST* ast){
	for(int i = 0; i < c->ast->numberOfNodes; i++){
		compileASTNode(c, &c->ast->nodes[i]);
	}
	//emitOP(c, DUMP_OP);
	emitOP(c, HALT_OP);
	return c->prog;
}