#include "Interpreting.h"

#define ARITHMETIC_BINARY_OP(op) \
	--vm->stackPtr; \
	Value v = *vm->stackPtr; \
	--vm->stackPtr; \
	if (v.type == I32_VAL && vm->stackPtr->type == I32_VAL) { \
		vm->stackPtr->type = I32_VAL; \
		vm->stackPtr->i32 = vm->stackPtr->i32 op v.i32; \
	} \
	else if (v.type == I32_VAL && vm->stackPtr->type == F32_VAL) { \
		vm->stackPtr->type = F32_VAL; \
		vm->stackPtr->f32 = vm->stackPtr->f32 op v.i32; \
	} \
	else if (v.type == F32_VAL && vm->stackPtr->type == I32_VAL) { \
		vm->stackPtr->type = F32_VAL; \
		vm->stackPtr->f32 = vm->stackPtr->i32 op v.f32; \
	} \
	else if (v.type == F32_VAL && vm->stackPtr->type == F32_VAL) { \
		vm->stackPtr->type = F32_VAL; \
		vm->stackPtr->f32 = vm->stackPtr->f32 op v.f32; \
	} \
	else { \
		printf("Integer, Double Binary Op Error"); \
		exit(1); \
	} \
	++vm->stackPtr; \


#define EXP_OP() \
	--vm->stackPtr; \
	Value v = *vm->stackPtr; \
	--vm->stackPtr; \
	if (v.type == I32_VAL && vm->stackPtr->type == I32_VAL) { \
		vm->stackPtr->type = I32_VAL; \
		vm->stackPtr->i32 = pow(vm->stackPtr->i32, v.i32); \
	} \
	else if (v.type == I32_VAL && vm->stackPtr->type == F32_VAL) { \
		vm->stackPtr->type = F32_VAL; \
		vm->stackPtr->f32 = pow(vm->stackPtr->f32, v.i32); \
	} \
	else if (v.type == F32_VAL && vm->stackPtr->type == I32_VAL) { \
		vm->stackPtr->type = F32_VAL; \
		vm->stackPtr->f32 = pow(vm->stackPtr->i32, v.f32); \
	} \
	else if (v.type == F32_VAL && vm->stackPtr->type == F32_VAL) { \
		vm->stackPtr->type = F32_VAL; \
		vm->stackPtr->f32 = pow(vm->stackPtr->f32, v.f32); \
	} \
	else { \
		printf("Exp Op Error"); \
		exit(1); \
	} \
	++vm->stackPtr; \

#define REM_OP() \
	--vm->stackPtr; \
	Value v = *vm->stackPtr; \
	--vm->stackPtr; \
	if (v.type == I32_VAL && vm->stackPtr->type == I32_VAL) { \
		vm->stackPtr->type = I32_VAL; \
		vm->stackPtr->i32 = vm->stackPtr->i32 % v.i32; \
	} \
	else { \
		printf("REM Op Error"); \
		exit(1); \
	} \
	++vm->stackPtr; \

#define FACT_OP() \
	--vm->stackPtr; \
	if (vm->stackPtr->type == I32_VAL) { \
		vm->stackPtr->type = I32_VAL; \
		vm->stackPtr->i32 = intFact(vm->stackPtr->i32); \
	} \
	else { \
		printf("Fact Op Error"); \
		exit(1); \
	} \
	++vm->stackPtr; \

#define BOOLEAN_ARITHMETIC_OP(op) \
	--vm->stackPtr; \
	Value v = *vm->stackPtr; \
	--vm->stackPtr; \
	if (v.type == BOOL_VAL && vm->stackPtr->type == BOOL_VAL) { \
		vm->stackPtr->type = BOOL_VAL; \
		vm->stackPtr->boolean = vm->stackPtr->boolean op v.boolean; \
	} \
	else { \
		printf("and or or Op Error"); \
		exit(1); \
	} \
	++vm->stackPtr; \

#define NOT_OP() \
	--vm->stackPtr; \
	if (vm->stackPtr->type == BOOL_VAL) { \
		vm->stackPtr->boolean = !vm->stackPtr->boolean; \
	} \
	else { \
		printf("not Op Error"); \
		exit(1); \
	} \
	++vm->stackPtr; \

#define UNARY_PLUS_OP() \
	--vm->stackPtr; \
	if (vm->stackPtr->type == I32_VAL) { \
		vm->stackPtr->type = I32_VAL; \
		vm->stackPtr->i32 = abs(vm->stackPtr->i32); \
	} \
	else if (vm->stackPtr->type == F32_VAL) { \
		vm->stackPtr->type = F32_VAL; \
		vm->stackPtr->f32 = abs(vm->stackPtr->f32); \
	} \
	else { \
		printf("not Op Error"); \
		exit(1); \
	} \
	++vm->stackPtr; \

#define UNARY_MINUS_OP() \
	--vm->stackPtr; \
	if (vm->stackPtr->type == I32_VAL) { \
		vm->stackPtr->type = I32_VAL; \
		vm->stackPtr->i32 = -vm->stackPtr->i32; \
	} \
	else if (vm->stackPtr->type == F32_VAL) { \
		vm->stackPtr->type = F32_VAL; \
		vm->stackPtr->f32 = -vm->stackPtr->f32; \
	} \
	else { \
		printf("not Op Error"); \
		exit(1); \
	} \
	++vm->stackPtr; \

#define COMPARITIVE_BINARY_OP(op) \
	--vm->stackPtr; \
	Value v = *vm->stackPtr; \
	--vm->stackPtr; \
	if (v.type == I32_VAL && vm->stackPtr->type == I32_VAL) { \
		vm->stackPtr->type = BOOL_VAL; \
		vm->stackPtr->boolean = vm->stackPtr->i32 op v.i32; \
	} \
	else if (v.type == I32_VAL && vm->stackPtr->type == F32_VAL) { \
		vm->stackPtr->type = BOOL_VAL; \
		vm->stackPtr->boolean = vm->stackPtr->f32 op v.i32; \
	} \
	else if (v.type == F32_VAL && vm->stackPtr->type == I32_VAL) { \
		vm->stackPtr->type = BOOL_VAL; \
		vm->stackPtr->boolean = vm->stackPtr->i32 op v.f32; \
	} \
	else if (v.type == F32_VAL && vm->stackPtr->type == F32_VAL) { \
		vm->stackPtr->type = BOOL_VAL; \
		vm->stackPtr->boolean = vm->stackPtr->f32 op v.f32; \
	} \
	else { \
		printf("Comparitive Binary Op Error"); \
		exit(1); \
	} \
	++vm->stackPtr; \

#define PUSH_VAL() \
	*vm->stackPtr = vm->p->values->values[*vm->ip];\
	++vm->stackPtr; \

int intFact(int n){
	if(n == 0) return 1;
	return n * intFact(n-1);
}

VM* initVM(Program* p){
	VM* vm = (VM*)malloc(sizeof(VM));
	vm->p = p;
	vm->ip = p->ops->ops;
	vm->stackPtr = vm->stack;
	vm->globalVariables = (Value*)malloc(sizeof(Value)*p->globalCount);
	vm->strings = p->strings;
	return vm;
}

void printValue(VM* vm, Value* v){
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
	else if(v->type == STR_VAL){
		printf("%s", vm->strings[v->i32]);
	}
	else{
		printf("\n%d\n", v->type);
		printf("Can't print value of given type");
		exit(1);
	}
}

void dumpStack(VM* vm){
	Value* ptr = vm->stack;
	while(ptr != vm->stackPtr){
		printValue(vm, ptr);
		printf("\n");
		ptr++;
	}
}	

void execute(VM* vm){
	int i = 0;
	for(;;){
		switch(*vm->ip){
			case PLUS_OP: {
				ARITHMETIC_BINARY_OP(+);
				vm->ip++;
				break;
			}
			case SUB_OP: {
				ARITHMETIC_BINARY_OP(-);
				vm->ip++;
				break;
			}
			case MULT_OP: {
				ARITHMETIC_BINARY_OP(*);
				vm->ip++;
				break;
			}
			case DIV_OP: {
				ARITHMETIC_BINARY_OP(/);
				vm->ip++;
				break;
			}
			case REM_OP: {
				REM_OP();
				vm->ip++;
				break;
			}
			case EXP_OP: {
				EXP_OP();
				vm->ip++;
				break;
			}
			case FACT_OP: {
				FACT_OP();
				vm->ip++;
				break;
			}
			case AND_OP: {
				BOOLEAN_ARITHMETIC_OP(&&);
				vm->ip++;
				break;
			}
			case OR_OP: {
				BOOLEAN_ARITHMETIC_OP(||);
				vm->ip++;
				break;
			}
			case NOT_OP: {
				NOT_OP();
				vm->ip++;
				break;
			}
			case UNARY_PLUS_OP: {
				UNARY_PLUS_OP();
				vm->ip++;
				break;
			}
			case UNARY_MINUS_OP: {
				UNARY_MINUS_OP();
				vm->ip++;
				break;
			}
			case EQUAL_OP: {
				COMPARITIVE_BINARY_OP(==);
				vm->ip++;
				break;
			}
			case LESS_OP: {	
				COMPARITIVE_BINARY_OP(<);
				vm->ip++;
				break;
			}
			case GREATER_OP: {
				COMPARITIVE_BINARY_OP(>);
				vm->ip++;
				break;
			}
			case LOE_OP: {
				COMPARITIVE_BINARY_OP(<=);
				vm->ip++;
				break;
			}
			case GOE_OP: {
				COMPARITIVE_BINARY_OP(>=);
				vm->ip++;
				break;
			}
			case CALL_OP: {
				//later
				break;
			}
			case VAL_OP: {
				vm->ip++;
				PUSH_VAL();
				vm->ip+=1;
				break;
			}
			case HALT_OP: {
				--vm->stackPtr;
				return;
			}
			case PRINT_OP:{
				printValue(vm, vm->stackPtr-1);
				printf("\n");
				vm->ip+=1;
				break;
			}
			case POP_OP:{
				--vm->stackPtr;
				vm->ip+=1;
				break;
			}
			case SET_GLOBAL_VAR_OP:{
				vm->ip++;
				vm->stackPtr--;
				vm->globalVariables[*vm->ip++] = *vm->stackPtr;
				//++vm->stackPtr;
				break;
			}
			case GET_GLOBAL_VAR_OP:{
				vm->ip++;
				*vm->stackPtr = vm->globalVariables[*vm->ip++];
				++vm->stackPtr;
				break;
			}
			case SET_LOCAL_VAR_OP:{
				vm->ip++;
				vm->stackPtr--;
				vm->stack[*vm->ip++] = *vm->stackPtr;
				//++vm->stackPtr;				
				break;
			}
			case GET_LOCAL_VAR_OP:{		
				vm->ip++;
				*vm->stackPtr = vm->stack[*vm->ip++];
				++vm->stackPtr;				
				break;
			}
			case STR_VAL_OP:{
				vm->ip++;
				vm->stackPtr->i32 = *vm->ip++;
				vm->stackPtr->type = STR_VAL;
				++vm->stackPtr;
				break;
			}
			case JMP_BACK_OP:{
				i = 1;
				vm->ip++;
				vm->ip -= *vm->ip; //vm->p->ops->ops[*vm->ip];
				break;
			}
			case JMP_ON_FALSE_OP:{
				vm->ip++;
				vm->stackPtr--;
				if (!vm->stackPtr->boolean) vm->ip += *vm->ip;
				else vm->ip++;	
				break;
			}
			default: {
				break;
			}
		}
	}
}
//Might be an error with incrementing and decrementing stack ptr after getting and setting variables

//Fix compiler and interpreter, organize files
//fix strings, and variables