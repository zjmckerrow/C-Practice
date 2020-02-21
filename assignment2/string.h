#ifndef __STRING_INCLUDED__
#define __STRING_INCLUDED__

#include <stdio.h>
#include <string.h>

typedef struct STRING STRING;

extern STRING *newSTRING(char *x);
extern char *getSTRING(STRING *v);
extern char *setSTRING(STRING *v, char *s);
extern int compareSTRING(void *v, void *w);
extern void displaySTRING(void *v, FILE *fp);
extern void freeSTRING(void *v);

#endif

