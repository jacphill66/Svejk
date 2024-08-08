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
	EXPR_TEMPLATE,
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
}ExprTemplate;

typedef struct {
	char* op;
	Type* t;
	int prec;
	bool ass;
	Template* expr1;
	Template* expr2;
	OperatorType type;
} OpTemplate;

//add more block id, ...


struct Template{
	union{
		ExprTemplate expr;
		OpTemplate op;
	};
	TemplateType type;
};

Template* newExpressionTemplate(char* id, Type* t);
Template* newOpTemplate(Template* e1, Template* e2, char* op, Type* t, int prec, bool ass, OperatorType type);
//newOpTemplate(newExpressionTemplate(NULL, newTrivialType(i32)), newExpressionTemplate(NULL, newTrivialType(i32)), "+", newTrivialType(i32));
void printTemplate(Template* t);
void freeTemplate(Template* t);

#endif