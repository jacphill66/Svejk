#ifndef REDBLACKTREE_H_ 
#define REDBLACKTREE_H_
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "Common.h"

typedef enum {RED, BLACK} Color;

typedef struct RBNode RBNode;

struct RBNode{
	Color c; 
	char* key;
	int keySize;
	Value value;
	RBNode* parent;
	RBNode* left;
	RBNode* right;
};

typedef struct {
	RBNode* root;
	RBNode* nil;
	int size;
} RedBlackTree;

RedBlackTree* newTree();
void insert(RedBlackTree* tree, char* key, int keySize, Value value);
void delete_(RedBlackTree* tree, char* key);
void printTree(RedBlackTree* tree);
void freeTreeNoKeys(RedBlackTree* tree);
void freeTree(RedBlackTree* tree);
Value search(RedBlackTree* tree, char* key);
RBNode* newNode(RBNode* parent, RBNode* left, RBNode* right, char* key, int keySize, Value value, Color c);
void testRedBlackTree(RedBlackTree* tree);
char* getKeys(RedBlackTree* tree);
int* getValues(RedBlackTree* tree);
void treeToArray(RedBlackTree* tree, RBNode* n, int* index, char** keys, Value* values);
bool contains(RedBlackTree* tree, char* id);
#endif
