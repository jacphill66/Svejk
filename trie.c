#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

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
	while(count < strlen(str)){
		n->child = newTrieNode(str[count], false);
		n = n->child;
		count += 1;
	}
	n->validEnd = true;
	return n;
}

//done...
TrieNode* findPlace(TrieNode* head, char c){
	//return the node whose next will be where the new string goes, if it returns the head, that's a special case I need to handle
	TrieNode* t = head;
	while(t->next != NULL && t->next->letter < c){
		t = t->next;
	}
	return t;
}

//what if it already exists? check it ...


TrieNode* addStringNode(TrieNode* n, char* str, int index){	
	//check the order of the first two ifs
	if (n == NULL){
		return addRest(str, index);
	}
	else if(index > strlen(str){
		n->validEnd = true;
		return n;
	}
	else{
		//work on this it's a bit messy...
		TrieNode* t = n;//was n->child;
		if(!(t->letter < str[index])){
			TrieNode* newNode = newTrieNode(str[index], false);
			if(index+1 < strlen(str)) addRest(str, index+1);
			else t->validEnd = true;
		}
		else{
			while(t->next != NULL && t->next->letter < str[index]) t = t->next;//find the place		
			if(t->letter == str[index]){
				t->children = addStringNode(t->children, str, index+1);
			}
			else{
				t->next = newTrieNode(str[index], false);
				if(index+1 < strlen(str)) addRest(str, index+1);
				else t->validEnd = true;
			}
		}
		return t;
	}
}


void addString(Trie* t, char* str){
	if(t->root == NULL){
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

int main(){

}