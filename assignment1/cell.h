#ifndef __CELL_INCLUDED__
#define __CELL_INCLUDED__

#include <stdio.h>

typedef struct cell CELL;

extern CELL *newCELL(int row, int col);
extern void setvisitedCELL(CELL *cell, int value);
extern int visited(CELL *cell);
extern void setwallCELL(CELL *cell, int wall, int value);
extern void setvalueCELL(CELL *cell, int value);
extern int getvalueCELL(CELL *cell);
extern int  getwallCELL(CELL *cell, int wall);
extern void freeCELL(CELL *cell);
extern int getrowCELL(CELL *cell);
extern int getcolCELL(CELL *cell);

#endif
