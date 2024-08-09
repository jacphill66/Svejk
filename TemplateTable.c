#include "TemplateTable.h"
TemplateTable* newTemplateTable(int length){
	TemplateTable* t = (TemplateTable*)malloc(sizeof(TemplateTable));
	t->length = length;
	t->elements = (TMNode**) malloc(sizeof(TMNode*) * length);
	for(int i = 0; i < t->length; i++) t->elements[i] = NULL;
	t->size = 0;
	return t;
}

TMNode* getTemplateTable(TemplateTable* t, char* key){
	return t->elements[hash(key)%t->length];
}

TMNode* newTMNode(char* key, Template* template){
	TMNode* n = (TMNode*)malloc(sizeof(TMNode));
	n->t = template;
	n->next = NULL;
	return n;
}

void setTemplateTable(TemplateTable* t, char* key, int keySize, Template* template){
	TMNode* n = t->elements[hash(key)%t->length];
	if(n == NULL) t->elements[hash(key)%t->length] = newTMNode(key, template);
	else{
		while(n->next != NULL) n = n->next;
		n->next = newTMNode(key, template);
	}
	t->size += 1;
}

void printTMNode(TMNode* n){
	while(n!=NULL){
		printTemplate(n->t);
		n = n->next;
	}
	printf("\n");
}

void printTemplateTable(TemplateTable* t){
	for(int i = 0; i < t->length; i++) printTMNode(t->elements[i]);
}

void freeTMNode(TMNode* n){
	if(n != NULL) freeTMNode(n->next);
	free(n);
}

void freeTemplateTable(TemplateTable* t){
	for(int i = 0; i < t->length; i++) freeTMNode(t->elements[i]);
	free(t->elements);
}