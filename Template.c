#include "Template.h"

char* printPart(TemplatePart* p){
	if(p->id == NULL) return newString("Template Part ID is NULL");
	switch(p->type){
		case EXPR_PART:{
			printf("Expr[%s]", p->id);
			break;
		}
		case ID_PART:{
			printf("ID[%s]", p->id);
			break;
		}	
		case BLOCK_PART:{
			printf("Block[%s]", p->id);
			break;
		}
		case WORD_PART:{
			printf("Word[%s]", p->id);
			break;
		}
		case STATEMENT_PART:{
			printf("Statement[%s]", p->id);
			break;
		}
		default:{
			return newString("Attempting to Print Invalid Part");
		}
	}
	if(p->t != NULL){
		printf(":");
		char* res = printType(p->t);
		if(res==NULL) return res;
	}
	return NULL;
}

char* printOPTemplate(Template* t){
	if(t->op.op == NULL) return newString("Attempting to Print Null Op in Operator Template");
	printf("OpTemplate[");
	switch(t->op.type){
		case PREFIX_OP:{
			printf("%s", t->op.op);
			char* res = printPart(t->op.expr1);
			if(res != NULL) return res;
			printf("]");
		}
		case POSTFIX_OP:{
			char* res = printPart(t->op.expr1);
			if(res != NULL) return res;
			printf("%s", t->op.op);
			printf("]");
		}
		case INFIX_OP:{
			char* res = printPart(t->op.expr1);
			if(res != NULL) return res;
			printf("%s", t->op.op);
			res = printPart(t->op.expr2);
			if(res != NULL) return res;
			printf("]");
		}
		default: return newString("Attempting to Print Invalid Template Op Type");
	}
	printf(":");
	char* res = printType(t->op.t);
	if(res != NULL) return res;
	printf("{\n");
	printf("	prec: %d\n", t->op.prec);
	if(t->op.ass) printf("	ass: right\n");
	else printf("	ass: left\n");
	printf("}\n");
	return NULL;
}

char* printFormTemplate(Template* t){
	printf("FormTemplate[");
	for(int i = 0; i < t->form.formSize; i++){
		char* res = printPart(t->form.parts[i]);
		printf(" ");
		if(res != NULL) return res;
	}
	printf("]\n");
	return NULL;
}

char* printTemplate(Template* t){
	if(t == NULL) return newString("Can't print null template");
	switch(t->type){
		case OP_TEMPLATE: return printOPTemplate(t);
		case FORM_TEMPLATE: return printFormTemplate(t);
		default: return newString("Attempting to Print Invalid Template Type\n");
	}
}

char* freePart(TemplatePart* part){
	return NULL;
}

char* freeOPTemplate(Template* t){
	if(t->op.op == NULL) return newString("Attempting to Free Null Op in Operator Template");
	free(t->op.op);
	switch(t->op.type){
		case PREFIX_OP:{
			char* res = freePart(t->op.expr1);
			if(res != NULL) return res;
		}
		case POSTFIX_OP:{
			char* res = freePart(t->op.expr1);
			if(res != NULL) return res;
		}
		case INFIX_OP:{
			char* res = freePart(t->op.expr1);
			if(res != NULL) return res;
			res = freePart(t->op.expr2);
			if(res != NULL) return res;
		}
		default: return newString("Attempting to Free Invalid Template Op Type");
	}
	char* res = freeType(t->op.t);
	if(res != NULL) return res;
	return NULL;
}

char* freeFormTemplate(Template* t){
	for(int i = 0; i < t->form.formSize; i++){
		char* res = freePart(t->form.parts[i]);
		if(res != NULL) return res;
	}
	return NULL;
}

char* freeTemplate(Template* t){
	switch(t->type){
		case OP_TEMPLATE:{
			char* res = freeOPTemplate(t);
			if(res != NULL) return res;
			return NULL;
		}
		case FORM_TEMPLATE:{
			char* res = freeFormTemplate(t);
			if(res != NULL) return res;
			return NULL;
		}
		default: return newString("Attempting to Free Invalid Template Type");
	}
}

TemplatePart* newTemplatePart(TemplateType type, char* id, Type* t){
	TemplatePart* part = malloc(sizeof(TemplatePart));
	part->type = type;
	part->id = id;
	part->t = t;
	return part;
}

TemplatePart* newExpressionPart(char* id, Type* t){
	return newTemplatePart(EXPR_PART, id, t);
}

TemplatePart* newBlockPart(char* id, Type*t){
	return newTemplatePart(BLOCK_PART, id, t);
}

TemplatePart* newWordPart(char* id){
	return newTemplatePart(WORD_PART, id, NULL);
}

TemplatePart* newStatementPart(char* id, Type*t){
	return newTemplatePart(STATEMENT_PART, id, t);
}

TemplatePart* newIDPart(char* id, Type*t){
	return newTemplatePart(ID_PART, id, t);
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

TemplatePart** newFormArr(int formSize){
	TemplatePart** parts = malloc(sizeof(TemplatePart*)*formSize);
	return parts;
}

Template* newFormTemplate(TemplatePart** parts, int formSize, Type* t){
	Template* formTemplate = (Template*)malloc(sizeof(Template));
	formTemplate->type = FORM_TEMPLATE;
	formTemplate->form.parts = parts;
	formTemplate->form.formSize = formSize;
	formTemplate->form.t = t;
	return formTemplate;
}