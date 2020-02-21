#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "bst.h"
#include "queue.h"
#include "tnode.h"

TNODE *findpred(TNODE *node);
TNODE *findsucc(TNODE *node);
int findmindepth(BST *t, TNODE *root);
int findmaxdepth(BST *t, TNODE *root);
int nullchild(TNODE *root);
void swapperbst(TNODE *a, TNODE *b);
void freenodes(BST *t, TNODE *root);
void printinorder(BST *t, TNODE *root, FILE *fp);
void printpreorder(BST *t, TNODE *root, FILE *fp);
void printpostorder(BST *t, TNODE *root, FILE *fp);
TNODE *insert(BST *t, TNODE *root, void *value);
void *find(BST *t, TNODE *root, void *key);
TNODE *locate(BST *t, TNODE *root, void *value);

struct bst {
        int (*comparator)(void *v1, void *v2);
        void (*displayf)(void *v, FILE *fp);
        void (*freef)(void *v);
        void (*swapper)(TNODE *n1, TNODE *n2);
        TNODE *root;
        int size;
        int debug;
};

void swapperbst(TNODE *a, TNODE *b) {
	void *x = getTNODEvalue(a);
	void *y = getTNODEvalue(b);
	setTNODEvalue(a, y);
	setTNODEvalue(b, x);
	return;
}

BST *newBST(int (*c)(void *v1, void *v2)) {
	BST *newbst = malloc(sizeof(BST));
	assert(newbst != NULL);
	newbst->comparator = c;
	newbst->displayf = NULL;
	newbst->freef = NULL;
	newbst->root = NULL;
	setBSTswapper(newbst, swapperbst);
	newbst->size = 0;
	newbst->debug = 0;
	return newbst;
}

void setBSTdisplay(BST *t, void (*d)(void *v, FILE *fp)) {
	t->displayf = d;
	return;
}

void setBSTswapper(BST *t, void (*s)(TNODE *n1, TNODE *n2)) {
	t->swapper = s;
	return;
}

void setBSTfree(BST *t, void (*f)(void *v)) {
	t->freef = f;
	return;
}

TNODE *getBSTroot(BST *t) {
	return t->root;
}

void setBSTroot(BST *t, TNODE *replacement) {
	t->root = replacement;
	if(replacement != NULL) {
		setTNODEparent(replacement, replacement);
	}
	return;
}

void setBSTsize(BST *t, int s) {
	t->size = s;
	return;
}

TNODE *insertBST(BST *t, void *value) {
	if(t->root == NULL) {
		TNODE *n = newTNODE(value, NULL, NULL, NULL);
		assert(n != NULL);
		setTNODEdisplay(n, t->displayf);
		setTNODEfree(n, t->freef);
		debugTNODE(n, t->debug);
		setBSTroot(t, n);
		t->size++;
		return t->root;
	}
	else {
		insert(t, getBSTroot(t), value);
		return locateBST(t, value);
	}
}

TNODE *insert(BST *t, TNODE *root, void *key) {
	if(root == NULL) {
		TNODE *new = newTNODE(key, NULL, NULL, NULL);
		assert(new != NULL);
		setTNODEdisplay(new, t->displayf);
		setTNODEfree(new, t->freef);
		debugTNODE(new, t->debug);
		t->size++;
		return new;
	}
	if(t->comparator(getTNODEvalue(root), key) > 0) {
		setTNODEleft(root, insert(t, getTNODEleft(root), key));
		setTNODEparent(getTNODEleft(root), root);
	}
	else if(t->comparator(getTNODEvalue(root), key) < 0) {
		setTNODEright(root, insert(t, getTNODEright(root), key));
		setTNODEparent(getTNODEright(root), root);
	}
	return root;
}

void *findBST(BST *t, void *key) {
	if(sizeBST(t) == 0) {
		return NULL;
	}
	return find(t, t->root, key);
}

void *find(BST *t, TNODE *root, void *value) {
	if(root == NULL) {
		return NULL;
	}
	else if(t->comparator(getTNODEvalue(root), value) == 0) {
		return getTNODEvalue(root);
	}
	else if(t->comparator(getTNODEvalue(root), value) > 0) {
		return find(t, getTNODEleft(root), value);
	}
	else {
		return find(t, getTNODEright(root), value);
	}
}

TNODE *locateBST(BST *t, void *key) {
	if(sizeBST(t) == 0) {
        	return NULL;
	}
	return locate(t, t->root, key);
}

TNODE *locate(BST *t, TNODE *root, void *value) {
	if(root == NULL) {
		return NULL;
	}
	else if(t->comparator(getTNODEvalue(root), value) == 0) {
		return root;
	}
	else if(t->comparator(getTNODEvalue(root), value) > 0) {
		return locate(t, getTNODEleft(root), value);
	}
	else {
		return locate(t, getTNODEright(root), value);
	}
}

int deleteBST(BST *t, void *key) {
	TNODE *located = locateBST(t, key);
	if(located == NULL) {
		return -1;
	}
	if(sizeBST(t) > 1) {
		located = swapToLeafBST(t, located);
		pruneLeafBST(t, located);
	}
	else {
		setTNODEparent(located, NULL);
		t->root = NULL;
	}
	free(located);
	t->size--;
	return 0;
}	

TNODE *swapToLeafBST(BST *t, TNODE *node) {
	TNODE *pred = findpred(node);
	if(pred == NULL) {
		TNODE *succ = findsucc(node);
		if(succ == NULL) {
			return node;
		}
		else {
			t->swapper(node, succ);
			succ = swapToLeafBST(t, succ);
			return succ;
		}
	}
	else {
		t->swapper(node, pred);
		pred = swapToLeafBST(t, pred);
		return pred;
	}
}	

TNODE *findpred(TNODE *node) {
	TNODE *left = getTNODEleft(node);
	if(left == NULL) {
		return NULL;
	}
	else {
		while(getTNODEright(left) != NULL) {
			left = getTNODEright(left);
		}
		return left;
	}
}

TNODE *findsucc(TNODE *node) {
	TNODE *right = getTNODEright(node);
	if(right == NULL) {
		return NULL;
	}
	else {
		while(getTNODEleft(right) != NULL) {
			right = getTNODEleft(right);
		}
		return right;
	}
}

void pruneLeafBST(BST *t, TNODE *leaf) {
	TNODE *parent = getTNODEparent(leaf);
	if(getTNODEleft(parent) != NULL && t->comparator(getTNODEvalue(getTNODEleft(parent)), getTNODEvalue(leaf)) == 0) {
		setTNODEleft(parent, NULL);
	}
	else if(getTNODEright(parent) != NULL && t->comparator(getTNODEvalue(getTNODEright(parent)), getTNODEvalue(leaf)) == 0){
		setTNODEright(parent, NULL);
	}
	setTNODEparent(leaf, NULL);
	return;
}

int sizeBST(BST *t) {
	return t->size;
}

void statisticsBST(BST *t, FILE *fp) {
	int size = sizeBST(t);
	int min = findmindepth(t, t->root);
	int max = findmaxdepth(t, t->root);
	if(max > 0) {
		max = max - 1;
	}
	fprintf(fp, "Nodes: %d\n", size);
	fprintf(fp, "Minimum depth: %d\n", min);
	fprintf(fp, "Maximum depth: %d\n", max);
	return;
}

int findmindepth(BST *t, TNODE *root) {
	int rightdepth, leftdepth;
	if(sizeBST(t) == 0) {
		return -1;
	}
	else if(getTNODEleft(t->root) == NULL || getTNODEright(t->root) == NULL) {
		return 0;
	}
	else if(nullchild(root) == 1) {
		return 0;
	}
	else {
		leftdepth = findmindepth(t, getTNODEleft(root));
		rightdepth = findmindepth(t, getTNODEright(root));
		if(leftdepth < rightdepth) {
			return (leftdepth+1);
		}
		else {
			return (rightdepth+1);
		}
	}
}

int nullchild(TNODE *root) {
	if(getTNODEleft(root) == NULL || getTNODEright(root) == NULL) {
		return 1;
	}
	else {
		return 0;
	}
}

int findmaxdepth(BST *t, TNODE *root) {
	int rightdepth, leftdepth;
	if(sizeBST(t) == 0) {
		return -1;
	}
	else if(getTNODEleft(t->root) == NULL && getTNODEright(t->root) == NULL) {
		return 0;
	}
	else if(root == NULL) {
		return 0;
	}
	else {
		leftdepth = findmaxdepth(t, getTNODEleft(root));
		rightdepth = findmaxdepth(t, getTNODEright(root));
		if(leftdepth > rightdepth) {
			return (leftdepth+1);
		}
		else {
			return (rightdepth+1);
		}
	}
}

void displayBST(BST *t, FILE *fp) {
	if(t->debug == 0) {
		if(t->size == 0) {
			fprintf(fp, "0:\n");
			return;
		}
		QUEUE *queue = newQUEUE();
		TNODE *current = NULL;
		enqueue(queue, t->root);
		enqueue(queue, NULL);
		int count = 1;
		fprintf(fp, "0:");
		while(sizeQUEUE(queue) > 1) {
			current = dequeue(queue);
			if(current == NULL) {
				enqueue(queue, NULL);
				fprintf(fp, "\n%d:", count);
				count++;
			}
			else {
				if(getTNODEleft(current)) {
					enqueue(queue, getTNODEleft(current));
				}
				if(getTNODEright(current)) {
					enqueue(queue, getTNODEright(current));
				}
				fprintf(fp, " ");
				if(getTNODEright(current) == NULL && getTNODEleft(current) == NULL) {
					fprintf(fp, "=");
				}
				t->displayf(getTNODEvalue(current), fp);
				fprintf(fp, "(");
				t->displayf(getTNODEvalue(getTNODEparent(current)), fp);
				fprintf(fp, ")");
				if(getTNODEleft(getTNODEparent(current)) != NULL && t->comparator(getTNODEvalue(getTNODEleft(getTNODEparent(current))), getTNODEvalue(current)) == 0) {
					fprintf(fp, "L");
				}
				else if(getTNODEright(getTNODEparent(current)) != NULL && t->comparator(getTNODEvalue(getTNODEright(getTNODEparent(current))), getTNODEvalue(current)) == 0) {
					fprintf(fp, "R");
				}
				else if(t->comparator(getTNODEvalue(t->root), getTNODEvalue(current)) == 0) {
					fprintf(fp, "X");
				}
			}
		}
		freeQUEUE(queue);
		fprintf(fp, "\n");
		return;
	}
	else if(t->debug > 0) {
		if(sizeBST(t) == 0) {
			fprintf(fp, "[]");
		}
		else if(t->debug == 1) {
			printinorder(t, getBSTroot(t), fp);
		}
		else if(t->debug == 2) {
			printpreorder(t, getBSTroot(t), fp);
		}
		else if(t->debug == 3) {
			printpostorder(t, getBSTroot(t), fp);
		}
		return;
	}					
}

void printinorder(BST *t, TNODE *root, FILE *fp) {
	if(root == NULL) {
		return;
	}
	fprintf(fp, "[");
	printinorder(t, getTNODEleft(root), fp);
	if(getTNODEleft(root) != NULL) {
		fprintf(fp, " ");
	}
	t->displayf(getTNODEvalue(root), fp);
	if(getTNODEright(root) != NULL) {
		fprintf(fp, " ");
	}
	printinorder(t, getTNODEright(root), fp);
	fprintf(fp, "]");
	return;
}

void printpreorder(BST *t, TNODE *root, FILE *fp) {
	if(root == NULL) {
		return;
	}
	fprintf(fp, "[");
	t->displayf(getTNODEvalue(root), fp);
	if(getTNODEleft(root) != NULL) {
		fprintf(fp, " ");
	}
	printpreorder(t, getTNODEleft(root), fp);
	if(getTNODEright(root) != NULL) {
		fprintf(fp, " ");
	}
	printpreorder(t, getTNODEright(root), fp);
	fprintf(fp, "]");
	return;
}

void printpostorder(BST *t, TNODE *root, FILE *fp) {
	if(root == NULL) {
		return;
	}
	fprintf(fp, "[");
	printpostorder(t, getTNODEleft(root), fp);
	if(getTNODEleft(root) != NULL) {
		fprintf(fp, " ");
	}
	printpostorder(t, getTNODEright(root), fp);
	if(getTNODEright(root) != NULL) {
		fprintf(fp, " ");
	}
	t->displayf(getTNODEvalue(root), fp);
	fprintf(fp, "]");
	return;
}
	
int debugBST(BST *t, int level) {
	int temp = t->debug;
	t->debug = level;
	return temp;
}

void freeBST(BST *t) {
	freenodes(t, getBSTroot(t));
	free(t);
	return;
}

void freenodes(BST *t, TNODE *node) {
	if(node != NULL) {
		freenodes(t, getTNODEright(node));
		freenodes(t, getTNODEleft(node));
		if(t->freef != NULL) {
			t->freef(getTNODEvalue(node));
		}
		free(node);
	}
	return;
}
