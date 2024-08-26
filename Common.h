#ifndef COMMON_H_ 
#define COMMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

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
	VAL_OP,
	STR_VAL_OP,
	PRINT_OP,
	HALT_OP,
	POP_OP,
	SET_GLOBAL_VAR_OP,
	GET_GLOBAL_VAR_OP,
	SET_LOCAL_VAR_OP,
	GET_LOCAL_VAR_OP,
	JMP_FORWARD_OP,
	JMP_BACK_OP,
	JMP_ON_FALSE_OP,
	DUMP_OP,
}OPCode;


typedef enum {
	I32_VAL,
	F32_VAL,
	STR_VAL,
	BOOL_VAL,
	HEAP_VAL,
} ValueType;

typedef enum {
	HEAP_STRING,
} HeapType;

typedef struct {
	HeapType type;
} HeapValue;

typedef enum{
	INTERNAL_ERROR_TYPE,
	I32_TYPE,
	F32_TYPE,
	STR_TYPE,
	BOOL_TYPE,
	INFERRED_TYPE,
	UNDECLARED_ERROR_TYPE,
	REDECLARATION_ERROR_TYPE,
	TYPE_MISMATCH_ERROR_TYPE,
	VOID_TYPE,
}TrivialType;

typedef enum{
	Trivial_KIND,
}TypeKind;

typedef struct {
	union {
		TrivialType trivial;
	};
	TypeKind kind;
} Type;

typedef struct {
	union {
		int i32;
		float f32;
		bool boolean;
		HeapValue* heapVal;
	};
	ValueType type;
}Value;

char* newString(const char* const_str);

char* printValue(Value v);

char* printType(Type* t);
char* freeType(Type* t);

Value newI32(int i);
Value newBool(bool b);
Value newStr(int i);
Value newF32(float f32);
TrivialType getTrivialType(Type* t);
Type* newTrivialType(TrivialType type);

int panic(const char* msg);
bool strComp(const char* c1, const char* c2);
#endif