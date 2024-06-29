#ifndef SCOPECHAIN_H_ 
#define SCOPECHAIN_H_
#include <stdlib.h>
#include <stdio.h>
#include "RedBlackTree.h"

typedef struct ScopeNode ScopeNode;

struct ScopeNode{
	RedBlackTree* scope;
	ScopeNode* next;
	ScopeNode* prev;
	int offset;
};

typedef struct{
	ScopeNode* head;
	ScopeNode* tail;
}ScopeChain;

ScopeChain* newScopeChain();

int searchScope(ScopeNode* n, char* id);
int searchScopes(ScopeChain* sc, char* id);
void newScope(ScopeChain* sc);
void closeScope(ScopeChain* sc);
void addToCurrentScope(ScopeChain* sc, char* id, int idSize, int t);
void printScopes(ScopeChain* sc);
#endif