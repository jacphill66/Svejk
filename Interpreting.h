#ifndef Interpreting_H_ 
#define Interpreting_H_

#include "Common.h"
#include <math.h>
#include "Compiling.h"

#define STACK_MAX 256


typedef struct {
		Program* p;
		OPCode* ip;
		Value stack[STACK_MAX];
		Value* stackPtr;
		Value* globalVariables;
		char** strings; 
		//CallFrame[X] callStack;
		
		//CodeSegment data;
		//DataSegment datasegment;
		//HeapObject* heap; 
} VM;

VM* initVM(Program* p);
void execute(VM* vm);

#endif