#include "Analyzer.h"

Error* newError(ErrorType type, char* message, long line){
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

Type analyzePrint(Analyzer* a, ErrorArray* errors, ASTPrint* p){
	return analyzeNode(a, errors, p->expr);
}

Type analyzeExpression(Analyzer* a, ErrorArray* errors, ASTExpression* expr){
	return analyzeNode(a, errors, expr->expr);
}

Type analyzeBinary(Analyzer* a, ErrorArray* errors, ASTBinaryOP* binOP){
	Type t1 = analyzeNode(a, errors, binOP->lhs);
	Type t2 = analyzeNode(a, errors, binOP->rhs);
	switch (binOP->op){
		case PLUS_OP:
		case SUB_OP:
		case MULT_OP:
		case DIV_OP:{
			if(t1 == I32_TYPE && t2 == I32_TYPE) return I32_TYPE;
			else if(t1 == F32_TYPE) if(t2 == F32_TYPE || t2 == I32_TYPE) return F32_TYPE;
			else if(t2 == F32_TYPE) if(t1 == F32_TYPE || t1 == I32_TYPE) return F32_TYPE;	
			else return ERROR_TYPE;
		}
		case REM_OP:{
			if(t1 == I32_TYPE && t2 == I32_TYPE) return I32_TYPE;
			else return ERROR_TYPE;
		}
		case AND_OP:
		case OR_OP:{
			if(t1 == BOOL_TYPE && t2 == BOOL_TYPE) return BOOL_TYPE;
			else return ERROR_TYPE;
		}
		default:{
			return ERROR_TYPE;
		}	
		//case custom op ...
	}
}

Type analyzeUnary(Analyzer* a, ErrorArray* errors, ASTUnaryOP* unOP){
	Type t = analyzeNode(a, errors, unOP->opperand);
	switch (unOP->op){
		case PLUS_OP:
		case SUB_OP:{
			if(t == I32_TYPE || t == F32_TYPE) return t;
			else return ERROR_TYPE;
		}
		case FACT_OP:{
			if(t == I32_TYPE) return I32_TYPE;			
			else return ERROR_TYPE;
		}
		case NOT_OP:{
			if(t == BOOL_TYPE) return BOOL_TYPE;
			else return ERROR_TYPE;
		}
		default:{
			return ERROR_TYPE;
		}	
	}
}

Type analyzeValue(Analyzer* a, ErrorArray* errors, ASTValue* v){
	switch(v->v.type){
		case I32_VAL: return I32_TYPE;
		case F32_VAL: return F32_TYPE;
		case STR_VAL: return STR_TYPE;
		case BOOL_VAL: return BOOL_TYPE;
		default: return ERROR_TYPE;
	}
}

Type analyzeGlobalVariable(Analyzer* a, ErrorArray* errors, ASTGlobalVariable* var){
	Type t = analyzeNode(a, errors, var->expr);
	if(var->type == INFERRED_TYPE) var->type = t;
	else { 
		if(var->type != t){
			size_t size = snprintf(NULL, 0, "Global Variable Declaration \"%s\"", var->id) + 1;
			char* errorMsg = (char*)malloc(size);
			snprintf(errorMsg, size, "Global Variable Declaration \"%s\"", var->id);
			emitError(errors, newError(TYPE_MISMATCH_ERROR, errorMsg, var->line));
		}
	}
	Value v;
	v.type = I32_VAL;
	v.i32 = t;
	if(var->index == -2){
		size_t size = snprintf(NULL, 0, "Global Variable Declaration \"%s\"", var->id) + 1;
		char* errorMsg = (char*)malloc(size);
		snprintf(errorMsg, size, "Global Variable Declaration \"%s\"", var->id);
		emitError(errors, newError(REDECLARATION_ERROR, errorMsg, var->line));
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

Type analyzeLocalVariable(Analyzer* a, ErrorArray* errors, ASTLocalVariable* var){
	Type t = analyzeNode(a, errors, var->expr);
	if(var->type == INFERRED_TYPE) var->type = t;
	else {
		size_t size = snprintf(NULL, 0, "Local Variable Declaration \"%s\"", var->id) + 1;
		char* errorMsg = (char*)malloc(size);
		snprintf(errorMsg, size, "Local Variable Declaration \"%s\"", var->id);
		if(var->type != t) emitError(errors, newError(TYPE_MISMATCH_ERROR, errorMsg, var->line));
	}
	Value v;
	v.type = I32_VAL;
	v.i32 = t;
	if(var->offset == -2){
		size_t size = snprintf(NULL, 0, "Local Variable Declaration \"%s\"", var->id) + 1;
		char* errorMsg = (char*)malloc(size);
		snprintf(errorMsg, size, "Local Variable Declaration \"%s\"", var->id);
		emitError(errors, newError(REDECLARATION_ERROR, errorMsg, var->line));
	}
	else addToCurrentScope(a->localVarTypes, var->id, -1, t);
	return t;
}

Type analyzeLocalVariableReference(Analyzer* a, ErrorArray* errors, ASTLocalID* id){
	Type t = searchScopes(a->localVarTypes, id->id);
	if(id->index == -1) {
		size_t size = snprintf(NULL, 0, "Variable Reference \"%s\"", id->id) + 1;
		char* errorMsg = (char*)malloc(size);
		snprintf(errorMsg, size, "Variable Reference \"%s\"", id->id);
		emitError(errors, newError(UNDECLARED_ERROR, errorMsg, id->line));
	}
	return t;
}

Type analyzeGlobalVariableReference(Analyzer* a, ErrorArray* errors, ASTGlobalID* id){
	Type t = get(a->globalVarTypes, id->id).i32;
	if(id->index == -1) {
		size_t size = snprintf(NULL, 0, "Variable Reference \"%s\"", id->id) + 1;
		char* errorMsg = (char*)malloc(size);
		snprintf(errorMsg, size, "Variable Reference \"%s\"", id->id);
		emitError(errors, newError(UNDECLARED_ERROR, errorMsg, id->line));
	}
	return t;
}

Type analyzeGlobalAssignment(Analyzer* a, ErrorArray* errors, ASTGlobalAssignment* ass){
	Type t = analyzeNode(a, errors, ass->expr);
	if(ass->index == -1) {
		size_t size = snprintf(NULL, 0, "Variable Assignment \"%s\"", ass->id) + 1;
		char* errorMsg = (char*)malloc(size);
		snprintf(errorMsg, size, "Variable Assignment \"%s\"", ass->id);
		emitError(errors, newError(UNDECLARED_ERROR, errorMsg, ass->line));
		return ERROR_TYPE;
	}
	if(t != get(a->globalVarTypes, ass->id).i32){
		size_t size = snprintf(NULL, 0, "Global Variable Assignment \"%s\"", ass->id) + 1;
		char* errorMsg = (char*)malloc(size);
		snprintf(errorMsg, size, "Global Variable Assignment \"%s\"", ass->id);
		emitError(errors, newError(TYPE_MISMATCH_ERROR, errorMsg, ass->line));
	}
	return t;
}

Type analyzeLocalAssignment(Analyzer* a, ErrorArray* errors, ASTLocalAssignment* ass){
	Type t = analyzeNode(a, errors, ass->expr);
	if(ass->offset == -1){
		size_t size = snprintf(NULL, 0, "Variable Assignment \"%s\"", ass->id) + 1;
		char* errorMsg = (char*)malloc(size);
		snprintf(errorMsg, size, "Variable Assignment \"%s\"", ass->id);
		emitError(errors, newError(UNDECLARED_ERROR, errorMsg, ass->line));
		return ERROR_TYPE;
	}
	if(t != searchScopes(a->localVarTypes, ass->id)){
		size_t size = snprintf(NULL, 0, "Local Variable Assignment \"%s\"", ass->id) + 1;
		char* errorMsg = (char*)malloc(size);
		snprintf(errorMsg, size, "Local Variable Assignment \"%s\"", ass->id);
		emitError(errors, newError(TYPE_MISMATCH_ERROR, errorMsg, ass->line));
	}
	return t;
}

Type analyzeBlock(Analyzer* a, ErrorArray* errors, ASTBlock* b){
	newScope(a->localVarTypes);
	for(int i = 0; i < b->numberOfNodes; i++){
		analyzeNode(a, errors, &b->nodes[i]);
	}
	closeScope(a->localVarTypes);
	return 0;
}

Type analyzeString(Analyzer* a, ErrorArray* errors, ASTString* str){
	return STR_TYPE;
}

Type analyzeNode(Analyzer* a, ErrorArray* errors, ASTNode* n){
	switch(n->type){
		case ASTPrint_NODE_TYPE:{
			Type t = analyzePrint(a, errors, &n->print);
			n->t = t;
			return t;
		}
		case ASTExpression_NODE_TYPE:{
			Type t = analyzeExpression(a, errors, &n->expr);
			n->t = t;
			return t;
		}
		case ASTBinaryOP_NODE_TYPE :{
			Type t = analyzeBinary(a, errors, &n->binaryOP);
			n->t = t;
			return t;
		}
		case ASTUnaryOP_NODE_TYPE : {
			Type t = analyzeUnary(a, errors, &n->unaryOP);
			n->t = t;
			return t;
		}
		case ASTValue_NODE_TYPE:{
			Type t = analyzeValue(a, errors, &n->value);
			n->t = analyzeValue(a, errors, &n->value);
			return t;
		}
		case ASTGlobalVariable_NODE_TYPE:{
			Type t = analyzeGlobalVariable(a, errors, &n->globalVar);
			n->t = t;
			return t;
		}
		case ASTGlobalID_NODE_TYPE:{
			Type t = analyzeGlobalVariableReference(a, errors, &n->globalID);
			return t;
		}
		case ASTLocalVariable_NODE_TYPE:{
			Type t = analyzeLocalVariable(a, errors, &n->localVar);
			n->t = t;
			return t;
		}
		case ASTLocalID_NODE_TYPE:{
			Type t = analyzeLocalVariableReference(a, errors, &n->localID);		
			n->t = t;
			return t;
		}
		case ASTGlobalAssignment_NODE_TYPE:{
			return analyzeGlobalAssignment(a, errors, &n->globalAss);
		}
		case ASTLocalAssignment_NODE_TYPE:{
			return analyzeLocalAssignment(a, errors, &n->localAss);
		}
		case ASTBlock_NODE_TYPE:{
			return analyzeBlock(a, errors, &n->block);
		}
		case ASTString_NODE_TYPE:{
			return analyzeString(a, errors, &n->str);
		}
		default : {
			printf("Cannot analyze node of given type: %d", n->type); 
			exit(1);
		}
	}
	return ERROR_TYPE;
}


ErrorArray* analyze(Analyzer* a, AST* ast){
	for(int i = 0; i < ast->numberOfNodes; i++){
		analyzeNode(a, a->errors, &ast->nodes[i]);
	}
}
/*
ErrorArray* checkAndInferTypes(ErrorArray* errors, AST* ast){
	
}*/

void printError(Error* error){
	//it depends on the type
	//clean this up
	switch(error->type){
		case TYPE_MISMATCH_ERROR :{
			printf("[Type Mismatch Error]: %s Line:%ld\n", error->message, error->line);
			break;
		}
		case UNDECLARED_ERROR :{
			printf("[Undeclared Variable Error]: %s Line:%ld\n", error->message, error->line);
			break;
		}
		case REDECLARATION_ERROR :{
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
	for(int i = 0; i < errorCount; i++) free(errors[i]->message);
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
