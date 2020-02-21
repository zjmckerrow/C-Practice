/*
This program was created by Zach McKerrow to implement a queue using a circular dynamic array.
CWID: 11702949
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "queue.h"
#include "cda.h"

struct queue {
	CDA *cda;
	int debug;
	void (*displayf)(void *v, FILE *fp);
	void (*freef)(void *v);
};

QUEUE *newQUEUE(void) {
	QUEUE *newqueue = malloc(sizeof(QUEUE));
	newqueue->cda = newCDA();
	newqueue->debug = 0;
	newqueue->displayf = NULL;
	newqueue->freef = NULL;
	return newqueue;
}

void setQUEUEdisplay(QUEUE *items, void (*display)(void *value, FILE *fp)) {
	setCDAdisplay(items->cda, display);
	items->displayf = display;
	return;
}

void setQUEUEfree(QUEUE *items, void (*free)(void *value)) {
	setCDAfree(items->cda, free);
	items->freef = free;
	return;
}

void enqueue(QUEUE *items, void *value) {
	insertCDA(items->cda, sizeCDA(items->cda), value);
	return;
}

void *dequeue(QUEUE *items) {
	assert(sizeCDA(items->cda) > 0);
	return removeCDA(items->cda, 0);
}

void *peekQUEUE(QUEUE *items) {
	assert(sizeCDA(items->cda) > 0);
	return getCDA(items->cda, 0);
}

void displayQUEUE(QUEUE *items, FILE *fp) {
	if(items->displayf != NULL) {
		if(items->debug != 1 && items->debug != 2) {
			fprintf(fp, "<");
			for(int i=0; i < sizeCDA(items->cda); i++) {
				items->displayf(getCDA(items->cda, i), fp);
				if(i != sizeCDA(items->cda) - 1) {
					fprintf(fp, ",");
				}
			}
			fprintf(fp, ">");
			return;
		}
		else {
			displayCDA(items->cda, fp);
			return;
		}
	}
	else {
		if(items->debug != 1 && items->debug != 2) {
			fprintf(fp, "<");
			for(int i=0; i < sizeCDA(items->cda); i++) {
				fprintf(fp, "@%p", getCDA(items->cda, i));
				if(i != sizeCDA(items->cda)-1) {
					fprintf(fp, ",");
				}
			}
			fprintf(fp, ">");
			return;
		}
		else {
			displayCDA(items->cda, fp);
			return;
		}
	}
}

int debugQUEUE(QUEUE *items, int level) {
	int temp = items->debug;
	items->debug = level;
	if(items->debug == 1) {
		debugCDA(items->cda, 0);
	}
	if(items->debug == 2) {
		debugCDA(items->cda, 1);
	}
	return temp;
}

void freeQUEUE(QUEUE *items) {
	freeCDA(items->cda);
	free(items);
	return;
}

int sizeQUEUE(QUEUE *items) {
	return sizeCDA(items->cda);
}

