#ifndef LEXING_H_ 
#define LEXING_H_

#include "Common.h"
#include <string.h>


typedef enum{
	I32_VAL_TOKEN,
	F32_VAL_TOKEN,
	STR_VAL_TOKEN,
	TRUE_VAL_TOKEN,
	FALSE_VAL_TOKEN,
	
	ID_TOKEN,

	PLUS_OP_TOKEN,
	SUB_OP_TOKEN,
	MULT_OP_TOKEN,
	DIV_OP_TOKEN,
	REM_OP_TOKEN,
	EXP_OP_TOKEN,

	
	FACT_OP_TOKEN,
	
	EQUAL_OP_TOKEN,
	
	LESS_OP_TOKEN,
	GREATER_OP_TOKEN,
	LOE_OP_TOKEN,
	GOE_OP_TOKEN,
	
	AND_OP_TOKEN,
	OR_OP_TOKEN,
	NOT_OP_TOKEN,
	
	LPAREN_OP_TOKEN,
	RPAREN_OP_TOKEN,
	
	PRINT_TOKEN,
	
	END_LINE_TOKEN,
	
	END_TOKEN,
	
} TokenType;

typedef struct {
	TokenType type;
	char* value;
	long size;
} Token;

typedef struct {
	Token* tokens;
	long cappacity;
	long tokenCount;
} TokenArray;

TokenArray lex(char* path);
void printTokens(TokenArray* arr);
void freeTokens(TokenArray* tokens);

#endif