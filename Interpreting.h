#ifndef Interpreting_H_ 
#define Interpreting_H_

#include "Common.h"
#include <math.h>
#include "Compiling.h"

#define STACK_MAX 256


typedef struct {
		Program p;
		OPCode* ip;
		Value stack[STACK_MAX];
		Value* stackPtr;
		//CallFrame[X] callStack;
		
		//CodeSegment data;
		//DataSegment datasegment;
		//HeapObject* heap; 
} VM;

void initVM(VM* vm);
void execute(VM* vm);

#endif