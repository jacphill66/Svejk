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
//Should I add all basic forms: var, let, if, if-else, while, ...?
typedef enum{
	EXPR_PART,
	BLOCK_PART,
	WORD_PART,
	ID_PART,
	STATEMENT_PART,
}TemplatePartType;

typedef enum{
	FORM_TEMPLATE,
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
	int line;
} OpTemplate;

typedef struct{
	TemplatePart** parts;
	int formSize;
	Type* t;
	int line;
} FormTemplate;

typedef struct{
	const char* msg;
	int line;
}ErrorTemplate;


struct Template{
	union{
		OpTemplate op;
		FormTemplate form;
	};
	TemplateType type;
};

TemplatePart* newExpressionPart(char* id, Type* t);
TemplatePart* newBlockPart(char* id, Type*t);
TemplatePart* newWordPart(char* id);
TemplatePart* newStatementPart(char* id, Type*t);
TemplatePart* newIDPart(char* id, Type*t);

TemplatePart** newFormArr(int formSize);

Template* newOpTemplate(TemplatePart* e1, TemplatePart* e2, char* op, Type* t, int prec, bool ass, OperatorType type);
Template* newFormTemplate(TemplatePart** form, int formSize, Type* t);

char* printTemplate(Template* t);
char* freeTemplate(Template* t);


#endif