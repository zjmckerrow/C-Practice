#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "bst.h"
#include "gst.h"
#include "rbt.h"
#include "tnode.h"
#include "string.h"
#include "scanner.h"

char *cleanstring(char *s);
int main(int argc, char *argv[]) {
	for(int i = 0; i < argc; i++) {
		if(strcmp(argv[i], "-v") == 0) {
			printf("Zach McKerrow\n");
			return 0;
		}
	}
	FILE *data = NULL;
	FILE *commands = NULL;
	if(strcmp(argv[1], "-g") != 0 && strcmp(argv[1], "-r") != 0) {
		data = fopen(argv[1], "r");
		commands = fopen(argv[2], "r");
	}
	else {
		data = fopen(argv[2], "r");
		commands = fopen(argv[3], "r");
	}
	if(strcmp(argv[1], "-g") == 0) {		
        	GST *gst = newGST(compareSTRING);
        	setGSTdisplay(gst, displaySTRING);
        	setGSTfree(gst, freeSTRING);
		char *s = NULL;
		if(stringPending(data)) {
			s = readString(data);
		}
		else {
			s = readToken(data);
		}
		while(!feof(data)) {
			s = cleanstring(s);
			if(s != NULL) {
				STRING *str = newSTRING(s);
				insertGST(gst, str);
			}
			if(stringPending(data)) {
                      		s = readString(data);
                	}
                	else {
                      		s = readToken(data);
                	}
		}
		if(stringPending(commands)) {
			s = readString(commands);
		}
		else {
			s = readToken(commands);
		}
		while(!feof(commands)) {
			s = cleanstring(s);
			if(strcmp(s, "i") == 0) {
				if(stringPending(commands)) {
					s = readString(commands);
				}
				else {
					s = readToken(commands);
				}
				s = cleanstring(s);
				if(s != NULL) {
					STRING *string = newSTRING(s);
					insertGST(gst, string);
				}
			}
			else if(strcmp(s, "d") == 0) {
				if(stringPending(commands)) {
					s = readString(commands);
				}
				else {
					s = readToken(commands);
				}
				s = cleanstring(s);
				if(s != NULL) {
					STRING *string = newSTRING(s);
					if(deleteGST(gst, string) == -1) {
						printf("Value ");
						displaySTRING(string, stdout);
						printf(" not found.\n");
					}
				}
			}
			else if(strcmp(s, "f") == 0) {
                                if(stringPending(commands)) {
                                        s = readString(commands);
                                }
                                else {
                                        s = readToken(commands);
                                }
                                s = cleanstring(s);
				if(s != NULL) {
                                	STRING *string = newSTRING(s);
					printf("Frequency of ");
					displaySTRING(string, stdout);
					printf(": %d\n", freqGST(gst, string));
				} 
			}
			else if(strcmp(s, "s") == 0) {
				displayGST(gst, stdout);
			}
			else if(strcmp(s, "r") == 0) {
				statisticsGST(gst, stdout);
			}
                        if(stringPending(commands)) {
                                s = readString(commands);
                        }
                        else {
                                s = readToken(commands);
                        }
                }
		freeGST(gst);
		free(s);
		fclose(data);
		fclose(commands);
		return 0;
	}
	else {
                RBT *rbt = newRBT(compareSTRING);
                setRBTdisplay(rbt, displaySTRING);
                setRBTfree(rbt, freeSTRING);
                char *s = NULL;
                if(stringPending(data)) {
                        s = readString(data);
                }
                else {
                        s= readToken(data);
                }
                while(!feof(data)) {
                        s = cleanstring(s);
			if(s != NULL) {
                        	STRING *str = newSTRING(s);
                        	insertRBT(rbt, str);
			}
                        if(stringPending(data)) {
                                s = readString(data);
                        }
                        else {
                                s = readToken(data);
                        }
                }
                if(stringPending(commands)) {
                        s = readString(commands);
                }
                else {
                        s = readToken(commands);
                }
                while(!feof(commands)) {
                        s = cleanstring(s);
                        if(strcmp(s, "i") == 0) {
                                if(stringPending(commands)) {
                                        s = readString(commands);
                                }
                                else {
                                        s = readToken(commands);
                                }
                                s = cleanstring(s);
				if(s != NULL) {
                                	STRING *string = newSTRING(s);
                                	insertRBT(rbt, string);
				}
                        }
                        else if(strcmp(s, "d") == 0) {
                                if(stringPending(commands)) {
                                        s = readString(commands);
                                }
                                else {
                                        s = readToken(commands);
                                }
                                s = cleanstring(s);
				if(s != NULL) {
                                	STRING *string = newSTRING(s);
                                	if(deleteRBT(rbt, string) == -1) {
						printf("Value ");
						displaySTRING(string, stdout);
						printf(" not found.\n");
					}
				}
                        }
                        else if(strcmp(s, "f") == 0) {
                                if(stringPending(commands)) {
                                        s = readString(commands);
                                }
                                else {
                                        s = readToken(commands);
                                }
                                s = cleanstring(s);
				if(s != NULL) {
                              		STRING *string = newSTRING(s);
                                	printf("Frequency of ");
                                	displaySTRING(string, stdout);
                                	printf(": %d\n", freqRBT(rbt, string));
				}
                        }
                        else if(strcmp(s, "s") == 0) {
                                displayRBT(rbt, stdout);
                        }
                        else if(strcmp(s, "r") == 0) {
                                statisticsRBT(rbt, stdout);
                        }
                        if(stringPending(commands)) {
                                s = readString(commands);
                        }
                        else {
                                s = readToken(commands);
                        }
                }
                freeRBT(rbt);
                free(s);
                fclose(data);
                fclose(commands);
                return 0;
        }
		
}

char *cleanstring(char *str) {
	unsigned int i=0;
	unsigned int j=0;
	while(i < strlen(str)) {
		if(isalpha(str[i])) {
			if(str[i] >= 'A' && str[i] <= 'Z') {
				str[i] = str[i] + 32;
			}
			str[j] = str[i];
			++j;
		}
		else if(str[i] == ' ' && i > 0 && ((str[i-1] >= 'A' && str[i-1] <= 'Z') || (str[i-1] >= 'a' && str[i-1] <= 'z')) && i < strlen(str) && ((str[i+1] >= 'A' && str[i+1] <= 'Z') || (str[i+1] >= 'a' && str[i+1] <= 'z'))) {
			str[j] = str[i];
			++j;
		}
		++i;
	}
	str[j] = '\0';
	if(str[0] == '\0') {
		return NULL;
	}
	return str;
}
