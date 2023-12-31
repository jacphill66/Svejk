#ifndef Table_H_ 
#define Table_H_
#include "Common.h"
#include "Memory.h"
#include "Heaping.h"
#include "Strings.h"

typedef struct {
	struct SubEntry* entry;	
	Value v;
} SubEntry;

typedef struct {
	HeapString* key;
	SubEntry* entries;
	long entryCount;
	long cappacity;
}Entry;

typedef struct{
	Entry* entries;
	long size;
	long cappacity;
}Table;

Table* newTable(long size);
Value getEntry(Table* table, HeapString* key);
Table* mergeTables();



void freeSubEntry(SubEntry* subEntry);
void freeEntry(Entry* entry);
void freeTable(Table* table);


#endif