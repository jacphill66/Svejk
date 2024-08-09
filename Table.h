#ifndef TABLE_H_ 
#define TABLE_H_
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "RedBlackTree.h"


typedef struct{
	RedBlackTree** elements;
	long length;
	long size;
} Table;

Table* newTable(int size);
Value get(Table* t, char* key);
void set(Table* t, char* key, int keySize, Value value);
void printTable(Table* t);
void freeTable(Table* t);
unsigned long hash(unsigned char *str);
#endif