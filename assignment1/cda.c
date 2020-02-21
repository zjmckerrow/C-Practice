/* 
This program was written by Zach McKerrow to implement a circular dynamic array.
CWID: 11702949
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "cda.h"

struct cda {
	int size;
	int capacity;
	int debug;
	int front;
	void **ptrarray;
	void (*displayf)(void *v, FILE *fp);
	void (*freef)(void *v);
};

void resizeCDA(CDA *items, int scale);

CDA *newCDA(void) {
	CDA *newcda = malloc(sizeof(CDA));
	newcda->capacity=1;
	newcda->size=0;
	newcda->debug=0;
	newcda->front=(-1);
	void **array = malloc(sizeof(void*) * newcda->capacity);
	for(int i=0; i < newcda->capacity; i++) {
		array[i] = 0;
	}
	assert(array != 0);
	newcda->ptrarray = array;
	newcda->displayf=NULL;
	newcda->freef=NULL;
	return newcda;
}

void setCDAdisplay(CDA *items, void (*display)(void *v, FILE *fp)) {
	items->displayf = display;
	return;
}

void setCDAfree(CDA *items, void (*free)(void *v)) {
	items->freef = free;
	return;
}

void insertCDA(CDA *items, int index, void *value) {
	assert(index >= 0);
	assert(index <= items->size);
	if(items->size == items->capacity) {
		resizeCDA(items, 1);
	}
	if(index == 0) {
		if(items->size == 0) {
			items->ptrarray[0] = value;
			items->front = 0;
			items->size++;
			return;
		}
		items->front = (items->front + items->capacity - 1) % items->capacity;
		setCDA(items, 0, value);
		items->size++;
		return;
	}
	else if(index == items->size) {
		items->size++;
		setCDA(items, index, value);
		return;
	}
	else {
		items->size++;
		int mid = (items->size-1) / 2;
		if(index > mid) {
			for(int i = items->size-1; i > index; i--) {
				setCDA(items, i, getCDA(items, i-1));
			}
			setCDA(items, index, value);
		}
		else {
			items->front = (items->front + items->capacity - 1) % items->capacity;
			for(int j = 0; j < index; j++) {
				setCDA(items, j, getCDA(items, j+1));
			}
			setCDA(items, index, value);
		}
	}
	return;
}

void *removeCDA(CDA *items, int index) {
	assert(items->size > 0);
	assert(index >= 0);
	assert(index < items->size);
	void *temp = getCDA(items, index);
	if(index == 0) {
		items->front = (items->front + 1) % items->capacity;
		items->size--;
	}
	else if(index == items->size-1) {
		items->size--;
	}
	else {
		int mid = items->size / 2;
		if(index < mid) {
			for(int i=index; i > 0; i--) {
				setCDA(items, i, getCDA(items, i-1));
			}
			items->front = (items->front + 1) % items->capacity;
			items->size--;
		}
		else {
			for(int j = index; j < items->size-1; j++) {
				setCDA(items, j, getCDA(items, j+1));
			}
			items->size--;
		}
	}
	if(items->size < (items->capacity)/4) {
		resizeCDA(items, 2);
	}
	return temp;
}
			
void unionCDA(CDA *recipient, CDA *donor) {
	for(int i = 0; i < donor->size; i++) {
		insertCDA(recipient, recipient->size, removeCDA(donor, 0));
	}
	return;
}

void *getCDA(CDA *items, int index) {
	assert(index >= 0);
	assert(index < items->size);
	return items->ptrarray[(items->front + index) % items->capacity];
}

void *setCDA(CDA *items, int index, void *value) {
	assert(index >= (-1));
	assert(index <= items->size);
	void *temp = NULL;
	if(index != (-1) && index != items->size) {
		temp = getCDA(items, index);
	}
	if(index == items->size) {
		insertCDA(items, items->size, value);
		return NULL;
	}
	if (index == (-1)) {
		insertCDA(items, 0, value);
		return NULL;
	}
	items->ptrarray[(items->front + index) % items->capacity] = value;
	return temp;
}

int sizeCDA(CDA *items) {
	return items->size;
}

void displayCDA(CDA *items, FILE *fp) {
	if(items->displayf == NULL) {
		fprintf(fp, "(");
		for(int i=0; i < items->size; i++) {
			fprintf(fp, "@%p", getCDA(items, i));
			if(i != items->size-1) {
				fprintf(fp, ",");
			}
		}
		if(items->debug > 0) {
			if(items->size > 0) {
				fprintf(fp, ",");
			}
			fprintf(fp, "(%d)", (items->capacity - items->size));
		}
		fprintf(fp, ")");
	}
	else {
		fprintf(fp, "(");
		for(int j=0; j < items->size; j++) {
			items->displayf(getCDA(items, j), fp);
			if(j != items->size-1) {
				fprintf(fp, ",");
			}
		}
		if(items->debug > 0) {
			if(items->size > 0) {
				fprintf(fp, ",");
			}
			fprintf(fp, "(%d)", (items->capacity - items->size));
		}
		fprintf(fp, ")");
	}
	return;	
}

int debugCDA(CDA *items, int level) {
	int temp = items->debug;
	items->debug = level;
	return temp;
}

void freeCDA(CDA* items) {
	if(items->freef != NULL) {
		for(int i=0; i < items->size; i++) {
			items->freef(getCDA(items, i));
		}
	}
	free(items->ptrarray);
	free((CDA *) items);
	return;
}	

void resizeCDA(CDA *items, int scale) {
	int newcapacity = 0;
	if(scale == 1) {
		newcapacity = items->capacity * 2;
	}
	else {
		newcapacity = items->capacity / 2;
	}
	if(items->size == 0 || newcapacity == 0) {
		newcapacity = 1;
	}
	void **array = malloc(sizeof(void *) * newcapacity);
	assert(array != 0);
	for(int i=0; i < items->size; i++) {
		array[i] = getCDA(items, i);
	}
	for(int i = items->size; i < newcapacity; i++) {
		array[i] = 0;
	}
	if(items->size > 0) {
		items->front = 0;
	}
	else {
		items->front = (-1);
	}
	free(items->ptrarray);
	items->ptrarray = array;
	items->capacity = newcapacity;
	return;
}	

