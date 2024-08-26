#include "Common.h"

char* newString(const char* const_str){
	char* dynamic_str = malloc(strlen(const_str) + 1);
	strcpy(dynamic_str, const_str);
	return dynamic_str;
}

bool strComp(const char* c1, const char* c2){
	return strcmp(c1, c2) == 0;
}

int panic(const char* msg){
	printf("PANIC:%s", msg);
	printf("\n");
	exit(1);
	return -1;
}

int printTrivialType(Type* t){
	switch(t->trivial){
		case INTERNAL_ERROR_TYPE: return printf("Internal Error Type");
		case I32_TYPE: return printf("I32 Type");
		case F32_TYPE: return printf("F32 Type");
		case STR_TYPE: return printf("Str Type");
		case BOOL_TYPE: return printf("Bool Type");
		case INFERRED_TYPE: return printf("Inferred Type");
		case UNDECLARED_ERROR_TYPE: return printf("Undeclared Error Type");
		case REDECLARATION_ERROR_TYPE: return printf("Redeclaration Error Type");
		case TYPE_MISMATCH_ERROR_TYPE: printf("Type Mismatch Error Type");
		case VOID_TYPE: return printf("Void Error Type");
		default: panic("Attempting to Print Invalid Trivial Type\n");
	}
	return -1;
}

char* printType(Type* t){
	switch(t->kind){
		case Trivial_KIND: {
			printTrivialType(t);
			return NULL;
		}
		default: {
			return newString("Attempting to Print Invalid Type");
		}
	}
}

char* freeType(Type* t){
	if(t != NULL){
		switch(t->kind){
			case Trivial_KIND: {
				free(t);
				return NULL;
			}
			default: return newString("Attempted to Free Invalid Kind");
		}
	}
}

Value newI32(int i){
	Value v;
	v.type = I32_VAL;
	v.i32 = i;
	return v;
}

Value newBool(bool b){
	Value v;
	v.type = BOOL_VAL;
	v.boolean = b;
	return v;
}

Value newStr(int i){
	Value v;
	v.type = STR_VAL;
	v.i32 = i;
	return v;
}

Value newF32(float f){
	Value v;
	v.type = F32_VAL;
	v.f32 = f;
	return v;
}

TrivialType getTrivialType(Type* t){
	if(t->kind == Trivial_KIND) return t->trivial;
	else return 0;
}

Type* newTrivialType(TrivialType type){
	Type* t = (Type*)malloc(sizeof(Type));
	t->kind = Trivial_KIND;
	t->trivial = type;
	return t;
}

char* printValue(Value v){
	switch(v.type){
		case I32_VAL: return printf("%d", v.i32),NULL;
		case F32_VAL: return printf("%f", v.f32),NULL;
		case BOOL_VAL: {
			if(v.boolean) return printf("true"),NULL;
			else return printf("false"),NULL;
		}
		default: return newString("Attempting to Print Invalid Value Type");
	}
}