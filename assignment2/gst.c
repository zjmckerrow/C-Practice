#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "gst.h"
#include "bst.h"
#include "tnode.h"
typedef struct GSTval GSTVALUE;
void displayGSTVALUE(void *a, FILE *fp);
int compareGSTVALUE(void *a, void *b);
void freeGSTVALUE(void *a);
GSTVALUE *newGSTVALUE(GST *t, void *value, int freq, int (*c)(void *v1, void *v2), void (*d)(void *v3, FILE *fp), void (*f)(void *v4));
void *getGSTVALUEvalue(GSTVALUE *v);
void setGSTVALUEvalue(GSTVALUE *v, void *val);
int getGSTVALUEfreq(GSTVALUE *v);
void setGSTVALUEfreq(GSTVALUE *v, int freq);
void swappergst(TNODE *a, TNODE *b);
void setGSTinsertions(GST *t, int insertions);

struct GSTval {
	GST *gst;
        void *value;
        int freq;
        int (*compare)(void *v1, void *v2);
        void (*displayf)(void *v, FILE *fp);
        void (*freef)(void *v3);
};

void displayGSTVALUE(void *a, FILE *fp) {
        GSTVALUE *x = a;
        x->displayf(x->value, fp);
	if(x->freq > 1) {
        	fprintf(fp, "<%d>", x->freq);
	}
        return;
}

int compareGSTVALUE(void *a, void *b) {
        GSTVALUE *x = a;
	GSTVALUE *y = b;
        return x->compare(getGSTVALUEvalue(x), getGSTVALUEvalue(y));
}

void freeGSTVALUE(void *v) {
        GSTVALUE *x = v;
	if(x->freef != NULL) {
		x->freef(x->value);
	}
        free(x);
        return;
}

GSTVALUE *newGSTVALUE(GST *t, void *value, int freq, int (*c)(void *v1, void *v2), void (*d)(void *v3, FILE *fp), void (*f)(void *v4)) {
        GSTVALUE *newvalue = malloc(sizeof(GSTVALUE));
	newvalue->gst = t;
        newvalue->value = value;
        newvalue->freq = freq;
        newvalue->compare = c;
        newvalue->displayf = d;
        newvalue->freef = f;
        return newvalue;
}

void *getGSTVALUEvalue(GSTVALUE *v) {
        return v->value;
}

void setGSTVALUEvalue(GSTVALUE *v, void *value) {
        v->value = value;
        return;
}

int getGSTVALUEfreq(GSTVALUE *v) {
        return v->freq;
}

void setGSTVALUEfreq(GSTVALUE *v, int freq) {
        v->freq = freq;
        return;
}

struct gst {
	BST *bst;
	int insertions;
	int (*compare)(void *v1, void *v2);
	void (*displayf)(void *v, FILE *fp);
	void (*freef)(void *v3);
	void (*swapper)(TNODE *n1, TNODE *n2);
};

void swappergst(TNODE *a, TNODE *b) {
	void *x = getTNODEvalue(a);
	void *y = getTNODEvalue(b);
	setTNODEvalue(a, y);
	setTNODEvalue(b, x);
	return;
}

GST *newGST(int (*c)(void *v1, void *v2)) {
	GST *newgst = malloc(sizeof(GST));
	newgst->bst = newBST(compareGSTVALUE);
	newgst->insertions = 0;
	newgst->compare = c;
	newgst->displayf = NULL;
	setBSTdisplay(newgst->bst, displayGSTVALUE);
	newgst->freef = NULL;
	setBSTfree(newgst->bst, freeGSTVALUE);
	setGSTswapper(newgst, swappergst);
	return newgst;
}

void setGSTdisplay(GST *t, void (*d)(void *v, FILE *fp)) {
	t->displayf = d;
	return;
}

void setGSTswapper(GST *t, void (*s)(TNODE *n1, TNODE *n2)) {
	t->swapper = s;
	setBSTswapper(t->bst, s);
	return;
}

void setGSTfree(GST *t, void (*f)(void *v)) {
	t->freef = f;
	return;
}

TNODE *getGSTroot(GST *t) {
	return getBSTroot(t->bst);
}

void setGSTroot(GST *t, TNODE *replacement) {
	setBSTroot(t->bst, replacement);
	return;
}

void setGSTsize(GST *t, int s) {
	setBSTsize(t->bst, s);
	return;
}

TNODE *insertGST(GST *t, void *value) {
	GSTVALUE *v = newGSTVALUE(t, value, 1, t->compare, t->displayf, t->freef);
	TNODE *located = locateBST(t->bst, v);
	if(located == NULL) {
		TNODE *inserted = insertBST(t->bst, v);
		t->insertions++;
		return inserted;
	}
	else {
		GSTVALUE *value = getTNODEvalue(located);
		setGSTVALUEfreq(value, value->freq + 1);
		freeGSTVALUE(v);
		t->insertions++;
		return NULL;
	}
}

void *findGST(GST *t, void *key) {
	GSTVALUE *v = newGSTVALUE(t, key, 1, t->compare, t->displayf, t->freef);
	GSTVALUE *v1 = findBST(t->bst, v);
	if(v1 == NULL) {
		free(v);
		return NULL;
	}
	free(v);
	return getGSTVALUEvalue(v1);
}

TNODE *locateGST(GST *t, void *key) {
	GSTVALUE *v = newGSTVALUE(t, key, 1, t->compare, t->displayf, t->freef);
	TNODE *located = locateBST(t->bst, v);
	free(v);
	return located;
}

int deleteGST(GST *t, void *key) {
	GSTVALUE *new = newGSTVALUE(t, key, 1, t->compare, t->displayf, t->freef);
	TNODE *node = locateBST(t->bst, new);
	free(new);
	if(node == NULL) {
        	return -1;
	}
	GSTVALUE *v = getTNODEvalue(node);
	int freq = getGSTVALUEfreq(v);
	if(freq == 1) {
		deleteBST(t->bst, v);
		free(v);
		t->insertions--;
		return 0;
	}
	else {
		setGSTVALUEfreq(v, getGSTVALUEfreq(v)-1);
		t->insertions--;
		return getGSTVALUEfreq(v);
	}
}

TNODE *swapToLeafGST(GST *t, TNODE *node) {
	return swapToLeafBST(t->bst, node);
}

void pruneLeafGST(GST *t, TNODE *leaf) {
	pruneLeafBST(t->bst, leaf);
	free(getTNODEvalue(leaf));
	t->insertions--;
	return;
}

int sizeGST(GST *t) {
	return sizeBST(t->bst);
}

void statisticsGST(GST *t, FILE *fp) {
	int dups = duplicatesGST(t);
	fprintf(fp, "Duplicates: %d\n", dups);
	statisticsBST(t->bst, fp);
	return;
}

void displayGST(GST *t, FILE *fp) {
        displayBST(t->bst, fp);
	return;
}

int debugGST(GST *t, int level) {
	return debugBST(t->bst, level);
}

void freeGST(GST *t) {
	freeBST(t->bst);
	free(t);
	return;
}

void *unwrapGST(TNODE *n) {
	GSTVALUE *v = getTNODEvalue(n);
	void *value = getGSTVALUEvalue(v);
	return value;
}

int freqGST(GST *g, void *key) {
	GSTVALUE *v = newGSTVALUE(g, key, 1, g->compare, g->displayf, g->freef);
	TNODE *located = locateBST(g->bst, v);
	if(located == NULL) {
		free(v);
		return 0;
	}
	GSTVALUE *v1 = getTNODEvalue(located);
	free(v);
	return getGSTVALUEfreq(v1);
}

int duplicatesGST(GST *g) {
	return g->insertions - sizeGST(g);
}

void setGSTinsertions(GST *t, int insertions) {
	t->insertions = insertions;
	return;
}
