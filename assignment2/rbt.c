#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "rbt.h"
#include "gst.h"
#include "tnode.h"

typedef struct RBTval RBTVALUE;
void swapperrbt(TNODE *a, TNODE *b);
RBTVALUE *newRBTVALUE(RBT *t, void *value, int color, int (*c)(void *v1, void *v2), void (*d)(void *v, FILE *fp), void (*f)(void *v3));
void *getRBTVALUEvalue(RBTVALUE *v);
void setRBTVALUEvalue(RBTVALUE *v, void *value);
int getRBTVALUEcolor(RBTVALUE *v);
void setRBTVALUEcolor(RBTVALUE *v, int color);
void freeRBTVALUE(void *a);
int compareRBTVALUE(void *a, void *b);
void displayRBTVALUE(void *a, FILE *fp);
void insertionFixUp(RBT *t, TNODE *node);
TNODE *uncle(TNODE *node);
void rotate(RBT *t, TNODE *child, TNODE *parent, int rol);
int linear(TNODE *node);
void freeRBTVALUES(TNODE *root);
void deletionFixUp(RBT *t, TNODE *node);
TNODE *sibling(TNODE *node);
TNODE *neice(TNODE *node);
TNODE *nephew(TNODE *node);
void freenodesRBT(TNODE *root);

struct rbt {
	GST *gst;
	int insertions;
	int (*compare)(void *v1, void *v2);
	void (*displayf)(void *v, FILE *fp);
	void (*freef)(void *v3);
	void (*swapper)(TNODE *n1, TNODE *n2);
};

struct RBTval {
	void *value;
	int color;
	RBT *rbt;
	int (*compare)(void *v1, void *v2);
	void (*displayf)(void *v, FILE *fp);
	void (*freef)(void *v3);
};

void freeRBTVALUE(void *a) {
	RBTVALUE *x = a;
	if(x->freef != NULL) {
		x->freef(x->value);
	}
	free(x);
	return;
}

int compareRBTVALUE(void *a, void *b) {
	RBTVALUE *x = a;
	RBTVALUE *y = b;
	return x->compare(getRBTVALUEvalue(x), getRBTVALUEvalue(y));
}

void displayRBTVALUE(void *a, FILE *fp) {
	RBTVALUE *x = a;
	x->displayf(x->value, fp);
	if(getRBTVALUEcolor(x) == 1) {
		fprintf(fp, "*");
	}
}
	
void swapperrbt(TNODE *a, TNODE *b) {
	void *va = getTNODEvalue(a);
	void *vb = getTNODEvalue(b);
	setTNODEvalue(a, vb);
	setTNODEvalue(b, va);
	RBTVALUE *x = unwrapGST(a);
	RBTVALUE *y = unwrapGST(b);
	int color = getRBTVALUEcolor(x);
	setRBTVALUEcolor(x, getRBTVALUEcolor(y));
	setRBTVALUEcolor(y, color);
	return;
}

RBTVALUE *newRBTVALUE(RBT *t, void *value, int color, int (*c)(void *v1, void *v2), void (*d)(void *v, FILE *fp), void (*f)(void *v3)) {
	RBTVALUE *newval = malloc(sizeof(RBTVALUE));
	newval->value = value;
	newval->rbt = t;
	newval->color = color;
	newval->compare = c;
	newval->displayf = d;
	newval->freef = f;
	return newval;
}

void *getRBTVALUEvalue(RBTVALUE *v) {
	return v->value;
}

void setRBTVALUEvalue(RBTVALUE *v, void *value) {
	v->value = value;
	return;
}

int getRBTVALUEcolor(RBTVALUE *v) {
	return v->color;
}

void setRBTVALUEcolor(RBTVALUE *v, int color) {
	v->color = color;
	return;
}

RBT *newRBT(int (*c)(void *v, void *v1)) {
	RBT *newrbt = malloc(sizeof(RBT));
	newrbt->gst = newGST(compareRBTVALUE);
	newrbt->compare = c;
	newrbt->insertions = 0;
	newrbt->displayf = NULL;
	setGSTdisplay(newrbt->gst, displayRBTVALUE);
	newrbt->freef = NULL;
	setGSTfree(newrbt->gst, freeRBTVALUE);
	setRBTswapper(newrbt, swapperrbt);
	setGSTswapper(newrbt->gst, swapperrbt);
	setRBTsize(newrbt, 0);
	return newrbt;
}

void setRBTdisplay(RBT *t, void (*d)(void *v, FILE *fp)) {
	t->displayf = d;
	return;
}

void setRBTswapper(RBT *t, void (*s)(TNODE *n, TNODE *n1)) {
	t->swapper = s;
	return;
}

void setRBTfree(RBT *t, void (*f)(void *v)) {
	t->freef = f;
	return;
}

TNODE *getRBTroot(RBT *t) {
	return getGSTroot(t->gst);
}

void setRBTroot(RBT *t, TNODE *replacement) {
	setGSTroot(t->gst, replacement);
	return;
}

void setRBTsize(RBT *t, int s) {
	setGSTsize(t->gst, s);
	return;
}

TNODE *insertRBT(RBT *t, void *value) {
	RBTVALUE *v = newRBTVALUE(t, value, 1, t->compare, t->displayf, t->freef);
	TNODE *inserted = insertGST(t->gst, v);
	if(inserted != NULL) {
		insertionFixUp(t, inserted);
	}
	t->insertions++;
	return inserted;
}

void insertionFixUp(RBT *t, TNODE *node) {
	int i = 0;
	while(i == 0) {
		if(compareRBTVALUE(unwrapGST(node), unwrapGST(getRBTroot(t))) == 0) {
			i = 1;
		}
		else if(getRBTVALUEcolor(unwrapGST(getTNODEparent(node))) == 0) {
			i = 1;
		}
		else if(uncle(node) != NULL && getRBTVALUEcolor(unwrapGST(uncle(node))) == 1) {
			setRBTVALUEcolor(unwrapGST(getTNODEparent(node)), 0);
			setRBTVALUEcolor(unwrapGST(uncle(node)), 0);
			setRBTVALUEcolor(unwrapGST(getTNODEparent(getTNODEparent(node))), 1);
			node = getTNODEparent(getTNODEparent(node));
		}
		else {
			if(linear(node) == 0) {
				TNODE *parent = getTNODEparent(node);
				if(getTNODEleft(getTNODEparent(node)) != NULL && compareRBTVALUE(unwrapGST(getTNODEleft(getTNODEparent(node))), unwrapGST(node)) == 0) {
					rotate(t, node, getTNODEparent(node), 1);
				}
				else {
					rotate(t, node, getTNODEparent(node), 0);
				}
				node = parent;
			}
			setRBTVALUEcolor(unwrapGST(getTNODEparent(node)), 0);
			setRBTVALUEcolor(unwrapGST(getTNODEparent(getTNODEparent(node))), 1);
			if(getTNODEleft(getTNODEparent(getTNODEparent(node))) != NULL && compareRBTVALUE(unwrapGST(getTNODEleft(getTNODEparent(getTNODEparent(node)))), unwrapGST(getTNODEparent(node))) == 0) {
				rotate(t, getTNODEparent(node), getTNODEparent(getTNODEparent(node)), 1);
			}
			else {
				rotate(t, getTNODEparent(node), getTNODEparent(getTNODEparent(node)), 0);
			}
			i = 1;
		}
	}
	setRBTVALUEcolor(unwrapGST(getRBTroot(t)), 0);
	return;
}

TNODE *uncle(TNODE *node) {
	TNODE *grandparent = getTNODEparent(getTNODEparent(node));
	if(compareRBTVALUE(unwrapGST(grandparent), unwrapGST(getTNODEparent(node))) == 0) {
		return NULL;
	}
	if(getTNODEleft(grandparent) != NULL && compareRBTVALUE(unwrapGST(getTNODEleft(grandparent)), unwrapGST(getTNODEparent(node))) == 0) {
		return getTNODEright(grandparent);
	}
	else {
		return getTNODEleft(grandparent);
	}
} 	

void rotate(RBT *t, TNODE *child, TNODE *parent, int rol) {
	TNODE *grandparent = getTNODEparent(parent);
	if(rol == 1) {
		setTNODEleft(parent, getTNODEright(child));
		if(getTNODEleft(parent) != NULL) {
			setTNODEparent(getTNODEleft(parent), parent);
		}
		if(compareRBTVALUE(unwrapGST(grandparent), unwrapGST(parent)) != 0) {
			if(getTNODEleft(grandparent) != NULL && compareRBTVALUE(unwrapGST(getTNODEleft(grandparent)), unwrapGST(parent)) == 0) {
				setTNODEleft(grandparent, child);
			}
			else {
				setTNODEright(grandparent, child);
			}
			setTNODEparent(child, grandparent);
			
		}
		else {
			setRBTroot(t, child);
		}
		setTNODEright(child, parent);
		setTNODEparent(parent, child);
		return;
	}
	else {
		setTNODEright(parent, getTNODEleft(child));
		if(getTNODEright(parent) != NULL) {
			setTNODEparent(getTNODEright(parent), parent);
		}
		if(compareRBTVALUE(unwrapGST(grandparent), unwrapGST(parent)) != 0) {
			if(compareRBTVALUE(unwrapGST(getTNODEleft(grandparent)), unwrapGST(parent)) == 0) {
				setTNODEleft(grandparent, child);
			}
			else {
				setTNODEright(grandparent, child);
			}
			setTNODEparent(child, grandparent);
		}
		else {
			setRBTroot(t, child);
		}
		setTNODEleft(child, parent);
		setTNODEparent(parent, child);
		return;
	}
}

int linear(TNODE *node) {
	if(getTNODEleft(getTNODEparent(node)) != NULL && getTNODEleft(getTNODEparent(getTNODEparent(node))) != NULL && compareRBTVALUE(unwrapGST(getTNODEleft(getTNODEparent(node))), unwrapGST(node)) == 0 && compareRBTVALUE(unwrapGST(getTNODEleft(getTNODEparent(getTNODEparent(node)))), unwrapGST(getTNODEparent(node))) == 0) {
		return 1;
	}
	if(getTNODEright(getTNODEparent(node)) != NULL && getTNODEright(getTNODEparent(getTNODEparent(node))) != NULL && compareRBTVALUE(unwrapGST(getTNODEright(getTNODEparent(node))), unwrapGST(node)) == 0 && compareRBTVALUE(unwrapGST(getTNODEright(getTNODEparent(getTNODEparent(node)))), unwrapGST(getTNODEparent(node))) == 0) {
		return 2;
	}
	else {
		return 0;
	}
}

void *findRBT(RBT *t, void *value) {
	RBTVALUE *v = newRBTVALUE(t, value, 1, t->compare, t->displayf, t->freef);
	RBTVALUE *v1 = findGST(t->gst, v);
	free(v);
	if(v1 == NULL) {
		return NULL;
	}
	else {
		return getRBTVALUEvalue(v1);
	}
}

TNODE *locateRBT(RBT *t, void *key) {
	RBTVALUE *v = newRBTVALUE(t, key, 1, t->compare, t->displayf, t->freef);
	TNODE *located = locateGST(t->gst, v);
	free(v);
	return located;
}

int deleteRBT(RBT *t, void *key) {
	RBTVALUE *new = newRBTVALUE(t, key, 1, t->compare, t->displayf, t->freef);
	TNODE *node = locateGST(t->gst, new);
	free(new);
	if(node == NULL) {
		return -1;
	}
	RBTVALUE *v = unwrapGST(node);
	int freq = freqGST(t->gst, v);
	if(freq == 1) {
		if(sizeRBT(t) > 1) {
			node = swapToLeafGST(t->gst, node);
			deletionFixUp(t, node);
			pruneLeafGST(t->gst, node);
		}
		else {
			setTNODEparent(node, NULL);
			setRBTroot(t, NULL);
			free(getTNODEvalue(node));
		}
		setRBTsize(t, sizeRBT(t)-1);
		free(node);
		free(v);
		return 0;
	}
	else {
		return deleteGST(t->gst, v);
	}
}

void deletionFixUp(RBT *t, TNODE *node) {
	int i = 0;
	while(i != 1) {
		if(compareRBTVALUE(unwrapGST(getRBTroot(t)), unwrapGST(node)) == 0) {
			i = 1;
		}
		else if(getRBTVALUEcolor(unwrapGST(node)) == 1) {
			i = 1;
		}
		else if(sibling(node) != NULL && getRBTVALUEcolor(unwrapGST(sibling(node))) == 1) {
			setRBTVALUEcolor(unwrapGST(getTNODEparent(node)), 1);
			setRBTVALUEcolor(unwrapGST(sibling(node)), 0);
			if(getTNODEleft(getTNODEparent(node)) != NULL && compareRBTVALUE(unwrapGST(getTNODEleft(getTNODEparent(node))), unwrapGST(node)) == 0) {
				rotate(t, sibling(node), getTNODEparent(node), 0);
			}
			else {
				rotate(t, sibling(node), getTNODEparent(node), 1);
			}
		}
		else if(nephew(node) != NULL && getRBTVALUEcolor(unwrapGST(nephew(node))) == 1) {
			setRBTVALUEcolor(unwrapGST(sibling(node)), getRBTVALUEcolor(unwrapGST(getTNODEparent(node))));
			setRBTVALUEcolor(unwrapGST(getTNODEparent(node)), 0);
			setRBTVALUEcolor(unwrapGST(nephew(node)), 0);
			if(sibling(node) != NULL) {
				if(getTNODEleft(getTNODEparent(node)) != NULL && compareRBTVALUE(unwrapGST(getTNODEleft(getTNODEparent(node))), unwrapGST(node)) == 0) {
                                	rotate(t, sibling(node), getTNODEparent(node), 0);
                        	}	
                        	else {
                                	rotate(t, sibling(node), getTNODEparent(node), 1);
                        	}
			}
			i = 1;
		}
		else if(neice(node) != NULL && getRBTVALUEcolor(unwrapGST(neice(node))) == 1) {
			setRBTVALUEcolor(unwrapGST(neice(node)), 0);
			setRBTVALUEcolor(unwrapGST(sibling(node)), 1);
			if(getTNODEleft(sibling(node)) != NULL && compareRBTVALUE(unwrapGST(getTNODEleft(sibling(node))), unwrapGST(neice(node))) == 0) {
                                rotate(t, neice(node), sibling(node), 1);
                        }
                        else {
                                rotate(t, neice(node), sibling(node), 0);
                        }
		}
		else {
			setRBTVALUEcolor(unwrapGST(sibling(node)), 1);
			node = getTNODEparent(node);
		}
	}
	setRBTVALUEcolor(unwrapGST(node), 0);
	return;
}

TNODE *sibling(TNODE *node) {
	if(getTNODEleft(getTNODEparent(node)) != NULL && compareRBTVALUE(unwrapGST(getTNODEleft(getTNODEparent(node))), unwrapGST(node)) == 0) {
		return getTNODEright(getTNODEparent(node));
	}
	else {
		return getTNODEleft(getTNODEparent(node));
	}
}

TNODE *neice(TNODE *node) {
	if(getTNODEleft(getTNODEparent(node)) != NULL && compareRBTVALUE(unwrapGST(getTNODEleft(getTNODEparent(node))), unwrapGST(node)) == 0) {
		return getTNODEleft(sibling(node));
	}
	else {
		return getTNODEright(sibling(node));
	}
}

TNODE *nephew(TNODE *node) {
        if(getTNODEleft(getTNODEparent(node)) != NULL && compareRBTVALUE(unwrapGST(getTNODEleft(getTNODEparent(node))), unwrapGST(node)) == 0) {
                return getTNODEright(sibling(node));
        }
        else {
                return getTNODEleft(sibling(node));
        }
}

TNODE *swapToLeafRBT(RBT *t, TNODE *node) {
	return swapToLeafGST(t->gst, node);
}

void pruneLeafRBT(RBT *t, TNODE *leaf) {
	pruneLeafGST(t->gst, leaf);
	free(unwrapGST(leaf));
	return;
}

int sizeRBT(RBT *t) {
	return sizeGST(t->gst);
}

void statisticsRBT(RBT *t, FILE *fp) {
	statisticsGST(t->gst, fp);
	return;
}

void displayRBT(RBT *t, FILE *fp) {
	displayGST(t->gst, fp);
	return;
}

int debugRBT(RBT *t, int level) {
	return debugGST(t->gst, level);
}

void freeRBT(RBT *t) {
	freeGST(t->gst);
	free(t);
	return;
}

void *unwrapRBT(TNODE *n) {
	RBTVALUE *v = unwrapGST(n);
	void *value = getRBTVALUEvalue(v);
	return value;
}

int freqRBT(RBT *g, void *key) {
	RBTVALUE *v = newRBTVALUE(g, key, 1, g->compare, g->displayf, g->freef);
	int freq = freqGST(g->gst, v);
	free(v);
	return freq;
}

int duplicatesRBT(RBT *g) {
	return g->insertions - sizeRBT(g);
}
