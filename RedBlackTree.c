#include "RedBlackTree.h"

/*Printing*/
void printNode(RBNode* node, RBNode* nil){
	if(node == nil) return;
	printf("(");
	printNode(node->left, nil);
	printf(") %s:%d (", node->key, node->value.i32);
	printNode(node->right, nil);
	printf(")");
}

void printTree(RedBlackTree* tree){
	printNode(tree->root, tree->nil);
	printf("\n");
}

/*Freeing*/
void freeSingleNode(RBNode* node){
	free(node->key); //Why doesn't the key need to be freed?
	free(node);
}

void freeNode(RedBlackTree* tree, RBNode* node){
	if(node == tree->nil) return;
	freeNode(tree, node->left);
	freeNode(tree, node->right);//tree->
	freeSingleNode(node);
}

void freeTree(RedBlackTree* tree){
	freeNode(tree, tree->root);
}

void freeSingleNodeNoKey(RBNode* node){
	free(node);
}

void freeNodeNoKeys(RedBlackTree* tree, RBNode* node){
	if(node == tree->nil) return;
	freeNodeNoKeys(tree, node->left);
	freeNodeNoKeys(tree, node->right);
	freeSingleNodeNoKey(node);
}

void freeTreeNoKeys(RedBlackTree* tree){
	freeNodeNoKeys(tree, tree->root);
}

RBNode* newNode(RBNode* parent, RBNode* left, RBNode* right, char* key, int keySize, Value value, Color c){
	RBNode* n = (RBNode*) malloc(sizeof(RBNode));
	n->parent = parent;
	n->left = left;
	n->right = right;
	n->key = key;
	n->keySize = keySize;
	n->value = value;
	n->c = c;
	return n;
}

/*Helper Functions*/
RedBlackTree* newTree(){
	RedBlackTree* tree = (RedBlackTree*) malloc(sizeof(RedBlackTree));
	Value v;
	v.i32 = -1;
	tree->nil = newNode(NULL, NULL, NULL, NULL, -1, v, BLACK);
	tree->root = tree->nil;
	return tree;
}

/*Searching*/
RBNode* findNode(RedBlackTree* tree, char* key){
	RBNode* n = tree->root; 
	while(n != tree->nil && key != n->key){
		if(strcmp(key, n->key) < 0) n = n->left;
		else n = n->right;
	}
	return n;
}

Value search(RedBlackTree* tree, char* key){
	RBNode* n = tree->root; 
	while(n != tree->nil && strcmp(key, n->key) != 0){
		if(strcmp(key, n->key)<0) n = n->left;
		else n = n->right;
	}
	if(n == tree->nil){
		Value v;
		v.i32 = -1;
		return v;
	}
	return n->value;
}

void leftRotate(RedBlackTree* tree, RBNode* n){
	RBNode* y = n->right;
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

void rightRotate(RedBlackTree* tree, RBNode* n){
	RBNode* y = n->left;
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

void fixInsert(RedBlackTree* tree, RBNode* z){
	while(z->parent->c == RED){
		if(z->parent == z->parent->parent->left){
			RBNode* y = z->parent->parent->right;
			if(y->c == RED){
				z->parent->c = BLACK;
				y->c = BLACK;
				z->parent->parent->c = RED;
				z = z->parent->parent;
			}
			else{
				if(z == z->parent->right){
					z = z->parent;
					leftRotate(tree, z);
				}
				z->parent->c = BLACK;
				z->parent->parent->c = RED;
				rightRotate(tree, z->parent->parent);
			}
		}
		else{
			RBNode* y = z->parent->parent->left;
			if(y->c == RED){
				z->parent->c = BLACK;
				y->c = BLACK;
				z->parent->parent->c = RED;
				z = z->parent->parent;
			}
			else{
				if(z == z->parent->left){
					z = z->parent;
					rightRotate(tree, z);
				}
				z->parent->c = BLACK;
				z->parent->parent->c = RED;
				leftRotate(tree, z->parent->parent);
			}
		}
	}
	tree->root->c = BLACK;
}

void insert(RedBlackTree* tree, char* key, int keySize, Value value){
	tree->size += 1;
	RBNode* z = newNode(tree->nil, tree->nil, tree->nil, key, keySize, value, -1);
	RBNode* x = tree->root;
	RBNode* y = tree->nil;
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
	fixInsert(tree, z);

}

void transplant(RedBlackTree* tree, RBNode* n1, RBNode* n2){
	if(n1->parent == tree->nil) tree->root = n2;
	else if(n1 == n1->parent->left) n1->parent->left = n2;
	else n1->parent->right = n2;
	n2->parent = n1->parent;
}

RBNode* treeMinimum(RedBlackTree* tree, RBNode* n){
	while(n->left != tree->nil){
		n = n->left;
	}
	return n;
}

RBNode* treeMaximum(RedBlackTree* tree, RBNode* n){
	while(n->right != tree->nil){
		n = n->right;
	}
	return n;
}

void fixDelete(RedBlackTree* tree, RBNode* n){
	while(n != tree->root && n->c == BLACK){
		if(n==n->parent->left){
			RBNode* w = n->parent->right;
			if(w->c == RED){
				w->c = BLACK;
				n->parent->c = RED;
				leftRotate(tree, n->parent);
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
					rightRotate(tree, w);
					w = n->parent->right;
				}
				w->c = n->parent->c;
				n->parent->c = BLACK;
				w->right->c = BLACK;
				leftRotate(tree, n->parent);
				n = tree->root;
			}
		}
		else{
			RBNode* w = n->parent->left;
			if(w->c == RED){
				w->c = BLACK;
				n->parent->c = RED;
				rightRotate(tree, n->parent);
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
					leftRotate(tree, w);
					w = n->parent->left;
				}
				w->c = n->parent->c;
				n->parent->c = BLACK;
				w->left->c = BLACK;
				rightRotate(tree, n->parent);
				n = tree->root;
			}
		}
	}
}

void delete_(RedBlackTree* tree, char* key){
	tree->size -= 1;
	RBNode* n = findNode(tree, key);	
	RBNode* y = n;
	RBNode* x = NULL;
	Color originalColor = y->c;
	if(n->left == tree->nil){
		x = n->right;
		transplant(tree, n, n->right);
	}
	else if(n->right == tree->nil){
		x = n->left;
		transplant(tree, n, n->left);
	}
	else{
		y = treeMinimum(tree, n->right);
		originalColor = y->c;
		x = y->right;
		if(y != n->right){
			transplant(tree, y, y->right);
			y->right = n->right;
			y->right->parent = y;
		}
		else{
			x->parent = y;
			transplant(tree, n, y);
			y->left = n->left;
			y->left->parent = y;
			y->c = n->c;
		}
	}
	if(originalColor==BLACK){
		fixDelete(tree, x);
	}
	free(n->key);
	free(n);
}

int testNode(RedBlackTree* tree, RBNode* n, int depth){
	if(n == tree->nil) return depth;
	if(n->c == RED) assert(n->left->c != RED && n->right->c != RED);
	assert(testNode(tree, n->right, depth+1) == testNode(tree, n->left, depth+1));
	return depth;
}

void testRedBlackTree(RedBlackTree* tree){
	testNode(tree, tree->root, 0);
}

void treeToArray(RedBlackTree* tree, RBNode* n, int* index, char** keys, Value* values){
	if(n->left != tree->nil) treeToArray(tree, n->left, index, keys, values);
	if(n != tree->nil){
		strcpy(keys[*index], n->key);
		values[*index] = n->value;
		(*index)++;
	}
	if(n->right != tree->nil) treeToArray(tree, n->right, index, keys, values);		
}

bool contains(RedBlackTree* tree, char* key){
	RBNode* n = tree->root; 
	while(n != tree->nil && strcmp(key, n->key) != 0){
		if(strcmp(key, n->key)<0) n = n->left;
		else n = n->right;
	}
	if(n == tree->nil) return false;
	return strcmp(key, n->key)==0;
}


//node should have a key 