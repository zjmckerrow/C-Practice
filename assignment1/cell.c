#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "cell.h"

struct cell {
	int rowlocation;
	int collocation;
	int value;
	int left;
	int right;
	int up;
	int down;
	int visited;
};

CELL *newCELL(int row, int col) {
	CELL *newcell = malloc(sizeof(CELL));
	newcell->rowlocation = row;
	newcell->collocation = col;
	newcell->value = -1;
	newcell->left = 0;
	newcell->right = 0;
	newcell->up = 0;
	newcell->down = 0;
	newcell->visited = 0;
	return newcell;
}

void setvisitedCELL(CELL *cell, int value) {
	cell->visited = value;
	return;
}

int visited(CELL *cell) {
	return cell->visited;
}

void setwallCELL(CELL *cell, int wall, int value) {
	if(wall == 2) {
		cell->left = value;
	}
	else if(wall == 0) {
		cell->right = value;
	}
	else if(wall == 3) {
		cell->up = value;
	}
	else if(wall == 1) {
		cell->down = value;
	}
	return;
}

void setvalueCELL(CELL *cell, int value) {
	cell->value = value;
	return;
}

int getvalueCELL(CELL *cell) {
	return cell->value;
}

int getwallCELL(CELL *cell, int wall) {
	if(wall == 1) {
		return cell->up;
	}
	else if(wall == 0) {
		return cell->left;
	}
	else if(wall == 2) {
		return cell->right;
	}
	else if(wall == 3) {
		return cell->down;
	}
	else{
		return 0;
	}
}


void freeCELL(CELL *cell) {
	free(cell);
	return;
}

int getrowCELL(CELL *cell) {
	return cell->rowlocation;
}

int getcolCELL(CELL *cell) {
	return cell->collocation;
}













