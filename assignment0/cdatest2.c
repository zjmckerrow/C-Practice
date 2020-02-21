#include <stdio.h>
#include <stdlib.h>
#include "cda.h"
#include "integer.h"

/*
CDA Test Suite
Version 2
Created by Lusth
Modified by Ben Gerszewski
*/

/*
Random: rand()%sizeCDA(d)
Front: 0
Insert Back: sizeCDA(d)
Remove Back: sizeCDA(d)-1
*/

#define I1 rand()%sizeCDA(d)
#define R1 rand()%sizeCDA(d)
#define I2 rand()%sizeCDA(d)
#define R2 rand()%sizeCDA(d)
#define I3 rand()%sizeCDA(d)
#define R3 rand()%sizeCDA(d)

int main(int argc, char **argv) {
	if (argc != 2) {
		printf("Add loop size to command line arguments.\n");
		printf("Example: ./cdatest 5000\n");
		return 1;
	}
	int LOOPSIZE = atoi(argv[1]);
	int count = 0;
	srand(10);
	CDA *d = newCDA();
	setCDAdisplay(d, displayINTEGER);
	setCDAfree(d, freeINTEGER);
	printf("doing a bunch of inserts\n");
	for (int i = 0; i < LOOPSIZE; ++i) {
		i ?
			insertCDA(d, I1, newINTEGER(rand() % 1000)) :
			insertCDA(d, 0, newINTEGER(rand() % 1000));
		++count;
		insertCDA(d, I3, newINTEGER(rand() % 1000));
		++count;
	}
	printf("doing a bunch of random inserts\n");
	for (int i = 0; i < LOOPSIZE; ++i) {
		int index = I2;
		insertCDA(d, index, newINTEGER(rand() % 1000));
		++count;
	}
	printf("size: %d\n", sizeCDA(d));
	printf("randomly doing a bunch of inserts and deletes\n");
	for (int i = 0; i < LOOPSIZE; ++i) {
		int mode = rand() % 6;
		if (mode == 0) {
			insertCDA(d, I1, newINTEGER(rand() % 1000));
			++count;
		}
		else if (mode == 1) {
			insertCDA(d, I2, newINTEGER(rand() % 1000)); 
			++count;
		}
		else if (mode == 2) {
			insertCDA(d, I3,newINTEGER(rand() % 1000));
			++count;
		}
		else if (mode == 3 && count > 0) {
			freeINTEGER(removeCDA(d, R1));
			--count;
		}
		else if (mode == 4 && count > 0) {
			freeINTEGER(removeCDA(d, R2));
			--count;
		}
		else if (count > 0) {
			freeINTEGER(removeCDA(d, R3));
			--count;
		}
	}
	printf("size: %d\n", sizeCDA(d));
	printf("doing a bunch of random removes\n");
	while (count > 10) {
		freeINTEGER(removeCDA(d, R2));
		--count;
	}
	printf("array d:");
	displayCDA(d, stdout);
	printf("\n");
	freeCDA(d);
	printf("CDA freed\n");
	return 0;
}

