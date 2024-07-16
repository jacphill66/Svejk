#ifndef TYPEMAP_H_ 
#define TYPEMAP_H_
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "Common.h"
#include "RedBlackTree.h"


typedef struct TypeTreeNode TypeTreeNode;

struct TypeTreeNode{
	Color c; 
	char* key;
	int keySize;
	Type* t;
	TypeTreeNode* parent;
	TypeTreeNode* left;
	TypeTreeNode* right;
};

typedef struct {
	TypeTreeNode* root;
	TypeTreeNode* nil;
	int size;
} TypeTree;

TypeTree* newTypeTree();
void insertTypeTree(TypeTree* tree, char* key, Type* t);
void deleteTypeTree(TypeTree* tree, char* key);
void printTypeTree(TypeTree* tree);
void freeTypeTreeNoKeys(TypeTree* tree);
Type* searchTypeTree(TypeTree* tree, char* key);
TypeTreeNode* newTypeTreeNode(TypeTreeNode* parent, TypeTreeNode* left, TypeTreeNode* right, char* key, Type* t, Color c);
bool containsTypeTree(TypeTree* tree, char* id);

typedef struct{
	TypeTree** elements;
	long length;
	long size;
} TypeMap;

TypeMap* newTypeMap(int size);
Type* getTypeMap(TypeMap* t, char* key);
void setTypeMap(TypeMap* t, char* key, Type* type);
void freeTypeMap(TypeMap* t);

typedef struct TypeScopeNode TypeScopeNode;

struct TypeScopeNode{
	TypeTree* scope;
	TypeScopeNode* next;
	TypeScopeNode* prev;
};

typedef struct{
	TypeScopeNode* head;
	TypeScopeNode* tail;
}TypeScopeChain;

TypeScopeChain* newTypeScopeChain();
Type* searchTypeScope(TypeScopeNode* n, char* id);
Type* searchTypeScopes(TypeScopeChain* sc, char* id);
void newTypeScope(TypeScopeChain* sc);
void closeTypeScope(TypeScopeChain* sc);
void addToCurrentTypeScope(TypeScopeChain* sc, char* id, Type* t);
#endif
