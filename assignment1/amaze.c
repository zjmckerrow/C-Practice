#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "maze.h"

int main(int argc, char *argv[]) {
	int seed = 1;
	for(int j = 0; j < argc; j++) {
		if(strcmp(argv[j], "-r") == 0) {
			seed = atoi(argv[j+1]);
		}
	}
	for(int i = 0; i < argc; i++) {
		if(strcmp(argv[i], "-v") == 0) {
			printf("Written by Zach McKerrow\n");
			return 0;
		}
	}
	for(int k = 0; k < argc; k++) {
		if(strcmp(argv[k], "-c") == 0) {
			FILE *fp = fopen(argv[k+3], "w");
			fprintf(fp, "built %d %d %d", atoi(argv[k+1]), atoi(argv[k+2]), seed);
			fclose(fp);
		}
	}
	for(int l = 0; l < argc; l++) {
		if(strcmp(argv[l], "-s") == 0) {
			int rows = 0;
			int cols = 0;
			char status[5];
			FILE *fp3 = fopen(argv[l+1], "r");
			fscanf(fp3, "%s %d %d %d", status, &rows, &cols, &seed);
			FILE *fp1 = fopen(argv[l+2], "w");
			fprintf(fp1, "solve %d %d %d", rows, cols, seed);
			fclose(fp1);
			fclose(fp3);
		}
	}
	for(int m = 0; m < argc; m++) {
		if(strcmp(argv[m], "-d") == 0) {
			char s[5];
			int row, col = 0;
			FILE *fp2 = fopen(argv[m+1], "r");
			fscanf(fp2, "%s %d %d %d", s, &row, &col, &seed);
			seedrandom(seed);
			MAZE *maze = newMAZE(row, col);
			buildMAZE(maze);
			if(strcmp(s, "built") == 0) {
				drawMAZE(maze, stdout, 0);
			}
			if(strcmp(s, "solve") == 0) {
				solveMAZE(maze);
				drawMAZE(maze, stdout, 1);
			}
			freeMAZE(maze);
			fclose(fp2);
		}
	}
	return 0;
}
