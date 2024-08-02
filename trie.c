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

TrieNode* addStringNode(TrieNode* n, char* str, int index){	
	//check the order of the first two ifs
	if (n == NULL){
		return addRest(str, index);
	}
	else if(index > strlen(str)){
		n->validEnd = true;
		return n;
	}
	else if(!(n->letter < str[index])){
		TrieNode* newNode = newTrieNode(str[index], false);
		if(index+1 < strlen(str)) newNode->child = addRest(str, index+1);
		else newNode->validEnd = true;
		newNode->next = n;
		return newNode;
	}
	else{
		TrieNode* t = n;
		//find the place
		while(t->next != NULL && t->next->letter < str[index]) t = t->next;	
		if(t->letter == str[index]){
			t->child = addStringNode(t->child, str, index+1);
		}
		else{
			t->next = newTrieNode(str[index], false);
			if(index+1 < strlen(str)) t->next->child = addRest(str, index+1);
			else t->validEnd = true;
		}
		return t;
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

void addToken(char** tokens, int tokenCount, const char* token){
	tokens[tokenCount] = (char*)malloc(sizeof(char)*strlen(token));
	for(int i = 0; i < strlen(token); i++) tokens[tokenCount][i] = token[i];
}

//add check for when it already exists
int main(){
	int numberOfTokens = 100;
	char** tokens = (char**)malloc(sizeof(char*)*numberOfTokens);
	addToken(tokens, 0, "and");
	addToken(tokens, 1, "ant");
	addToken(tokens, 2, "dad");
	addToken(tokens, 3, "do");	
	Trie* t = newTrie();
	addString(t, tokens[2]);
	addString(t, tokens[3]);

	printf("%c\n", t->root->child->letter);
	printf("%c\n", t->root->child->child->letter);
	printf("%c\n", t->root->child->child->child->letter);
}