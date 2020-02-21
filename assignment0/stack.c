/*
This program was written by Zach McKerrow to implement a stack using a dynamic array.
CWID: 11702949
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "stack.h"
#include "da.h"

struct stack {
	DA *da;
	int debug;
	void (*displayf)(void *v, FILE *fp);
	void (*freef)(void *v);
};

STACK *newSTACK(void) {
	STACK *newstack = malloc(sizeof(STACK));
	newstack->da = newDA();
	newstack->debug = 0;
	newstack->displayf = NULL;
	newstack->freef = NULL;
	return newstack;
}

void setSTACKdisplay(STACK *items, void (*display)(void *value, FILE *fp)) {
	setDAdisplay(items->da, display);
	items->displayf = display;
	return;
}

void setSTACKfree(STACK *items, void (*free)(void *value)) {
	setDAfree(items->da, free);
	items->freef = free;
	return;
}

void push(STACK *items, void *value) {
	insertDA(items->da, sizeDA(items->da), value);
	return;
}

void *pop(STACK *items) {
	assert(sizeDA(items->da) > 0);
	return removeDA(items->da, sizeDA(items->da)-1);
}

void *peekSTACK(STACK *items) {
	assert(sizeDA(items->da) > 0);
	return getDA(items->da, sizeDA(items->da)-1);
}

void displaySTACK(STACK *items, FILE *fp) {
	if(items->displayf != NULL) {
		if(items->debug != 1 && items->debug != 2) {
			fprintf(fp, "|");
			for(int i=sizeDA(items->da)-1; i >=0; i--) {
				items->displayf(getDA(items->da,i), fp);
				if(i != 0) {
					fprintf(fp, ",");
				}
			}
			fprintf(fp, "|");
			return;
		}
		else {
			displayDA(items->da, fp);
			return;
		}
	}
	else {
		if(items->debug != 1 && items->debug != 2) {
			fprintf(fp, "|");
			for(int i=sizeDA(items->da)-1; i >= 0; i--) {
				fprintf(fp, "@%p", getDA(items->da, i));
				if(i != 0) {
					fprintf(fp, ",");
				}
			}
			fprintf(fp, "|");
			return;
		}
		else {
			displayDA(items->da, fp);
			return;
		}
	}		
}

int debugSTACK(STACK *items, int level) {
	int temp = items->debug;
	items->debug = level;
	if(items->debug == 1) {
		debugDA(items->da, 0);
	}
	if(items->debug == 2) {
		debugDA(items->da, 1);
	}
	return temp;
}

void freeSTACK(STACK *items) {
	freeDA(items->da);
	free(items);
	return;
}

int sizeSTACK(STACK *items) {
	return sizeDA(items->da);
}

