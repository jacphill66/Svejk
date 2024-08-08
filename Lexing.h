#ifndef LEXING_H_ 
#define LEXING_H_

#include "Common.h"
#include <string.h>

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

typedef struct TrieNode TrieNode;

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
} TokenType;

typedef struct {
	TokenType type;
	char* value;
	long size;
	long line;
} Token;

struct TrieNode{
	char letter;
	bool validEnd;
	TrieNode* next;
	//might add prev;
	TrieNode* child;
};

typedef struct{
	TrieNode* root;
}Trie;

typedef struct {
	Token* tokens;
	Trie* trie;
	long cappacity;
	long tokenCount;
} TokenArray;

TokenArray* initTokenArray();
void lex(TokenArray* tokenArr, char* path);
void printTokens(TokenArray* arr);
void freeTokens(TokenArray* tokens);
void printToken(Token* t);
void addString(Trie* t, char* str);
void addToken(char** tokens, int tokenCount, const char* token);
#endif