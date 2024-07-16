#include "TypeMap.h"

void freeSingleTypeTreeNodeNoKey(TypeTreeNode* node){
	free(node);
}

void freeTypeTreeNodeNoKeys(TypeTree* tree, TypeTreeNode* node){
	if(node == tree->nil) return;
	freeTypeTreeNodeNoKeys(tree, node->left);
	freeTypeTreeNodeNoKeys(tree, node->right);
	freeSingleTypeTreeNodeNoKey(node);
}

void freeTypeTreeNoKeys(TypeTree* tree){
	freeTypeTreeNodeNoKeys(tree, tree->root);
}

TypeTreeNode* newTypeTreeNode(TypeTreeNode* parent, TypeTreeNode* left, TypeTreeNode* right, char* key, Type* t, Color c){
	TypeTreeNode* n = (TypeTreeNode*) malloc(sizeof(TypeTreeNode));
	n->parent = parent;
	n->left = left;
	n->right = right;
	n->key = key;
	n->t = t;
	n->c = c;
	return n;
}

/*Helper Functions*/
TypeTree* newTypeTree(){
	TypeTree* tree = (TypeTree*) malloc(sizeof(TypeTree));
	Type* t = NULL;
	tree->nil = newTypeTreeNode(NULL, NULL, NULL, NULL, t, BLACK);
	tree->root = tree->nil;
	return tree;
}

/*Searching*/
TypeTreeNode* findTypeTreeNode(TypeTree* tree, char* key){
	TypeTreeNode* n = tree->root; 
	while(n != tree->nil && key != n->key){
		if(strcmp(key, n->key) < 0) n = n->left;
		else n = n->right;
	}
	return n;
}

Type* searchTypeTree(TypeTree* tree, char* key){
	TypeTreeNode* n = tree->root; 
	while(n != tree->nil && strcmp(key, n->key) != 0){
		if(strcmp(key, n->key)<0) n = n->left;
		else n = n->right;
	}
	if(n == tree->nil){
		return NULL;
	}
	return n->t;
}

void leftRotateTypeTree(TypeTree* tree, TypeTreeNode* n){
	TypeTreeNode* y = n->right;
	n->right = y->left;
	if(y->left != tree->nil){
		y->left->parent = n;
	}
	y->parent = n->parent;
	if(n->parent == tree->nil){
		tree->root = y;
	}
	else if(n == n->parent->left){
		n->parent->left = y;
	}
	else n->parent->right = y;
	y->left = n;
	n->parent = y;
}

void rightRotateTypeTree(TypeTree* tree, TypeTreeNode* n){
	TypeTreeNode* y = n->left;
	n->left = y->right;
	if(y->right != tree->nil){
		y->right->parent = n;
	}
	y->parent = n->parent;
	if(n->parent == tree->nil){
		tree->root = y;
	}
	else if(n == n->parent->right){
		n->parent->right = y;
	}
	else n->parent->left = y;
	y->right = n;
	n->parent = y;
}

//z->parent

void fixInsertTypeTree(TypeTree* tree, TypeTreeNode* z){
	while(z->parent->c == RED){
		if(z->parent == z->parent->parent->left){
			TypeTreeNode* y = z->parent->parent->right;
			if(y->c == RED){
				z->parent->c = BLACK;
				y->c = BLACK;
				z->parent->parent->c = RED;
				z = z->parent->parent;
			}
			else{
				if(z == z->parent->right){
					z = z->parent;
					leftRotateTypeTree(tree, z);
				}
				z->parent->c = BLACK;
				z->parent->parent->c = RED;
				rightRotateTypeTree(tree, z->parent->parent);
			}
		}
		else{
			TypeTreeNode* y = z->parent->parent->left;
			if(y->c == RED){
				z->parent->c = BLACK;
				y->c = BLACK;
				z->parent->parent->c = RED;
				z = z->parent->parent;
			}
			else{
				if(z == z->parent->left){
					z = z->parent;
					rightRotateTypeTree(tree, z);
				}
				z->parent->c = BLACK;
				z->parent->parent->c = RED;
				leftRotateTypeTree(tree, z->parent->parent);
			}
		}
	}
	tree->root->c = BLACK;
}

void insertTypeTree(TypeTree* tree, char* key, Type* t){
	tree->size += 1;
	TypeTreeNode* z = newTypeTreeNode(tree->nil, tree->nil, tree->nil, key, t, -1);
	TypeTreeNode* x = tree->root;
	TypeTreeNode* y = tree->nil;
	while(x != tree->nil){
		y = x;
		if(strcmp(z->key, x->key) < 0) x = x->left;		
		else x = x->right;
	}

	z->parent = y;
	if(y == tree->nil) tree->root = z;
	else if(strcmp(z->key, y->key) < 0) y->left = z;
	else y->right = z;
	z->left = tree->nil;
	z->right = tree->nil;
	z->c = RED;
	fixInsertTypeTree(tree, z);
}

void transplantTypeTree(TypeTree* tree, TypeTreeNode* n1, TypeTreeNode* n2){
	if(n1->parent == tree->nil) tree->root = n2;
	else if(n1 == n1->parent->left) n1->parent->left = n2;
	else n1->parent->right = n2;
	n2->parent = n1->parent;
}

TypeTreeNode* treeMinimumTypeTree(TypeTree* tree, TypeTreeNode* n){
	while(n->left != tree->nil){
		n = n->left;
	}
	return n;
}

TypeTreeNode* treeMaximumTypeTree(TypeTree* tree, TypeTreeNode* n){
	while(n->right != tree->nil){
		n = n->right;
	}
	return n;
}

void fixDeleteTypeTree(TypeTree* tree, TypeTreeNode* n){
	while(n != tree->root && n->c == BLACK){
		if(n==n->parent->left){
			TypeTreeNode* w = n->parent->right;
			if(w->c == RED){
				w->c = BLACK;
				n->parent->c = RED;
				leftRotateTypeTree(tree, n->parent);
				w = n->parent->right;
			}
			if(w->left->c == BLACK && w->right->c == BLACK){
				w->c = RED;
				n = n->parent;
			}
			else{
				if(w->right->c == BLACK){
					w->left->c = BLACK;
					w->c = RED;
					rightRotateTypeTree(tree, w);
					w = n->parent->right;
				}
				w->c = n->parent->c;
				n->parent->c = BLACK;
				w->right->c = BLACK;
				leftRotateTypeTree(tree, n->parent);
				n = tree->root;
			}
		}
		else{
			TypeTreeNode* w = n->parent->left;
			if(w->c == RED){
				w->c = BLACK;
				n->parent->c = RED;
				rightRotateTypeTree(tree, n->parent);
				w = n->parent->left;
			}
			if(w->right->c == BLACK && w->left->c == BLACK){
				w->c = RED;
				n = n->parent;
			}
			else{
				if(w->left->c == BLACK){
					w->right->c = BLACK;
					w->c = RED;
					leftRotateTypeTree(tree, w);
					w = n->parent->left;
				}
				w->c = n->parent->c;
				n->parent->c = BLACK;
				w->left->c = BLACK;
				rightRotateTypeTree(tree, n->parent);
				n = tree->root;
			}
		}
	}
}

void deleteTypeTree(TypeTree* tree, char* key){
	tree->size -= 1;
	TypeTreeNode* n = findTypeTreeNode(tree, key);	
	TypeTreeNode* y = n;
	TypeTreeNode* x = NULL;
	Color originalColor = y->c;
	if(n->left == tree->nil){
		x = n->right;
		transplantTypeTree(tree, n, n->right);
	}
	else if(n->right == tree->nil){
		x = n->left;
		transplantTypeTree(tree, n, n->left);
	}
	else{
		y = treeMinimumTypeTree(tree, n->right);
		originalColor = y->c;
		x = y->right;
		if(y != n->right){
			transplantTypeTree(tree, y, y->right);
			y->right = n->right;
			y->right->parent = y;
		}
		else{
			x->parent = y;
			transplantTypeTree(tree, n, y);
			y->left = n->left;
			y->left->parent = y;
			y->c = n->c;
		}
	}
	if(originalColor==BLACK){
		fixDeleteTypeTree(tree, x);
	}
	free(n->key);
	free(n);
}

bool containsTypeTree(TypeTree* tree, char* key){
	TypeTreeNode* n = tree->root; 
	while(n != tree->nil && strcmp(key, n->key) != 0){
		if(strcmp(key, n->key)<0) n = n->left;
		else n = n->right;
	}
	if(n == tree->nil) return false;
	return strcmp(key, n->key)==0;
}

TypeMap* newTypeMap(int length){
	TypeMap* t = (TypeMap*)malloc(sizeof(TypeMap));
	t->length = length;
	t->elements = (TypeTree**) malloc(sizeof(TypeTree*) * length);
	for(int i = 0; i < t->length; i++) t->elements[i] = newTypeTree();
	t->size = 0;
	return t;
}

//https://stackoverflow.com/questions/7666509/hash-function-for-string
//djb2 by Dan Bernstein.
unsigned long hashTypeMap(unsigned char *str){
    unsigned long hash = 5381;
    int c;
    while (c = *str++){
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	}
    return hash;
}

Type* getTypeMap(TypeMap* t, char* key){
	return searchTypeTree(t->elements[hashTypeMap(key)%t->length], key);
}

void setTypeMap(TypeMap* t, char* key, Type* type){
	int index = hashTypeMap(key)%t->length;
	insertTypeTree(t->elements[index], key, type);
	t->size += 1;
}

void freeTypeMap(TypeMap* t){
	for(int i = 0; i < t->length; i++) freeTypeTreeNoKeys(t->elements[i]);
	free(t->elements);
}

TypeScopeNode* newTypeScopeNode(TypeTree* tree, TypeScopeNode* next, TypeScopeNode* prev){
	TypeScopeNode* n = (TypeScopeNode*)malloc(sizeof(TypeScopeNode));
	n->scope = tree;
	n->next = next;
	n->prev = prev;
	return n;
}

TypeScopeChain* newTypeScopeChain(){
	TypeScopeChain* sc = (TypeScopeChain*)malloc(sizeof(TypeScopeChain));
	sc->head = NULL;
	sc->tail = NULL;
	return sc;
}

void newTypeScope(TypeScopeChain* sc){
	if(sc->head == NULL){
		sc->head = newTypeScopeNode(newTypeTree(), NULL, NULL);
		sc->tail = sc->head;
	}
	else{
		sc->tail->next = newTypeScopeNode(newTypeTree(), NULL, sc->tail);
		sc->tail = sc->tail->next;
	}
}

void closeTypeScope(TypeScopeChain* sc){
	if(sc->tail == NULL) return;
	else{
		freeTypeTreeNoKeys(sc->tail->scope);
		TypeScopeNode* t = sc->tail->prev;
		free(sc->tail);
		sc->tail = t;
		if(sc->tail == NULL){
			sc->head = NULL;
		}
		if(sc->tail!=NULL) sc->tail->next = NULL;
	}
}

void addToCurrentTypeScope(TypeScopeChain* sc, char* id, Type* t){
	insertTypeTree(sc->tail->scope, id, t);
}

Type* searchTypeScope(TypeScopeNode* n, char* id){
	if(containsTypeTree(n->scope, id)) return searchTypeTree(n->scope, id);
	return NULL;
}

Type* searchTypeScopes(TypeScopeChain* sc, char* id){
	TypeScopeNode* tail = sc->tail;
	while(tail != NULL){
		Type* search = searchTypeScope(tail, id);
		if(search != NULL) return search;
		tail = tail->prev;
	}
	return NULL;
}
