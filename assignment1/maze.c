#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "queue.h"
#include "stack.h"
#include "cell.h"
#include "maze.h"

long int random(void);
void srandom(unsigned int seed);

struct maze {
	int rows;
	int columns;
	int solved;
	CELL ***array;
};

MAZE *newMAZE(int r, int c) {
	MAZE *newmaze = malloc(sizeof(MAZE));
	newmaze->rows = r;
	newmaze->columns = c;
	newmaze->solved = 0;
	CELL ***newarray = malloc(r * sizeof(CELL **));
	for(int i = 0; i < r; i++) {
		newarray[i] = malloc(c * sizeof(CELL *));
	}
	for(int k = 0; k < r; k++) {
		for(int j=0; j < c; j++) {
			newarray[k][j] = newCELL(k, j);
		}
	}
	newmaze->array = newarray;
	return newmaze;
}

void buildMAZE(MAZE *maze) {
	STACK *stack = newSTACK();
	setwalls(maze);
	setvisitedCELL(maze->array[0][0], 1);
	push(stack, maze->array[0][0]);
	while(sizeSTACK(stack) > 0) {
		CELL *cell = pop(stack);
		int row = getrowCELL(cell);
		int col = getcolCELL(cell);
		if(visneighbors(maze, maze->array[row][col]) < neighbors(maze, maze->array[row][col])) {
			int x = neighbors(maze, maze->array[row][col]) - visneighbors(maze, maze->array[row][col]);
			CELL **cellarray = malloc(x * sizeof(CELL *));
			createcellarray(maze, cell, cellarray);
			CELL *neighbor = randomneighbor(cellarray, x);
			removewall(cell, neighbor);
			setvisitedCELL(neighbor, 1);
			push(stack, cell);
			push(stack, neighbor);
			free(cellarray);
		}
	}
	freeSTACK(stack);
	resetvisited(maze);
	return;
}

void setwalls(MAZE *maze) {
	for(int i = 0; i < maze->rows; i++) {
		for(int j = 0; j < maze->columns; j++) {
			setwallCELL(maze->array[i][j], 0, 1);
			setwallCELL(maze->array[i][j], 1, 1);	
			setwallCELL(maze->array[i][j], 2, 1);
			setwallCELL(maze->array[i][j], 3, 1);
		}
	}
	setwallCELL(maze->array[0][0], 2, 0);
	setwallCELL(maze->array[maze->rows-1][maze->columns-1], 0, 0);
	return;
}

int visneighbors(MAZE *maze, CELL *cell) {
	int row = getrowCELL(cell);
	int col = getcolCELL(cell);
	int i = 0;
	if(row + 1 < maze->rows) {
		if(visited(maze->array[row + 1][col]) == 1) {
			i++;
		}
	}
	if(row - 1 >= 0) {
  	 	if(visited(maze->array[row - 1][col]) == 1) {
        		i++;
         	}
 	}	
	if(col + 1 < maze->columns) {
        	if(visited(maze->array[row][col + 1]) == 1) {
                	i++;
        	}
	}
	if(col-1 >= 0) {
        	if(visited(maze->array[row][col-1]) == 1) {
                	i++;
        	}
	}	
	return i;
}

int neighbors(MAZE *maze, CELL *cell) {		
	int row = getrowCELL(cell);
	int col = getcolCELL(cell);
	int i = 0;
	if(row + 1 < maze->rows) {
		i++;
	}
	if(row - 1 >= 0) {
		i++;
	}
	if(col + 1 < maze->columns) {
		i++;	
	}
	if(col - 1 >= 0) {
		i++;
	}
	return i;
}

CELL *randomneighbor(CELL **cellarray, int eligible) {
	int x = random() % eligible;
	return cellarray[x];
}

void removewall(CELL *current, CELL *neighbor) {
	int x = getrowCELL(current) - getrowCELL(neighbor);
	int y = getcolCELL(current) - getcolCELL(neighbor);
	if(x == 1) {
		setwallCELL(current, 3, 0);
		setwallCELL(neighbor, 1, 0);
	}
	else if(x == -1) {
		setwallCELL(current, 1, 0);
		setwallCELL(neighbor, 3, 0);
	}
	else if(y == 1) {
		setwallCELL(current, 2, 0);
		setwallCELL(neighbor, 0, 0);
	}
	else if(y == -1) {
		setwallCELL(current, 0, 0);
		setwallCELL(neighbor, 2, 0);
	}
	return;
}

void resetvisited(MAZE *maze) {
	for(int i=0; i < maze->rows; i++) {
		for(int j=0; j < maze->columns; j++) {
			setvisitedCELL(maze->array[i][j], 0);
		}
	}
	return;
}

void createcellarray(MAZE *maze, CELL *cell, CELL **cellarray) {
	int row = getrowCELL(cell);
	int col = getcolCELL(cell);
	int i = 0;
	if(row > 0) {
		if(visited(maze->array[row-1][col]) != 1) {
			cellarray[i] = maze->array[row-1][col];
			i++;
		}
	}
	if(col > 0) {
		if(visited(maze->array[row][col-1]) != 1) {
			cellarray[i] = maze->array[row][col-1];
			i++;
		}
	}
	if(col < maze->columns - 1) {
		if(visited(maze->array[row][col+1]) != 1) {
			cellarray[i] = maze->array[row][col+1];
			i++;
		}
	}
	if(row < maze->rows - 1) {
		if(visited(maze->array[row+1][col]) != 1) {
			cellarray[i] = maze->array[row+1][col];
			i++;
		}
	}
	return;
}

void solveMAZE(MAZE *maze) {
	QUEUE *queue = newQUEUE();
	setvalueCELL(maze->array[0][0], 0);
	setvisitedCELL(maze->array[0][0], 1);
	enqueue(queue, maze->array[0][0]);
	while(sizeQUEUE(queue) > 0) {
		CELL *cell = dequeue(queue);
		int i = getvalueCELL(cell);
		if(getrowCELL(cell) == maze->rows-1 && getcolCELL(cell) == maze->columns-1) {
			freeQUEUE(queue);
			return;
		}
		else {
			int end = enqueueneighbors(maze, cell, queue, i);
			if(end == 1) {
				freeQUEUE(queue);
				return;
			}
		}
	}
	freeQUEUE(queue);
	return;
}

int enqueueneighbors(MAZE *maze, CELL *cell, QUEUE *queue, int i) {
	int row = getrowCELL(cell);
	int col = getcolCELL(cell);
	int end = 0;
	if(row - 1 >= 0) {
		if(getwallCELL(cell, 1) != 1) {
			if(visited(maze->array[row - 1][col]) != 1) {
				setvisitedCELL(maze->array[row-1][col], 1);
				setvalueCELL(maze->array[row-1][col], (i+1)%10);
				enqueue(queue, maze->array[row-1][col]);
				if(row+1 == maze->rows-1 && col == maze->columns-1) {
					end = 1;
				}
			}
		}
	}
	if(col - 1 >= 0) {
		if(getwallCELL(cell, 0) != 1) {
			if(visited(maze->array[row][col-1]) != 1) {
				setvisitedCELL(maze->array[row][col-1], 1);
				setvalueCELL(maze->array[row][col-1], (i+1)%10);
				enqueue(queue, maze->array[row][col-1]);
				if(row == maze->rows-1 && col-1 == maze->columns-1) {
					end = 1;
				}
			}
		}
	}
	if(col + 1 < maze->columns) {
		if(getwallCELL(cell, 2) != 1) {
			if(visited(maze->array[row][col+1]) != 1) {
				setvisitedCELL(maze->array[row][col+1], 1);
				setvalueCELL(maze->array[row][col+1], (i+1)%10);
				enqueue(queue, maze->array[row][col+1]);
				if(row == maze->rows-1 && col+1 == maze->columns-1) {
					end = 1;
				}
			}
		}
	}
	if(row + 1 < maze->rows) {
		if(getwallCELL(cell, 3) != 1) {
			if(visited(maze->array[row+1][col]) != 1) {
				setvisitedCELL(maze->array[row+1][col], 1);
				setvalueCELL(maze->array[row+1][col], (i+1)%10);
				enqueue(queue, maze->array[row+1][col]);
				if(row+1 == maze->rows-1 && col == maze->columns-1) {
					end = 1;
				}
			}
		}
	}
	return end;
}
		
void drawMAZE(MAZE *maze, FILE *fp, int solve) {
	if(solve == 0) {
		for(int i=0; i < maze->rows; i++) {
			for(int k=0; k < maze->columns; k++) {
				if(getwallCELL(maze->array[i][k], 1) == 1) {
					fprintf(fp, "----");
				}
				else {
					fprintf(fp, "-   ");
				}
				if(k == maze->columns-1) {
					fprintf(fp, "-\n");
				}
			}
			for(int j=0; j < maze->columns; j++) {
				if(getwallCELL(maze->array[i][j], 0) == 1) {
					fprintf(fp, "|   ");
				}
				else {
					fprintf(fp, "    ");
				}
				if(j == maze->columns-1) {
					if(getwallCELL(maze->array[i][j], 2) == 1) {
						fprintf(fp, "|\n");
					}
					else {
						fprintf(fp, " \n");
					}
				}
			}
		}
		for(int l=0; l < maze->columns; l++) {
			fprintf(fp, "----");
			if(l == maze->columns-1) {
				fprintf(fp, "-\n");
			}
		}
	}
	if(solve == 1) {
		for(int i=0; i < maze->rows; i++) {
                        for(int k=0; k < maze->columns; k++) {
                               	if(getwallCELL(maze->array[i][k], 1) == 1) {
                                       	fprintf(fp, "----");
                               	}
                               	else {
                                       	fprintf(fp, "-   ");
                               	}
                               	if(k == maze->columns-1) {
                                       	fprintf(fp, "-\n");
                               	}
                       	}
			for(int j=0; j< maze->columns; j++) {
                         	if(getwallCELL(maze->array[i][j], 0) == 1) {
					if(getvalueCELL(maze->array[i][j]) != (-1)) {
				
                                		fprintf(fp, "| %d ", getvalueCELL(maze->array[i][j]));
					}
					else {
						fprintf(fp, "|   ");
					}
                         	}
                         	else {
					if(getvalueCELL(maze->array[i][j]) != (-1)) {
                                		fprintf(fp, "  %d ", getvalueCELL(maze->array[i][j]));
					}
					else {
						fprintf(fp, "    ");
					}
                         	}
                        	if(j == maze->columns-1) {
                                	if(getwallCELL(maze->array[i][j], 2) == 1) {
                                        	fprintf(fp, "|\n");
                                	}
                                 	else {
                                         	fprintf(fp, " \n");
                                 	}
                         	}
                 	}
         	}
		for(int l=0; l < maze->columns; l++) {
                	fprintf(fp, "----");
                	if(l == maze->columns-1) {
                        	fprintf(fp, "-\n");
                	}
        	}
	}
	return;
}				

void freeMAZE(MAZE *maze) {
	for(int i=0; i < maze->rows; i++) {
		for(int j=0; j < maze->columns; j++) {
			freeCELL(maze->array[i][j]);
		}
		free(maze->array[i]);
	}
	free(maze->array);
	free(maze);
	return;
}

void seedrandom(int seed) {
	srandom(seed);
	return;
}

void setMAZEsolved(MAZE *maze, int solved) {
	maze->solved = solved;
	return;
}

int getMAZEsolved(MAZE *maze) {
	return maze->solved;
}


































































































































































































