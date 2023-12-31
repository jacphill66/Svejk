#include "Heaping.h"
#include "Memory.h"

HeapValue* allocateHeapValue(size_t size, HeapType type){
	HeapValue* v = (HeapValue*) reallocate(NULL, 0, sizeof(type));
	v->type = type;
	return v;
}