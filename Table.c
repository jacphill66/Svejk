#include "Table.h"

Table* newTable(int length){
	Table* t = (Table*)malloc(sizeof(Table));
	t->length = length;
	t->elements = (RedBlackTree**) malloc(sizeof(RedBlackTree*) * length);
	for(int i = 0; i < t->length; i++) t->elements[i] = newTree();
	t->size = 0;
	return t;
}

//https://stackoverflow.com/questions/7666509/hash-function-for-string
//djb2 by Dan Bernstein.
unsigned long hash(unsigned char *str){
    unsigned long hash = 5381;
    int c;
    while (c = *str++){
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	}
    return hash;
}

Value get(Table* t, char* key){
	return search(t->elements[hash(key)%t->length], key);
}

void set(Table* t, char* key, int keySize, Value value){
	int index = hash(key)%t->length;
	insert(t->elements[index], key, keySize, value);
	t->size += 1;
}

void printTable(Table* t){
	for(int i = 0; i < t->length; i++) printTree(t->elements[i]);
}

void freeTable(Table* t){
	for(int i = 0; i < t->length; i++) freeTreeNoKeys(t->elements[i]);
	free(t->elements);
}