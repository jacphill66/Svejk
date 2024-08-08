#include "TemplateTree.h"
void printTemplateNode(TemplateRBNode* node, TemplateRBNode* nil){
	if(node == nil) return;
	printf("(");
	printTemplateNode(node->left, nil);
	printTemplate(node->t);
	printTemplateNode(node->right, nil);
	printf(")");
}

void printTemplateTree(TemplateRedBlackTree* tree){
	printTemplateNode(tree->root, tree->nil);
	printf("\n");
}

void freeSingleTemplateNodeNoKey(TemplateRBNode* node){
	free(node);
}

void freeTemplateNodeNoKeys(TemplateRedBlackTree* tree, TemplateRBNode* node){
	if(node == tree->nil) return;
	freeTemplateNodeNoKeys(tree, node->left);
	freeTemplateNodeNoKeys(tree, node->right);
	freeSingleTemplateNodeNoKey(node);
}

void freeTemplateTreeNoKeys(TemplateRedBlackTree* tree){
	freeTemplateNodeNoKeys(tree, tree->root);
}

TemplateRBNode* newNodeTemplate(TemplateRBNode* parent, TemplateRBNode* left, TemplateRBNode* right, char* key, int keySize, Template* t, Color c){
	TemplateRBNode* n = (TemplateRBNode*) malloc(sizeof(TemplateRBNode));
	n->parent = parent;
	n->left = left;
	n->right = right;
	n->key = key;
	n->keySize = keySize;
	n->t = t;
	n->c = c;
	return n;
}

TemplateRedBlackTree* newTemplateTree(){
	TemplateRedBlackTree* tree = (TemplateRedBlackTree*) malloc(sizeof(TemplateRedBlackTree));
	Template* t = (Template*)malloc(sizeof(Template));
	tree->nil = newNodeTemplate(NULL, NULL, NULL, NULL, -1, NULL, BLACK);
	tree->root = tree->nil;
	return tree;
}

TemplateRBNode* findTemplateNode(TemplateRedBlackTree* tree, char* key){
	TemplateRBNode* n = tree->root; 
	while(n != tree->nil && key != n->key){
		if(strcmp(key, n->key) < 0) n = n->left;
		else n = n->right;
	}
	return n;
}

Template* searchTemplate(TemplateRedBlackTree* tree, char* key){
	TemplateRBNode* n = tree->root; 
	while(n != tree->nil && strcmp(key, n->key) != 0){
		if(strcmp(key, n->key)<0) n = n->left;
		else n = n->right;
	}
	if(n == tree->nil) return NULL;
	return n->t;
}

void leftRotateTemplate(TemplateRedBlackTree* tree, TemplateRBNode* n){
	TemplateRBNode* y = n->right;
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

void rightRotateTemplate(TemplateRedBlackTree* tree, TemplateRBNode* n){
	TemplateRBNode* y = n->left;
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

void fixInsertTemplate(TemplateRedBlackTree* tree, TemplateRBNode* z){
	while(z->parent->c == RED){
		if(z->parent == z->parent->parent->left){
			TemplateRBNode* y = z->parent->parent->right;
			if(y->c == RED){
				z->parent->c = BLACK;
				y->c = BLACK;
				z->parent->parent->c = RED;
				z = z->parent->parent;
			}
			else{
				if(z == z->parent->right){
					z = z->parent;
					leftRotateTemplate(tree, z);
				}
				z->parent->c = BLACK;
				z->parent->parent->c = RED;
				rightRotateTemplate(tree, z->parent->parent);
			}
		}
		else{
			TemplateRBNode* y = z->parent->parent->left;
			if(y->c == RED){
				z->parent->c = BLACK;
				y->c = BLACK;
				z->parent->parent->c = RED;
				z = z->parent->parent;
			}
			else{
				if(z == z->parent->left){
					z = z->parent;
					rightRotateTemplate(tree, z);
				}
				z->parent->c = BLACK;
				z->parent->parent->c = RED;
				leftRotateTemplate(tree, z->parent->parent);
			}
		}
	}
	tree->root->c = BLACK;
}

void insertTemplate(TemplateRedBlackTree* tree, char* key, int keySize, Template* t){
	tree->size += 1;
	TemplateRBNode* z = newNodeTemplate(tree->nil, tree->nil, tree->nil, key, keySize, t, -1);
	TemplateRBNode* x = tree->root;
	TemplateRBNode* y = tree->nil;
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
	fixInsertTemplate(tree, z);

}

void transplantTemplate(TemplateRedBlackTree* tree, TemplateRBNode* n1, TemplateRBNode* n2){
	if(n1->parent == tree->nil) tree->root = n2;
	else if(n1 == n1->parent->left) n1->parent->left = n2;
	else n1->parent->right = n2;
	n2->parent = n1->parent;
}

TemplateRBNode* treeMinimumTemplate(TemplateRedBlackTree* tree, TemplateRBNode* n){
	while(n->left != tree->nil){
		n = n->left;
	}
	return n;
}

TemplateRBNode* treeMaximumTemplate(TemplateRedBlackTree* tree, TemplateRBNode* n){
	while(n->right != tree->nil){
		n = n->right;
	}
	return n;
}

void fixDeleteTemplate(TemplateRedBlackTree* tree, TemplateRBNode* n){
	while(n != tree->root && n->c == BLACK){
		if(n==n->parent->left){
			TemplateRBNode* w = n->parent->right;
			if(w->c == RED){
				w->c = BLACK;
				n->parent->c = RED;
				leftRotateTemplate(tree, n->parent);
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
					rightRotateTemplate(tree, w);
					w = n->parent->right;
				}
				w->c = n->parent->c;
				n->parent->c = BLACK;
				w->right->c = BLACK;
				leftRotateTemplate(tree, n->parent);
				n = tree->root;
			}
		}
		else{
			TemplateRBNode* w = n->parent->left;
			if(w->c == RED){
				w->c = BLACK;
				n->parent->c = RED;
				rightRotateTemplate(tree, n->parent);
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
					leftRotateTemplate(tree, w);
					w = n->parent->left;
				}
				w->c = n->parent->c;
				n->parent->c = BLACK;
				w->left->c = BLACK;
				rightRotateTemplate(tree, n->parent);
				n = tree->root;
			}
		}
	}
}

void deleteTemplate(TemplateRedBlackTree* tree, char* key){
	tree->size -= 1;
	TemplateRBNode* n = findTemplateNode(tree, key);	
	TemplateRBNode* y = n;
	TemplateRBNode* x = NULL;
	Color originalColor = y->c;
	if(n->left == tree->nil){
		x = n->right;
		transplantTemplate(tree, n, n->right);
	}
	else if(n->right == tree->nil){
		x = n->left;
		transplantTemplate(tree, n, n->left);
	}
	else{
		y = treeMinimumTemplate(tree, n->right);
		originalColor = y->c;
		x = y->right;
		if(y != n->right){
			transplantTemplate(tree, y, y->right);
			y->right = n->right;
			y->right->parent = y;
		}
		else{
			x->parent = y;
			transplantTemplate(tree, n, y);
			y->left = n->left;
			y->left->parent = y;
			y->c = n->c;
		}
	}
	if(originalColor==BLACK){
		fixDeleteTemplate(tree, x);
	}
	free(n->key);
	free(n);
}

int testNodeTemplate(TemplateRedBlackTree* tree, TemplateRBNode* n, int depth){
	if(n == tree->nil) return depth;
	if(n->c == RED) assert(n->left->c != RED && n->right->c != RED);
	assert(testNodeTemplate(tree, n->right, depth+1) == testNodeTemplate(tree, n->left, depth+1));
	return depth;
}

void testTemplateTree(TemplateRedBlackTree* tree){
	testNodeTemplate(tree, tree->root, 0);
}


bool containsTemplate(TemplateRedBlackTree* tree, char* key){
	TemplateRBNode* n = tree->root; 
	while(n != tree->nil && strcmp(key, n->key) != 0){
		if(strcmp(key, n->key)<0) n = n->left;
		else n = n->right;
	}
	if(n == tree->nil) return false;
	return strcmp(key, n->key)==0;
}
