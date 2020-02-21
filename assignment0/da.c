/*
This program was written by Zach McKerrow to implement a dynamic array.
CWID: 11702949
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "da.h"

struct da {
	void **ptrarray;
	int size;
	int capacity;
	void (*displayf)(void *v, FILE *fp);
	void (*freef)(void *v);
	int debug;
};

void resize(DA *items, int scale);

DA *newDA(void) {
	DA *newda = malloc(sizeof(DA));
	newda->capacity = 1;
	newda->size = 0;
	void **array = malloc(sizeof(void*) * newda->capacity);
	for(int i=0; i < newda->capacity; i++) {
		array[i] = 0;
	}
	assert(array != 0);
	newda->ptrarray = array;
	newda->displayf = NULL;
	newda->freef = NULL;
	newda->debug = 0;
	return newda;
}
	
void setDAdisplay(DA *items, void (*display)(void *v, FILE *fp)) {
	items->displayf = display;
	return;
}

void setDAfree(DA *items, void(*free)(void *v)) {
	items->freef = free;
	return;
}

void insertDA(DA *items, int index, void *value) {
	if(items->size == items->capacity) {
		resize(items, 1);
	}
	items->size++;
	if(index == items->size-1) {
		setDA(items, index, value);
	}
	else{
		for(int i = items->size-1; i > index; i--) {
			setDA(items, i, getDA(items, i-1));
		}
		setDA(items, index, value);
	}
	return;
}	

void *removeDA(DA *items, int index) {
	assert(items->size > 0);
	void *temp = items->ptrarray[index];
	for(int i=index; i < items->size - 1; i++) {
		setDA(items, i, getDA(items, i+1));
	}
	items->size--;
	if(items->size < (items->capacity)/4) {
		resize(items, 2);
	}
	return temp;
}

void unionDA(DA *recipient, DA *donor) {
	for(int i=0; i < donor->size; i++) {
		insertDA(recipient, recipient->size, removeDA(donor, 0));
	}
	return;
}

void *getDA(DA *items, int index) {
	assert(index >= 0);
	assert(index < items->size);
	return items->ptrarray[index];
}

void *setDA(DA *items, int index, void *value) {
	assert(index >= 0);
	assert(index <= items->size);
	void *temp = NULL;
	if(index == items->size) {
		insertDA(items, index, value);
		temp = 0;
	}
	else {
		temp = items->ptrarray[index];
		items->ptrarray[index] = value;
	}
	return temp;
}

int sizeDA(DA *items) {
	return items->size;
}

void displayDA(DA *items, FILE *fp) {
	if(items->displayf == NULL) {
		fprintf(fp, "[");
		for(int i=0; i < items->size; i++) {
			fprintf(fp, "@%p", items->ptrarray[i]);
			if(i != items->size-1) {
				fprintf(fp, ",");
			}
		}	
		if(items->debug > 0) {
			if(items->size > 0) {
				fprintf(fp,",");
			}
			fprintf(fp, "[%d]", (items->capacity - items->size));
		}
		fprintf(fp, "]");
	}
	else {
		fprintf(fp, "[");
		for(int j=0; j < items->size; j++) {
			items->displayf(items->ptrarray[j], fp);
			if(j != items->size-1) {
				fprintf(fp, ",");
			}
		}
		if(items->debug > 0) {
			if(items->size > 0) {
				fprintf(fp,",");
			}
			fprintf(fp, "[%d]", (items->capacity - items->size));
		}
		fprintf(fp, "]");
	}
	return;
}
		

int debugDA(DA *items, int level) {
	int temp = items->debug;
	items->debug = level;
	return temp;
}

void freeDA(DA *items) {
	if(items->freef != NULL) {
		for(int i=0; i < items->size; i++) {
			items->freef(getDA(items, i));
		}
	}
	free(items->ptrarray);
	free((DA *) items);
	return;
}

void resize(DA *items, int scale) {
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
	void **array = realloc(items->ptrarray, sizeof(void *) * newcapacity);
	for(int i = items->size; i < newcapacity; i++) {
		array[i] = 0;
	}
	assert(array != 0);
	items->ptrarray = array;
	items->capacity = newcapacity;
	return;
}

	
	
	
		
