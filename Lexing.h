#ifndef LEXING_H_ 
#define LEXING_H_

#include "Common.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "Trie.h"

typedef enum{
	INT_TOKEN,
	FLOAT_TOKEN,
	STR_A_TOKEN,
	STR_B_TOKEN,	
	WORD_TOKEN,
	SYM_TOKEN,
	SEMI_COLON_TOKEN,
	END_LINE_TOKEN,
	END_TOKEN,
	ERROR_TOKEN,
} TokenType;

typedef struct {
	TokenType type;
	char* value;
	long size;
	long line;
} Token;

typedef struct {
	Token* tokens;
	long cappacity;
	long tokenCount;
} TokenArray;

typedef struct{
	TokenArray* tokens;
	Trie* trie;
	bool invalidRead;
}Lexer;

/*Initialization */
Token newToken(TokenType type, char* value, long size, long lineNumber);
Lexer* newLexer();

/*Freeing*/
char* freeLexer(Lexer* l);

/*Printing*/
char* printToken(Token* t);
char* printTokens(TokenArray* arr);

/* Lexes a text file to tokens*/
char* lex(Lexer* l, char* path);

#endif