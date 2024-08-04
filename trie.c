#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
typedef struct TrieNode TrieNode;

struct TrieNode{
	char letter;
	bool validEnd;
	TrieNode* next;
	//might add prev;
	TrieNode* child;
};

typedef struct{
	TrieNode* root;
}Trie;

TrieNode* newTrieNode(char c, bool validEnd){
	TrieNode* n = (TrieNode*)malloc(sizeof(TrieNode));
	n->letter = c;
	n->validEnd = NULL;
	n->next = NULL;
	n->child = NULL;
	return n;
}

Trie* newTrie(){
	Trie* t = (Trie*)malloc(sizeof(Trie));
	t->root = NULL;
	return t;
}

TrieNode* addRest(char* str, int index){
	//if and when NULL is hit, this adds the rest of a string
	TrieNode* n = newTrieNode(str[index], false);
	int count = index+1;
	TrieNode* t = n;
	while(count < strlen(str)){
		t->child = newTrieNode(str[count], false);
		t = t->child;
		count += 1;
	}
	t->validEnd = true;
	return n;
}

void printDown(TrieNode* n){
	while(n != NULL){
		printf("%c, Next:", n->letter);
		if(n->next == NULL) printf("NULL");
		else printf("%c", n->next->letter);
		printf(", Valid End: ");
		if(n->validEnd) printf("True\n");
		else printf("False\n");
		//if(n->next != NULL) printf("Error!");
		n = n->child;
	}
}

bool searchForStringNode(TrieNode* n, char* str, int index){
	if(n == NULL) return false;
	else if((n->letter == str[index]) && (index == strlen(str)-1) && n->validEnd) return true;
	else if(n->letter == str[index]) return searchForStringNode(n->child, str, index+1);
	else if(n->next != NULL) return searchForStringNode(n->next, str, index);
	else return false;
}

bool searchForString(Trie* t, char* str){
	searchForStringNode(t->root->child, str, 0);
}

void printSearch(Trie* t, char* str){
	if(searchForString(t, str)) printf("%s is contained in the trie!\n", str);
	else printf("%s isn't contained in trie! Perhaps you should trie again.\n", str);
}

TrieNode* addStringNode(TrieNode* n, char* str, int index){	
	if(index >= strlen(str)){
		return n;
	}
	else if (n == NULL){
		return addRest(str, index);
	}
	else if(n->letter > str[index]){
		TrieNode* newNode = newTrieNode(str[index], false);
		if(index+1 < strlen(str)) newNode->child = addRest(str, index+1);
		else newNode->validEnd = true;
		newNode->next = n;
		return newNode;
	}
	else{
		TrieNode* t = n;
		while(t->next != NULL && t->next->letter <= str[index]) t = t->next;	
		if(t->letter == str[index]){
			t->child = addStringNode(t->child, str, index+1);
			if(index+1 >= strlen(str)) t->validEnd = true;
			return n;
		}
		else{
			TrieNode* t2 = t->next;
			t->next = newTrieNode(str[index], false);
			if(index+1 < strlen(str)) t->next->child = addRest(str, index+1);
			else t->next->validEnd = true;
			t->next->next = t2;
			return n;
		}
	}
}


void addString(Trie* t, char* str){
	if(t->root == NULL){
		t->root = (TrieNode*)malloc(sizeof(TrieNode));
		t->root->letter = '\0';
		t->root->validEnd = false;
		t->root->next = NULL;
		t->root->child = addRest(str, 0);
	}
	else t->root->child = addStringNode(t->root->child, str, 0);
}


void freeTrieNode(TrieNode* t){
	//don't free key -- it will be used later
	if(t->child!=NULL) freeTrieNode(t->child);
	if(t->next!=NULL) freeTrieNode(t->next);
	free(t);
}

void freeTrie(Trie* t){
	freeTrieNode(t->root);
}

void addToken(char** tokens, int tokenCount, const char* token){
	tokens[tokenCount] = (char*)malloc(sizeof(char)*strlen(token));
	for(int i = 0; i < strlen(token); i++) tokens[tokenCount][i] = token[i];
}

/*
void trieNodePrintString(TrieNode* n){
	//work on this...
	printf("%c", n->letter);
	TrieNode* t = n->child;
	while(t != NULL && t->validEnd != true){
		trieNodePrintString(t);
		t = t->next;
	}
	t->validEnd = false;
}

void triePrintStrings(Trie* t){
	trieNodePrintString(t->root->child);
}

void printTrie(Trie* trie){
	
}
*/



//add check for when it already exists
int main(){
	int numberOfTokens = 100;
	char** tokens = (char**)malloc(sizeof(char*)*numberOfTokens);
	addToken(tokens, 0, "<");
	addToken(tokens, 1, "<<");
	addToken(tokens, 2, "<<?");
	addToken(tokens, 3, "<<!");
	addToken(tokens, 4, "<<??");
	addToken(tokens, 5, "apples");
	addToken(tokens, 6, "sdfdsfsdf");
	addToken(tokens, 7, "asdposfd");
	addToken(tokens, 8, "qweerre");
	addToken(tokens, 9, "ewrer");
	addToken(tokens, 10, "wreerwerdf");
	addToken(tokens, 11, "appfdsfles");
	addToken(tokens, 12, "fgdappdfgfgles");
	addToken(tokens, 13, "applfdgdfes");
	addToken(tokens, 14, "appdgles");
	addToken(tokens, 15, "dsfdsfapples");
	addToken(tokens, 16, "appldffdses");

	Trie* t = newTrie();
	addString(t, tokens[3]);
	addString(t, tokens[2]);
	addString(t, tokens[4]);
	addString(t, tokens[0]);
	addString(t, tokens[1]);
	addString(t, tokens[5]);
	addString(t, tokens[16]);
	addString(t, tokens[7]);
	addString(t, tokens[13]);
	addString(t, tokens[14]);
	addString(t, tokens[8]);
	addString(t, tokens[9]);
	addString(t, tokens[12]);
	addString(t, tokens[6]);
	addString(t, tokens[15]);
	addString(t, tokens[10]);
	addString(t, tokens[11]);


	printf("valid strings\n");
	printSearch(t, "<");
	printSearch(t, "<<");
	printSearch(t, "<<?");
	printSearch(t, "<<!");
	printSearch(t, "<<??");
	printSearch(t, "apples");
	printSearch(t, tokens[3]);
	printSearch(t, tokens[2]);
	printSearch(t, tokens[4]);
	printSearch(t, tokens[0]);
	printSearch(t, tokens[1]);
	printSearch(t, tokens[5]);
	printSearch(t, tokens[16]);
	printSearch(t, tokens[7]);
	printSearch(t, tokens[13]);
	printSearch(t, tokens[14]);
	printSearch(t, tokens[8]);
	printSearch(t, tokens[9]);
	printSearch(t, tokens[12]);
	printSearch(t, tokens[6]);
	printSearch(t, tokens[15]);
	printSearch(t, tokens[10]);
	printSearch(t, tokens[11]);

	printf("invalid strings\n");
	printSearch(t, ">");
	printSearch(t, "a");
	printSearch(t, "?");
	printSearch(t, "b");
	printSearch(t, "cdef");
	printSearch(t, "<<?!");
	printSearch(t, "<<!?");
	printSearch(t, "a");
	printSearch(t, "a");
	printSearch(t, "d");
	
	freeTrie(t);
	printf("completed");
	printf("\n");
}