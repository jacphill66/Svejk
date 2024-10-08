#include "Trie.h"

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

bool searchForStringNode(TrieNode* n, char* str, int index){
	if(n == NULL) return false;
	else if((n->letter == str[index]) && (index == strlen(str)-1) && n->validEnd) return true;
	else if(n->letter == str[index]) return searchForStringNode(n->child, str, index+1);
	else if(n->next != NULL) return searchForStringNode(n->next, str, index);
	else return false;
}

bool searchForString(Trie* t, char* str){
	return searchForStringNode(t->root->child, str, 0);
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
	if(t->child!=NULL) freeTrieNode(t->child);
	if(t->next!=NULL) freeTrieNode(t->next);
	free(t);
}

void freeTrie(Trie* t){
	freeTrieNode(t->root);
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

void printSearch(Trie* t, char* str){
	if(searchForString(t, str)) printf("%s is contained in the trie!\n", str);
	else printf("%s isn't contained in trie! Perhaps you should trie again.\n", str);
}
*/