#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "tnode.h"

struct tnode {
	void *value;
	TNODE *left;
	TNODE *right;
	TNODE *parent;
	void (*displayf)(void *v, FILE *fp);
	void (*freef)(void *v);
	int debug;
};

TNODE *newTNODE(void *value, TNODE *left, TNODE *right, TNODE *parent) {
	TNODE *newnode = malloc(sizeof(TNODE));
	assert(newnode != NULL);
	newnode->value = value;
	newnode->left = left;
	newnode->right = right;
	newnode->parent = parent;
	newnode->displayf = NULL;
	newnode->freef = NULL;
	newnode->debug = 0;
	return newnode;
}

void setTNODEdisplay(TNODE *n, void (*d)(void *value, FILE *fp)) {
	n->displayf = d;
	return;
}

void setTNODEfree(TNODE *n, void (*f)(void *value)) {
	n->freef = f;
	return;
}

void *getTNODEvalue(TNODE *n) {
	return n->value;
}

void setTNODEvalue(TNODE *n, void *replacement) {
	n->value = replacement;
	return;
}

TNODE *getTNODEleft(TNODE *n) {
	return n->left;
}

void setTNODEleft(TNODE *n, TNODE *replacement) {
	n->left = replacement;
	return;
}

TNODE *getTNODEright(TNODE *n) {
	return n->right;
}

void setTNODEright(TNODE *n, TNODE *replacement) {
	n->right = replacement;
	return;
}

TNODE *getTNODEparent(TNODE *n) {
	return n->parent;
}

void setTNODEparent(TNODE *n, TNODE *replacement) {
	n->parent = replacement;
	return;
}

void displayTNODE(TNODE *n, FILE *fp) {
	if(n->displayf == NULL) {
		if(n->debug > 0) {
			fprintf(fp, "&%p&%p", n->value, n);
		}
		else {
			fprintf(fp, "&%p", n->value);
		}
	}
	else {
		if(n->debug > 0) {
			n->displayf(n->value, fp);
			fprintf(fp, "&%p", n);
		}
		else {
			n->displayf(n->value, fp);
		}
	}
	return;		
}

int debugTNODE(TNODE *n, int level) {
	int temp = n->debug;
	n->debug = level;
	return temp;
}

void freeTNODE(TNODE *n) {
	if(n->freef != NULL) {
		n->freef(n->value);
	}
	free(n);
	return;
}	
