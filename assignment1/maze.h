#ifndef __MAZE_INCLUDED__
#define __MAZE_INCLUDED__

#include <stdio.h>
#include "cell.h"
#include "stack.h"
#include "queue.h"

typedef struct maze MAZE;

extern MAZE *newMAZE(int r, int c);
extern void buildMAZE(MAZE *maze);
extern void setwalls(MAZE *maze);
extern int visneighbors(MAZE *maze, CELL *cell);
extern int neighbors(MAZE *maze, CELL *cell);
extern CELL *randomneighbor(CELL **cellarray, int x);
extern void removewall(CELL *current, CELL *neighbor);
extern void resetvisited(MAZE *maze);
extern void createcellarray(MAZE *maze, CELL *cell, CELL **cellarray);
extern void solveMAZE(MAZE *maze);
extern int enqueueneighbors(MAZE *maze, CELL *cell, QUEUE *queue, int i);
extern void drawMAZE(MAZE *maze, FILE *fp, int solve);
extern void freeMAZE(MAZE *maze);
extern void seedrandom(int seed);
extern void setMAZEsolved(MAZE *maze, int solved);
extern int getMAZEsolved(MAZE *maze);

#endif
