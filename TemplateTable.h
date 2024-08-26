#ifndef TEMPLATETABLE_H_ 
#define TEMPLATETABLE_H_
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "Template.h"
#include "Table.h"

typedef struct TMNode TMNode;

struct TMNode{
	TMNode* next;
	Template* t;
};

typedef struct{
	TMNode** elements;
	long length;
	long size;
}TemplateTable;

TemplateTable* newTemplateTable(int size);
TMNode* getTemplateTable(TemplateTable* t, char* key);
void setTemplateTable(TemplateTable* t, char* key, Template* temp);
void printTemplateTable(TemplateTable* t);
void freeTemplateTable(TemplateTable* t);

#endif