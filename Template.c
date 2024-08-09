#include "Template.h"

Template* newOp(TemplateType t, int prec, bool ass, char* op, Type* evalType){
	//how do I include types?
}

void printPart(TemplatePart* p){
	switch(p->type){
		case EXPR_PART:{
			if(p->id != NULL) printf("Expr[%s]:", p->id);
			else{
				printf("Expr:");
				printType(p->t);
			}
			break;
		}
	}
}

void printTemplate(Template* t){
	if(t == NULL){
		printf("Can't print null template!");
		exit(1);
	}
	switch(t->type){
		case OP_TEMPLATE:{
			printf("Op[");
			if(t->op.type == PREFIX_OP){
				printf("%s", t->op.op);
				printPart(t->op.expr1);
				printf("]");
			}
			else if(t->op.type == INFIX_OP){
				printPart(t->op.expr1);
				printf("%s", t->op.op);
				printPart(t->op.expr2);
				printf("]");
			}
			else if(t->op.type == POSTFIX_OP){
				printPart(t->op.expr1);
				printf("%s", t->op.op);
				printf("]");
			}
			else {
				printf("Unidentified OP");
				exit(1);
			}
			printf(":");
			printType(t->op.t);
			printf("{\n");
			printf("	prec: %d\n", t->op.prec);
			if(t->op.ass) printf("	ass: right\n");
			else printf("	ass: left\n");
			printf("}\n");
			break;
		}
		default:{
			printf("Failed Printing Template\n");
			exit(1);
		}
	}
}

void freeTemplate(Template* t){
	//switch on the template type
}

TemplatePart* newExpressionPart(char* id, Type* t){
	TemplatePart* temp = (TemplatePart*)malloc(sizeof(TemplatePart));
	temp->type = EXPR_PART;
	temp->id = id;
	temp->t = t;
	return temp;
}

Template* newOpTemplate(TemplatePart* e1, TemplatePart* e2, char* op, Type* t, int prec, bool ass, OperatorType type){
	Template* temp = (Template*)malloc(sizeof(Template));
	temp->type = OP_TEMPLATE;
	temp->op.expr1 = e1;
	temp->op.expr2 = e2;
	temp->op.op = op;
	temp->op.t = t;
	temp->op.prec = prec;
	temp->op.ass = ass;
	temp->op.type = type;
	return temp;
}