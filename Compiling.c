#include "Compiling.h"

Value* resizeValues(Value* values, long size){
	Value* newValues = (Value*)realloc(values, 2*size*sizeof(Value));
	if(newValues == NULL){
		printf("could not resize values!, %d", 2*size);
		free(values);
		free(newValues);
		exit(1);
	}
	return newValues;
}

OPCode* resizeOPs(OPCode* ops, long size){
	OPCode* newOps = (OPCode*)realloc(ops, 2*size*sizeof(OPCode));
	if(newOps == NULL){
		printf("could not resize ops!, %d", 2*size);
		free(ops);
		free(newOps);
		exit(1);
	}
	return newOps;
}

void emitOP(OPArray* ops, OPCode op){
	ops->ops[ops->opCount] = op;
	ops->opCount++;
	if(ops->opCount == ops->cappacity){
		ops->ops = resizeOPs(ops->ops, ops->cappacity);
		ops->cappacity *= 2;
	}
}

void emitValue(ValueArray* values, Value v){
	//printf("\n B-Type: %d\n", v.type);
	values->values[values->valueCount] = v;
	values->valueCount++;
	if(values->valueCount == values->cappacity){
		values->values = resizeValues(values->values, values->cappacity);
		values->cappacity *= 2;
	}
}

Program initProgram(){
	OPCode* ops = (OPCode*)malloc(sizeof(OPCode));
	OPArray o = {ops, 1, 0};
	Value* values = (Value*)malloc(sizeof(Value));
	ValueArray v = {values, 1, 0};
	Program p = {v, o};
	return p;
}

void compileASTCallNode(ASTCallOP* node, Program* p){
	//fill in later
}

void compileASTIDNode(ASTID* node, Program* p){
	//fill in later
}

void compileASTBinaryNode(ASTBinaryOP* node, Program* p){
	compileASTNode(node->lhs, p);
	compileASTNode(node->rhs, p);
	emitOP(&p->ops, node->op);
}

void compileASTUnaryNode(ASTUnaryOP* node, Program* p){
	compileASTNode(node->opperand, p);
	emitOP(&p->ops, node->op);
}

void compileASTValueNode(ASTValue* value, Program* p){
	//printf("\n A-Type: %d\n", value->v.type);
	//exit(1);
	emitValue(&p->values, value->v);
	emitOP(&p->ops, VAL_OP);
	//turn the int into 4 bytes and emit them
	emitOP(&p->ops, p->values.valueCount-1);
}

void compileASTExpressionNode(ASTExpression* expr, Program* p){
	compileASTNode(expr->expr, p);
	emitOP(&p->ops, POP_OP);
}

void compileASTPrint(ASTPrint* print, Program* p){
	compileASTNode(print->expr, p);
	emitOP(&p->ops, PRINT_OP);
	emitOP(&p->ops, POP_OP);
}

void compileASTNode(ASTNode* node, Program* p){
	switch(node->type){
		case ASTPrint_NODE_TYPE:{
			compileASTPrint(&node->print, p);
			break;
		}
		case ASTExpression_NODE_TYPE:{
			compileASTExpressionNode(&node->expr, p);
			break;
		}
		case ASTBinaryOP_NODE_TYPE :{
			compileASTBinaryNode(&node->binaryOP, p);
			break;
		}
		case ASTUnaryOP_NODE_TYPE : {
			compileASTUnaryNode(&node->unaryOP, p);
			break;
		}
		case ASTCallOP_NODE_TYPE:{
			compileASTCallNode(&node->callOP, p);
			break;
		}
		case ASTID_NODE_TYPE:{
			compileASTIDNode(&node->id, p);
			break;
		}
		case ASTValue_NODE_TYPE:{
			compileASTValueNode(&node->value, p);
			break;
		}
		default : {
			printf("Cannot compile node of given type"); 
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
	for(int i = 0; i < p->ops.opCount; i++){
		switch (p->ops.ops[i]) {
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
				printf("%");
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
			case ID_OP: {
				printf("ID[");
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
			case VAL_OP:{
				i += 1;
				if(p->values.values[p->ops.ops[i]].type == I32_VAL){
					printf("val:i32[");
					printf("%d", p->values.values[p->ops.ops[i]].i32);
					printf(" at: " );
					printf("%d", p->ops.ops[i]);
					printf("]");
				}
				else if(p->values.values[p->ops.ops[i]].type == F32_VAL){
					printf("val:f32[");
					printf("%f", p->values.values[p->ops.ops[i]].f32);
					printf(" at: " );
					printf("%d", p->ops.ops[i]);
					printf("]");
				}
				else if(p->values.values[p->ops.ops[i]].type == BOOL_VAL){
					printf("val:bool[");
					if(p->values.values[p->ops.ops[i]].boolean){
						printf("true");
					}
					else{ 
						printf("false");
					}
					printf(" at: " );
					printf("%d", p->ops.ops[i]);
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
			default: {
				printf("");
				break;
			}
		}
		printf(", ");
	}
	printf("\n\n");
}

void printBytes(Program* p){
	printf("ops: ");
	for(int i = 0; i < p->ops.opCount; i++){
		printf("%d, ", p->ops.ops[i]);
	}
}

void printProgram(Program* p){
	printOPS(p);
	//printValues(&p->values);
	//printBytes(p);
}

void freeOPs(OPArray* arr){
	free(arr->ops);
	free(arr);
}

void freeValues(ValueArray* arr){
	free(arr->values);
	free(arr);
}

void freeProgram(Program* p){
	freeOPs(&p->ops);
	freeValues(&p->values);
	free(p);
}


//->ops
//p
Program compile(AST* ast){
	Program p = initProgram();
	for(int i = 0; i < ast->numberOfNodes; i++){
		compileASTNode(&ast->nodes[i], &p);
	}
	emitOP(&p.ops, HALT_OP);
	return p;
}