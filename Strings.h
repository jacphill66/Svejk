#ifndef Strings_H_ 
#define Strings_H_
#include "Common.h"

typedef struct {
	HeapValue heapVal;
	char* chars;
	long size;
	long cappacity;//for mutable strings
	uint8_t hash;
} HeapString;

void freeString(HeapString* str);

#endif