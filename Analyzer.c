#include "Analyzer.h"

Error* newError(Type* type, char* message, long line){
	Error* error = (Error*) malloc(sizeof(Error));
	error->type = type;
	error->message = message;
	error->line = line;
	return error;
}

Error** resizeErrors(Error** errors, long size){
	Error** newErrors = (Error**)realloc(errors, 2*size*sizeof(Error));
	if(newErrors == NULL){
		printf("could not resize errors!, %ld", 2*size);
		free(errors);
		free(newErrors);
		exit(1);
	}
	return newErrors;
}

void emitError(ErrorArray* errors, Error* error){
	errors->errors[errors->errorCount] = error;
	errors->errorCount++;
	//printf("%s", errors->errors[errors->errorCount-1]->message);
	if(errors->errorCount == errors->cappacity){
		errors->errors = resizeErrors(errors->errors, errors->cappacity);
		errors->cappacity *= 2;
	}
}

ErrorArray* initErrorArray(){
	ErrorArray* errors = (ErrorArray*)malloc(sizeof(ErrorArray));
	errors->cappacity = 1;
	errors->errors = (Error**)malloc(sizeof(Error));
	errors->errorCount = 0;
	return errors;
}

void freeType(Type* t){
	switch(t->kind){
		case Trivial_KIND:{
			free(t);
			break;
		}
		default:{
			printf("Invalid Kind\n");
			exit(1);
			break;
		}
	}
}

bool compareTypes(Type* t1, Type* t2){
	return getTrivialType(t1) == getTrivialType(t2);
}

Type* analyzePrint(Analyzer* a, ErrorArray* errors, ASTPrint* p){
	Type* t = analyzeNode(a, errors, p->expr);
	if(getTrivialType(t) == TYPE_MISMATCH_ERROR_TYPE){
		char* errorMsg;
		strcpy(errorMsg, "Print Statement");
		emitError(errors, newError(newTrivialType(TYPE_MISMATCH_ERROR_TYPE), errorMsg, p->line));
	}
	return t;
}

Type* analyzeExpression(Analyzer* a, ErrorArray* errors, ASTExpression* expr){
	Type* t = analyzeNode(a, errors, expr->expr);
	if(getTrivialType(t) == TYPE_MISMATCH_ERROR_TYPE){
		char* errorMsg;
		strcpy(errorMsg, "Expression");
		emitError(errors, newError(newTrivialType(TYPE_MISMATCH_ERROR_TYPE), errorMsg, expr->line));
	}
	return t;
}

Type* analyzeBinary(Analyzer* a, ErrorArray* errors, ASTBinaryOP* binOP){
	Type* type1 = analyzeNode(a, errors, binOP->lhs);
	Type* type2 = analyzeNode(a, errors, binOP->rhs);
	TrivialType t1 = getTrivialType(type1);
	TrivialType t2 = getTrivialType(type2);
	freeType(type1);
	freeType(type2);
	if(t1 == -1 || t2 == -1) return newTrivialType(TYPE_MISMATCH_ERROR_TYPE);
	switch (binOP->op){
		case PLUS_OP:
		case SUB_OP:
		case MULT_OP:
		case EXP_OP:
		case DIV_OP:{
			if(t1 == I32_TYPE && t2 == I32_TYPE) return newTrivialType(I32_TYPE);
			else if(t1 == F32_TYPE) {if(t2 == F32_TYPE || t2 == I32_TYPE) return newTrivialType(F32_TYPE);}
			else if(t2 == F32_TYPE) {if(t1 == F32_TYPE || t1 == I32_TYPE) return newTrivialType(F32_TYPE);}
			else return newTrivialType(TYPE_MISMATCH_ERROR_TYPE);
		}
		case REM_OP:{
			if(t1 == I32_TYPE && t2 == I32_TYPE) return newTrivialType(I32_TYPE);
			else return newTrivialType(TYPE_MISMATCH_ERROR_TYPE);
		}
		case AND_OP:
		case OR_OP:{
			if(t1 == BOOL_TYPE && t2 == BOOL_TYPE) return newTrivialType(BOOL_TYPE);
			else return newTrivialType(TYPE_MISMATCH_ERROR_TYPE);
		}
		case LESS_OP:
		case GREATER_OP:
		case LOE_OP:
		case GOE_OP:{
			if((t1 == I32_TYPE || t1 == F32_TYPE) && (t2 == I32_TYPE || t2 == F32_TYPE)) return newTrivialType(BOOL_TYPE);
			else return newTrivialType(TYPE_MISMATCH_ERROR_TYPE);		
		}
		default:{
			return newTrivialType(TYPE_MISMATCH_ERROR_TYPE);
		}	
		//case custom op ...
	}
}

Type* analyzeUnary(Analyzer* a, ErrorArray* errors, ASTUnaryOP* unOP){
	Type* type = analyzeNode(a, errors, unOP->opperand);
	TrivialType t = getTrivialType(type);
	freeType(type); 
	switch (unOP->op){
		case PLUS_OP:
		case SUB_OP:{
			if(t == I32_TYPE || t == F32_TYPE) return newTrivialType(t);
			else return newTrivialType(TYPE_MISMATCH_ERROR_TYPE);
		}
		case FACT_OP:{
			if(t == I32_TYPE) return newTrivialType(I32_TYPE);			
			else return newTrivialType(TYPE_MISMATCH_ERROR_TYPE);
		}
		case NOT_OP:{
			if(t == BOOL_TYPE) return newTrivialType(BOOL_TYPE);
			else return newTrivialType(TYPE_MISMATCH_ERROR_TYPE);
		}
		default:{
			return newTrivialType(TYPE_MISMATCH_ERROR_TYPE);
		}	
	}
}

Type* analyzeValue(Analyzer* a, ErrorArray* errors, ASTValue* v){
	switch(v->v.type){
		case I32_VAL: return newTrivialType(I32_TYPE);
		case F32_VAL: return newTrivialType(F32_TYPE);
		case STR_VAL: return newTrivialType(STR_TYPE);
		case BOOL_VAL: return newTrivialType(BOOL_TYPE);
		default: return newTrivialType(TYPE_MISMATCH_ERROR_TYPE);
	}
}

Type* analyzeGlobalVariable(Analyzer* a, ErrorArray* errors, ASTGlobalVariable* var){
	Type* t = analyzeNode(a, errors, var->expr);
	if(getTrivialType(var->type) == INFERRED_TYPE) var->type = t;
	else { 
		if(var->type != t){
			size_t size = snprintf(NULL, 0, "Global Variable Declaration \"%s\"", var->id) + 1;
			char* errorMsg = (char*)malloc(size);
			snprintf(errorMsg, size, "Global Variable Declaration \"%s\"", var->id);
			emitError(errors, newError(newTrivialType(TYPE_MISMATCH_ERROR_TYPE), errorMsg, var->line));
		}
	}
	Value v;
	v.type = I32_VAL;
	v.i32 = -1;
	if(var->index == -2){
		size_t size = snprintf(NULL, 0, "Global Variable Declaration \"%s\"", var->id) + 1;
		char* errorMsg = (char*)malloc(size);
		snprintf(errorMsg, size, "Global Variable Declaration \"%s\"", var->id);
		emitError(errors, newError(newTrivialType(REDECLARATION_ERROR_TYPE), errorMsg, var->line));
	}
	else set(a->globalVarTypes, var->id, -1, v);
	//id size
	return t;
}

/*
	I32_TYPE,
	F32_TYPE,
	STR_TYPE,
	BOOL_TYPE,
	INFERRED_TYPE,
	ERROR_TYPE,
	VOID_TYPE,
*/

Type* analyzeLocalVariable(Analyzer* a, ErrorArray* errors, ASTLocalVariable* var){
	Type* t = analyzeNode(a, errors, var->expr);
	if(getTrivialType(var->type) == INFERRED_TYPE) var->type = t;
	else {
		size_t size = snprintf(NULL, 0, "Local Variable Declaration \"%s\"", var->id) + 1;
		char* errorMsg = (char*)malloc(size);
		snprintf(errorMsg, size, "Local Variable Declaration \"%s\"", var->id);
		if(compareTypes(var->type, t)) emitError(errors, newError(newTrivialType(TYPE_MISMATCH_ERROR_TYPE), errorMsg, var->line));
	}
	Value v;
	v.type = I32_VAL;
	v.i32 = -1;
	if(var->offset == -2){
		size_t size = snprintf(NULL, 0, "Local Variable Declaration \"%s\"", var->id) + 1;
		char* errorMsg = (char*)malloc(size);
		snprintf(errorMsg, size, "Local Variable Declaration \"%s\"", var->id);
		emitError(errors, newError(newTrivialType(REDECLARATION_ERROR_TYPE), errorMsg, var->line));
	}
	else addToCurrentScope(a->localVarTypes, var->id, -1, -1);
	return t;
}

Type* analyzeLocalVariableReference(Analyzer* a, ErrorArray* errors, ASTLocalID* id){
	Type* t = NULL;//searchScopes(a->localVarTypes, id->id);
	if(id->index == -1) {
		size_t size = snprintf(NULL, 0, "Variable Reference \"%s\"", id->id) + 1;
		char* errorMsg = (char*)malloc(size);
		snprintf(errorMsg, size, "Variable Reference \"%s\"", id->id);
		emitError(errors, newError(newTrivialType(UNDECLARED_ERROR_TYPE), errorMsg, id->line));
	}
	return t;
}

Type* analyzeGlobalVariableReference(Analyzer* a, ErrorArray* errors, ASTGlobalID* id){
	Type* t = NULL; //get(a->globalVarTypes, id->id).i32;
	if(id->index == -1) {
		size_t size = snprintf(NULL, 0, "Variable Reference \"%s\"", id->id) + 1;
		char* errorMsg = (char*)malloc(size);
		snprintf(errorMsg, size, "Variable Reference \"%s\"", id->id);
		emitError(errors, newError(newTrivialType(UNDECLARED_ERROR_TYPE), errorMsg, id->line));
	}
	return t;
}

Type* analyzeGlobalAssignment(Analyzer* a, ErrorArray* errors, ASTGlobalAssignment* ass){
	Type* t = analyzeNode(a, errors, ass->expr);
	if(ass->index == -1) {
		size_t size = snprintf(NULL, 0, "Variable Assignment \"%s\"", ass->id) + 1;
		char* errorMsg = (char*)malloc(size);
		snprintf(errorMsg, size, "Variable Assignment \"%s\"", ass->id);
		emitError(errors, newError(newTrivialType(UNDECLARED_ERROR_TYPE), errorMsg, ass->line));
		return newTrivialType(UNDECLARED_ERROR_TYPE);
	}
	if(getTrivialType(t) != get(a->globalVarTypes, ass->id).i32){
		size_t size = snprintf(NULL, 0, "Global Variable Assignment \"%s\"", ass->id) + 1;
		char* errorMsg = (char*)malloc(size);
		snprintf(errorMsg, size, "Global Variable Assignment \"%s\"", ass->id);
		emitError(errors, newError(newTrivialType(TYPE_MISMATCH_ERROR_TYPE), errorMsg, ass->line));
	}
	return t;
}

Type* analyzeLocalAssignment(Analyzer* a, ErrorArray* errors, ASTLocalAssignment* ass){
	Type* t = analyzeNode(a, errors, ass->expr);
	if(ass->offset == -1){
		size_t size = snprintf(NULL, 0, "Variable Assignment \"%s\"", ass->id) + 1;
		char* errorMsg = (char*)malloc(size);
		snprintf(errorMsg, size, "Variable Assignment \"%s\"", ass->id);
		emitError(errors, newError(newTrivialType(UNDECLARED_ERROR_TYPE), errorMsg, ass->line));
		return newTrivialType(UNDECLARED_ERROR_TYPE);
	}
	if(getTrivialType(t) != searchScopes(a->localVarTypes, ass->id)){
		size_t size = snprintf(NULL, 0, "Local Variable Assignment \"%s\"", ass->id) + 1;
		char* errorMsg = (char*)malloc(size);
		snprintf(errorMsg, size, "Local Variable Assignment \"%s\"", ass->id);
		emitError(errors, newError(newTrivialType(TYPE_MISMATCH_ERROR_TYPE), errorMsg, ass->line));
	}
	return t;
}

Type* analyzeBlock(Analyzer* a, ErrorArray* errors, ASTBlock* b){
	newScope(a->localVarTypes);
	for(int i = 0; i < b->numberOfNodes; i++){
		freeType(analyzeNode(a, errors, &b->nodes[i]));
	}
	closeScope(a->localVarTypes);
	return newTrivialType(VOID_TYPE);
}

Type* analyzeString(Analyzer* a, ErrorArray* errors, ASTString* str){
	return newTrivialType(STR_TYPE);
}

Type* analyzeLoop(Analyzer* a, ErrorArray* errors, ASTForLoop* loop){
	//figure out the type, then type check it, append types
	//if(){}
	newScope(a->localVarTypes);
	if(loop->n1 != NULL){
		Type* type = analyzeNode(a, errors, loop->n1);
		TrivialType t = getTrivialType(type);
		freeType(type);
		if(t == TYPE_MISMATCH_ERROR_TYPE || (t != BOOL_TYPE && t != I32_TYPE)){
			char errorMsg[14];
			strcpy(errorMsg, "For Statement");
			emitError(errors, newError(newTrivialType(TYPE_MISMATCH_ERROR_TYPE), errorMsg, loop->line));
		}
		if(loop->n2 != NULL){
			freeType(analyzeNode(a, errors, loop->n2));
			analyzeNode(a, errors, loop->n3);
		}
		//for-in loop, later...
	}
	Type* type = analyzeNode(a, errors, loop->min);
	TrivialType t = getTrivialType(type);
	freeType(type);
	if(loop->min != NULL) if(t != I32_TYPE){
		char* errorMsg;
		char* msg = "Loop Minimum";
		strcpy(errorMsg, msg);
		emitError(errors, newError(newTrivialType(TYPE_MISMATCH_ERROR_TYPE), errorMsg, loop->line));
	}
	type = analyzeNode(a, errors, loop->max);
	t = getTrivialType(type);
	freeType(type);
	if(loop->max != NULL) if(t != I32_TYPE){
		char* errorMsg;
		char* msg = "Loop Maximum";
		strcpy(errorMsg, msg);
		emitError(errors, newError(newTrivialType(TYPE_MISMATCH_ERROR_TYPE), errorMsg, loop->line));
	}
	newScope(a->localVarTypes);
	type = analyzeNode(a, errors, loop->b);
	freeType(type);
	closeScope(a->localVarTypes);
	closeScope(a->localVarTypes);
	return newTrivialType(VOID_TYPE);
}	

Type* analyzeElse(Analyzer* a, ErrorArray* errors, ASTElse* elseS){
	elseS->t = analyzeNode(a, errors, elseS->s);
	return elseS->t;
}

Type* analyzeIf(Analyzer* a, ErrorArray* errors, ASTIf* ifS){
	Type* type = analyzeNode(a, errors, ifS->expr);
	TrivialType t = getTrivialType(type);
	if(t != BOOL_TYPE) {
		char* errorMsg;
		char* msg = "If Statements Must Use Boolean Expressions";
		strcpy(errorMsg, msg);
		emitError(errors, newError(newTrivialType(TYPE_MISMATCH_ERROR_TYPE), errorMsg, ifS->line));
	}
	type = analyzeNode(a, errors, ifS->s);
	if(ifS->elseS != NULL) analyzeElse(a, errors, &ifS->elseS->elseS);
	return type;
}

Type* analyzeNode(Analyzer* a, ErrorArray* errors, ASTNode* n){
	switch(n->type){
		case ASTPrint_NODE_TYPE:{
			Type* t = analyzePrint(a, errors, &n->print);
			n->print.t = t;
			return t;
		}
		case ASTExpression_NODE_TYPE:{
			Type* t = analyzeExpression(a, errors, &n->expr);
			n->expr.t = t;
			return t;
		}
		case ASTBinaryOP_NODE_TYPE :{
			Type* t = analyzeBinary(a, errors, &n->binaryOP);
			n->binaryOP.t = t;
			return t;
		}
		case ASTUnaryOP_NODE_TYPE : {
			Type* t = analyzeUnary(a, errors, &n->unaryOP);
			n->unaryOP.t = t;
			return t;
		}
		case ASTValue_NODE_TYPE:{
			Type* t = analyzeValue(a, errors, &n->value);
			n->value.t = analyzeValue(a, errors, &n->value);
			return t;
		}
		case ASTGlobalVariable_NODE_TYPE:{
			Type* t = analyzeGlobalVariable(a, errors, &n->globalVar);
			n->globalVar.t = t;
			return t;
		}
		case ASTGlobalID_NODE_TYPE:{
			Type* t = analyzeGlobalVariableReference(a, errors, &n->globalID);
			n->globalID.t = t;
			return t;
		}
		case ASTLocalVariable_NODE_TYPE:{
			Type* t = analyzeLocalVariable(a, errors, &n->localVar);
			n->localVar.t = t;
			return t;
		}
		case ASTLocalID_NODE_TYPE:{
			Type* t = analyzeLocalVariableReference(a, errors, &n->localID);		
			n->localID.t = t;
			return t;
		}
		case ASTGlobalAssignment_NODE_TYPE:{
			Type* t = analyzeGlobalAssignment(a, errors, &n->globalAss);
			n->globalAss.t = t;
			return t;
		}
		case ASTLocalAssignment_NODE_TYPE:{
			Type* t = analyzeLocalAssignment(a, errors, &n->localAss);
			n->localAss.t = t;
			return t;
		}
		case ASTBlock_NODE_TYPE:{
			Type* t = analyzeBlock(a, errors, &n->block);
			n->block.t = t;
			return t;
		}
		case ASTString_NODE_TYPE:{
			Type* t = analyzeString(a, errors, &n->str);
			n->str.t = t;
			return t;
		}
		case ASTForLoop_NODE_TYPE:{
			Type* t = analyzeLoop(a, errors, &n->loop);
			n->loop.t = t;
			return t;			
		}
		case ASTIf_NODE_TYPE:{
			Type* t = analyzeIf(a, errors, &n->ifS);
			n->ifS.t = t;
			break;
		}
		default : {
			printf("Cannot analyze node of given type: %d", n->type);
			exit(1);
		}
	}
	return newTrivialType(TYPE_MISMATCH_ERROR_TYPE);
}


ErrorArray* analyze(Analyzer* a, AST* ast){
	for(int i = 0; i < ast->numberOfNodes; i++){
		freeType(analyzeNode(a, a->errors, &ast->nodes[i]));
	}
}
/*
ErrorArray* checkAndInferTypes(ErrorArray* errors, AST* ast){
	
}*/

void printError(Error* error){
	//it depends on the type
	//clean this up
	switch(getTrivialType(error->type)){
		case TYPE_MISMATCH_ERROR_TYPE :{
			printf("[Type Mismatch Error]: %s Line:%ld\n", error->message, error->line);
			break;
		}
		case UNDECLARED_ERROR_TYPE :{
			printf("[Undeclared Variable Error]: %s Line:%ld\n", error->message, error->line);
			break;
		}
		case REDECLARATION_ERROR_TYPE :{
			printf("[Redeclaration Variable Error]: %s Line:%ld\n", error->message, error->line);	
			break;			
		}
		default:{
			printf("Unidentified Error Type\n");
			exit(1);
		}
	}
	//printf("ERROR: %s Line: %ld\n", error->message, error->line);
}

void printErrors(ErrorArray* errors){
	for(int i = 0; i < errors->errorCount; i++){
		printError(errors->errors[i]);
	}
}

void freeErrors(Error** errors, int errorCount){
	for(int i = 0; i < errorCount; i++){
		freeType(errors[i]->type);
		free(errors[i]->message);
	}
	free(errors);
}

void freeErrorArray(ErrorArray* errorArray){
	freeErrors(errorArray->errors, errorArray->errorCount);
	free(errorArray);
}
void freeAnalyzer(Analyzer* analyzer){
	freeErrorArray(analyzer->errors);
	freeTable(analyzer->globalVarTypes);
	free(analyzer->localVarTypes);
	free(analyzer);
}



ErrorArray* newErrorArray(){
	ErrorArray* errorArray = (ErrorArray*) malloc(sizeof(ErrorArray));
	errorArray->cappacity = 1;
	errorArray->errorCount = 0;
	errorArray->errors = (Error**)malloc(sizeof(Error*));
	return errorArray;
}
Analyzer* newAnalyzer(int globalCount){
	Analyzer * analyzer = (Analyzer*)malloc(sizeof(Analyzer));
	analyzer->errors = newErrorArray();
	analyzer->globalVarTypes = newTable(globalCount);
	analyzer->localVarTypes = newScopeChain();
	return analyzer;
}



//Fix error format
//different types of errors
//differntiate between assignment and declarations ... 
//return error type
//Errors should include names of variables


//undeclared error should be the only error - Done I think
