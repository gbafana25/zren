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
	while(n != NULL) {
		if(n->data != '\0') {
			printf("%c\n", n->data);
		}
		n = n->next;
	}
	
	

	
}
