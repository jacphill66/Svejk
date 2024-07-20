#ifndef ANALYZER_H_ 
#define ANALYZER_H_

#include "Parsing.h"
#include "Common.h"
#include "ScopeChain.h"
#include "TypeMap.h"

typedef struct {
	Type* type;
	char* message;
	long line;
	char** names;
} Error;

typedef struct {
	Error** errors;
	long cappacity;
	long errorCount;
} ErrorArray;

typedef struct{
	ErrorArray* errors;
	long globalCount;
	long stringCount;
	RedBlackTree* strings;
} Analysis;

typedef struct {
	Analysis* a;
	TypeScopeChain* varTypes;
} Analyzer;

Type* analyzeNode(Analyzer* a, ErrorArray* errors, ASTNode* n);
ErrorArray* analyze(Analyzer* a, AST* ast);
ErrorArray* checkAndInferTypes(ErrorArray* errors, AST* ast);
void printErrors(ErrorArray* errors);

void freeErrors(Error** errors, int errorCount);
void freeErrorArray(ErrorArray* errorArray);
void freeAnalysis(Analysis* a);
void freeAnalyzer(Analyzer* anlayzer);

ErrorArray* newErrorArray();
Analyzer* newAnalyzer();

TrivialType getTrivialType(Type* t);
Type* newTrivialType(TrivialType type);
void freeType(Type* t);
#endif