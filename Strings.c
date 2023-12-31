#include "Strings.h"
#include "Memory.h"
#include "Heaping.h"
#include "Memory.h"
#include <string.h>

HeapString* createString(char* characters, long length){
	//\0 is included by the lexer
	char* chars = reallocate(NULL, 0, length*sizeof(char));
	memcpy(chars, characters, length);
	HeapString* str = (HeapString*)allocateHeapValue(sizeof(HeapString), HEAP_STRING);
	//HeapValue* hV = reallocate(NULL, 0, sizeof(HeapValue));
	str->size = length;
	str->cappacity = length;
	return str;
}

void freeString(HeapString* str){
	free(str->chars);
	free(str);
}