#ifndef COMMON_H_ 
#define COMMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

typedef enum{
	PLUS_OP,
	SUB_OP,
	MULT_OP,
	DIV_OP,
	REM_OP,
	
	EXP_OP,
	
	FACT_OP,
	
	AND_OP,
	OR_OP,
	NOT_OP,
	
	UNARY_PLUS_OP,
	UNARY_MINUS_OP,
	
	EQUAL_OP,
	
	LESS_OP,
	GREATER_OP,
	LOE_OP,
	GOE_OP,
	
	CALL_OP,
	
	ID_OP,
	
	VAL_OP,
	
	PRINT_OP,
	
	HALT_OP,
	POP_OP,
	
}OPCode;

typedef enum {
	I32_VAL,
	F32_VAL,
	BOOL_VAL,
	HEAP_VAL,
} ValueType;

typedef enum {
	HEAP_STRING,
} HeapType;

typedef struct {
	HeapType type;
} HeapValue;

typedef struct {
	union {
		int i32;
		float f32;
		bool boolean;
		HeapValue* heapVal;
	};
	ValueType type;
}Value;


#endif