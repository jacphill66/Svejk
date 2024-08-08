#ifndef TEMPLATETREE_H_ 
#define TEMPLATETREE_H_
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "RedBlackTree.h"
#include "Template.h"

typedef struct TemplateRBNode TemplateRBNode;

struct TemplateRBNode{
	Color c; 
	char* key;
	int keySize;
	Template* t;
	TemplateRBNode* parent;
	TemplateRBNode* left;
	TemplateRBNode* right;
};

typedef struct {
	TemplateRBNode* root;
	TemplateRBNode* nil;
	int size;
} TemplateRedBlackTree;

TemplateRedBlackTree* newTemplateTree();
void insertTemplate(TemplateRedBlackTree* tree, char* key, int keySize, Template* t);
void deleteTemplate(TemplateRedBlackTree* tree, char* key);
void printTemplateTree(TemplateRedBlackTree* tree);
void freeTemplateTreeNoKeys(TemplateRedBlackTree* tree);
void freeTemplateTree(TemplateRedBlackTree* tree);
Template* searchTemplate(TemplateRedBlackTree* tree, char* key);
TemplateRBNode* newNodeTemplate(TemplateRBNode* parent, TemplateRBNode* left, TemplateRBNode* right, char* key, int keySize, Template* t, Color c);
bool containsTemplate(TemplateRedBlackTree* tree, char* id);

#endif