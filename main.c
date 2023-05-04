#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "track.h"

int main() {
	baseobject base;
	baseobject mod;
	//strobject test_string;
	
		
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
	strobject *head = (strobject*)malloc(sizeof(strobject));
	strobject *t = (strobject*)malloc(sizeof(strobject));

	initStrObject(head, 'H', NULL, CHANGE); 
	findDiff(base.data, mod.data, head);
	// skip the head node
	strobject *n = head->next;

	// writing commit to file works
	FILE *commit = fopen("commit", "w+");
	
	int space_counter = 0;
	char strind = 0x16;
	char spind = 0x15;
	while(n != NULL) {
		if(n->t != STAY) {
			if(space_counter != 0) {
				//printf("%d\n", space_counter);
				// write space object
				fwrite(&spind, sizeof(char), 1, commit);
				fwrite(&space_counter, sizeof(int), 1, commit);
			}
			//printf("%c\n", n->data);
			space_counter = 0;
			fwrite(&strind, sizeof(strind), 1, commit);
			fwrite(n, sizeof(strobject), 1, commit);
		} else {
			space_counter+=1;	
		}
		n = n->next;
	}
	
	fclose(commit);	

	FILE *rtest = fopen("commit", "r");
	// for reading, dont initialize as pointer
	strobject re;
	int num_spaces;
	char c;
	// figure out how to terminate while loop
	for(int i = 0; i < 3; i++) {
		fread(&c, sizeof(char), 1, rtest);
		if(c == 0x16) {
			//printf("string\n");
			fread(&re, sizeof(strobject), 1, rtest);
			printf("%c\n", re.data);
		} else if(c == 0x15) {
			//printf("space\n");
			fread(&num_spaces, sizeof(int), 1, rtest);
			printf("%d\n", num_spaces);
			num_spaces = 0;
		}
	}	
	
	fclose(rtest);
}
