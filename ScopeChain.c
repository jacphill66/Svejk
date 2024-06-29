#include "ScopeChain.h"
#include "assert.h"

ScopeNode* newScopeNode(RedBlackTree* tree, ScopeNode* next, ScopeNode* prev){
	ScopeNode* n = (ScopeNode*)malloc(sizeof(ScopeNode));
	n->scope = tree;
	n->next = next;
	n->prev = prev;
	n->offset = 0;
	return n;
}

ScopeChain* newScopeChain(){
	ScopeChain* sc = (ScopeChain*)malloc(sizeof(ScopeChain));
	sc->head = NULL;
	sc->tail = NULL;
	return sc;
}

void newScope(ScopeChain* sc){
	if(sc->head == NULL){
		sc->head = newScopeNode(newTree(), NULL, NULL);
		sc->tail = sc->head;
	}
	else{
		sc->tail->next = newScopeNode(newTree(), NULL, sc->tail);
		sc->tail = sc->tail->next;
	}
	if(sc->tail->prev != NULL) sc->tail->offset = sc->tail->prev->offset;
}

void closeScope(ScopeChain* sc){
	if(sc->tail == NULL) return;
	else{
		freeTreeNoKeys(sc->tail->scope);
		ScopeNode* t = sc->tail->prev;
		free(sc->tail);
		sc->tail = t;
		if(sc->tail == NULL){
			sc->head = NULL;
		}
		if(sc->tail!=NULL) sc->tail->next = NULL;
	}
}

void addToCurrentScope(ScopeChain* sc, char* id, int idSize, int t){
	Value v;
	v.i32 = sc->tail->offset;
	if(t == -1) insert(sc->tail->scope, id, idSize, v);
	else {
		v.i32 = t;
		insert(sc->tail->scope, id, idSize, v);
	}
	sc->tail->offset += 1;
}

int searchScope(ScopeNode* n, char* id){
	if(contains(n->scope, id)) return search(n->scope, id).i32;
	return -1;
}

int searchScopes(ScopeChain* sc, char* id){
	ScopeNode* tail = sc->tail;
	while(tail != NULL){
		int search = searchScope(tail, id);
		if(search != -1) return search;
		tail = tail->prev;
	}
	return -1;
}

void printScopes(ScopeChain* sc){
	ScopeNode* n = sc->head;
	int count = 0;
	while(sc->head != NULL){
		printf("Scope: %d", count);
		printTree(n->scope);
		count++;
		n = n->next;
	}
}

