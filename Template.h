#ifndef TEMPLATE_H_ 
#define TEMPLATE_H_
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "RedBlackTree.h"
#include "Common.h"

typedef struct Template Template;

typedef enum{
	EXPR_PART,
	BLOCK_PART,
	WORD_PART,
}TemplatePartType;

typedef enum{
	PROC_TEMPLATE,
	OP_TEMPLATE,
}TemplateType;

typedef enum{
	PREFIX_OP,
	POSTFIX_OP,
	INFIX_OP,
}OperatorType;

typedef struct{
	char* id;
	Type* t;
	TemplatePartType type;
}TemplatePart;

typedef struct {
	char* op;
	Type* t;
	int prec;
	bool ass;
	TemplatePart* expr1;
	TemplatePart* expr2;
	OperatorType type;
} OpTemplate;

//add more block id, ...


struct Template{
	union{
		OpTemplate op;
	};
	TemplateType type;
};

TemplatePart* newExpressionPart(char* id, Type* t);
Template* newOpTemplate(TemplatePart* e1, TemplatePart* e2, char* op, Type* t, int prec, bool ass, OperatorType type);
void printTemplate(Template* t);
void freeTemplate(Template* t);

#endif