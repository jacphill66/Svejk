#include "Memory.h"
#include "Common.h"
#include <string.h>
void* reallocate(void* ptr, size_t oldSize, size_t newSize){
	if(newSize == 0){
		free(ptr);
	}
	void* newPtr = realloc(ptr, newSize);
	if(newPtr == NULL){
		printf("Could not reallocate");
		exit(1);
	}
	
	return newPtr;
}

char* copyString(char* str){
	char* str2;
	strcpy(str2, str);
	return str2;
}