#include "Table.h"


//add a value to a table
//remove a value from a table
//initialize a table
//get a value from a table
//add all of the entries from one table to another
//hash function



//DONE:
//table structure
//entry structure


Table* newTable(long size){
	Table* t = (Table*)reallocate(NULL, 0, sizeof(Table));
	t->entries = (Entry*)reallocate(NULL, 0, sizeof(Entry)*size);
	t->cappacity = size;
	t->size = size;
	return t;
}

void insertSubEntry(SubEntry* entry, Value v){
	/*if(entry->entry == NULL){
		SubEntry e = {NULL, v};
		entry->entry = &e;
		return;
	}
	insertSubEntry(entry->entry, v);*/
}

void insert(Table* table, HeapString* key, Value v){
	int index = key->hash % table->size;
	if(table->entries[index].entries == NULL){
		//SubEntry entry = ;
		//table->entries[index].entries = struct SubEntry{NULL, v};
	}
	else{
		insertSubEntry(table->entries[index].entries, v);
	}
}
//;


uint8_t hash(const char* key, int length){
	//https://craftinginterpreters.com/hash-tables.html
	//https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
    uint8_t hash = 2166136261u;
	for(int i = 0; i < length; i++){
		hash ^= (uint8_t)key[i];
		hash *= 16777619;
	}
    return hash;
}

void freeSubEntry(SubEntry* subEntry){
	if(subEntry->entry == NULL){
		free(subEntry);
		return;
	}
	free(subEntry->entry);
}
//;

void freeEntry(Entry* entry){
	freeString(entry->key);
	freeSubEntry(entry->entries);
	free(entry);
}

void freeTable(Table* table){
	for(int i = 0; i < table->size; i++){
		freeEntry(&table->entries[i]);
		free(&table->entries[i]);
	}
	free(table);
}
