#include "Common.h"

void freeType(Type* t){
	if(t != NULL){
		switch(t->kind){
			case Trivial_KIND:{
				free(t);
				break;
			}
			default:{
				printf("Invalid Kind\n");
				exit(1);
				break;
			}
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
