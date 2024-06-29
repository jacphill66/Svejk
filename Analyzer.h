#ifndef ANALYZER_H_ 
#define ANALYZER_H_

#include "Parsing.h"
#include "Common.h"
#include "ScopeChain.h"

typedef enum {
	TYPE_MISMATCH_ERROR,
	UNDECLARED_ERROR,
	REDECLARATION_ERROR,
} ErrorType;

typedef struct {
	ErrorType type;
	char* message;
	long line;
	char** names;
} Error;

typedef struct {
	Error** errors;
	long cappacity;
	long errorCount;
} ErrorArray;

typedef struct {
	ErrorArray* errors;
	Table* globalVarTypes;
	ScopeChain* localVarTypes;
} Analyzer;

Type analyzeNode(Analyzer* a, ErrorArray* errors, ASTNode* n);
ErrorArray* analyze(Analyzer* a, AST* ast);
ErrorArray* checkAndInferTypes(ErrorArray* errors, AST* ast);
void printErrors(ErrorArray* errors);

void freeErrors(Error** errors, int errorCount);
void freeErrorArray(ErrorArray* errorArray);
void freeAnalyzer(Analyzer* anlayzer);

ErrorArray* newErrorArray();
Analyzer* newAnalyzer(int globalCount);

#endif