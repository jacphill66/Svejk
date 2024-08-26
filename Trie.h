#ifndef TRIE_H_ 
#define TRIE_H_

#include "Common.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct TrieNode TrieNode;

struct TrieNode{
	char letter;
	bool validEnd;
	TrieNode* next;
	TrieNode* child;
};

typedef struct{
	TrieNode* root;
}Trie;

/*Initialization*/
TrieNode* newTrieNode(char c, bool validEnd);
Trie* newTrie();

/*Adding and Searching*/
void addString(Trie* t, char* str);
bool searchForString(Trie* t, char* str);

/*Freeing*/
void freeTrie(Trie* t);

#endif