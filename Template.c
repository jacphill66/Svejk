#include "Template.h"

Template* newOp(TemplateType t, int prec, bool ass, char* op, Type* evalType){
	//how do I include types?
}

void printTemplate(Template* t){
	if(t == NULL){
		printf("Can't print null template!");
		exit(1);
	}
	switch(t->type){
		case EXPR_TEMPLATE:{
			if(t->expr.id != NULL) printf("Expr[%s]:", t->expr.id);
			else{
				printf("Expr:");
				printType(t->expr.t);
			}
			break;
		}
		case OP_TEMPLATE:{
			printf("Op[");
			if(t->op.type == PREFIX_OP){
				printf("%s", t->op.op);
				printTemplate(t->op.expr1);
				printf("]");
			}
			else if(t->op.type == INFIX_OP){
				printTemplate(t->op.expr1);
				printf("%s", t->op.op);
				printTemplate(t->op.expr2);
				printf("]");
			}
			else if(t->op.type == POSTFIX_OP){
				printTemplate(t->op.expr1);
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

Template* newExpressionTemplate(char* id, Type* t){
	Template* temp = (Template*)malloc(sizeof(Template));
	temp->type = EXPR_TEMPLATE;
	temp->expr.t = t;
	temp->expr.id = id;
	return temp;
}

Template* newOpTemplate(Template* e1, Template* e2, char* op, Type* t, int prec, bool ass, OperatorType type){
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