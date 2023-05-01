#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "track.h"

int main() {
	baseobject base;
	baseobject mod;
	strobject test_string;
	spaceobject spaces;
	// try to get rid of array, use pointer to next element in structs
	commit commit_array[50];
	
	/*
	char *test = "replacement";
	char *t2 = "another change\n";
	*/
	
	FILE *f = fopen("original", "r");
	fseek(f, 0L, SEEK_END);
	size_t s = ftell(f);
	rewind(f);

	// load original file
	char *src = (char*)malloc(sizeof(char)*s);
	fread(src, sizeof(char), s, f);
	src[strlen(src)-1] = '\0';
	initBaseObject(&base, src);
	//printBaseObject(&base);

	// load modified file
	FILE *f2 = fopen("file", "r");
	fseek(f2, 0L, SEEK_END);
	size_t s2 = ftell(f2);
	rewind(f2);

	char *m = (char*)malloc(sizeof(char)*s2);
	fread(m, sizeof(char), s2, f2);
	m[strlen(m)-1] = '\0';
	initBaseObject(&mod, m);
	//printBaseObject(&mod);

	// create commit	
	findDiff(base.data, mod.data, &commit_array);
	for(int i = 0; i < 100; i++) {
		if(commit_array[i].t == STR) {
			printf("string");
		} else if(commit_array[i].t == SPACE) {
			printf(" ");
		}
	}
	
	/*
	initStrObject(&test_string, test, NULL, strlen(test));
	initSpaceObject(&spaces, 5, NULL);
	applyStrChange(base.data, &test_string, &base.pos);

	insertSpaceAfter(&test_string, &spaces);
	printBaseObject(&base);
	*/
	

	
}
