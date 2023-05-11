#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "track.h"

void initBaseObject(baseobject *bo, char *data) {
	bo->data = (char*)malloc(sizeof(char)*strlen(data)+1);
	strcpy(bo->data, data);
	bo->pos = 0;
}

void initStrObject(strobject *so, char data, strobject* n, types tp) {
	so->data = data;
	so->next = n;	
	so->t = tp;
}


void insertStrObject(strobject *so, strobject *new) {
	strobject *temp = so->next;
	so->next = new;
	new->next = temp;

}


void findDiff(char *base, char *current, strobject *head) {	
	strobject *curr = head;	
	int blen = strlen(base);
	int clen = strlen(current);

	if(blen < clen) {
		for(int i = 0; i < clen; i++) {
			if(base[i] != current[i]) {
				strobject *ch = (strobject*)malloc(sizeof(strobject));
				initStrObject(ch, current[i], NULL, CHANGE);
				insertStrObject(curr, ch);
				curr = ch;
			} else {	
				strobject *ch = (strobject*)malloc(sizeof(strobject));
				initStrObject(ch, '\0', NULL, STAY);
				insertStrObject(curr, ch);
				curr = ch;
			}
		}
	} else {
		for(int i = 0; i < blen; i++) {
			if(base[i] != current[i]) {
				strobject *ch = (strobject*)malloc(sizeof(strobject));
				initStrObject(ch, current[i], NULL, CHANGE);
				insertStrObject(curr, ch);
				curr = ch;
			} else {
				strobject *ch = (strobject*)malloc(sizeof(strobject));
				initStrObject(ch, '\0', NULL, STAY);
				insertStrObject(curr, ch);
				curr = ch;
			}
		}
	}
}

void printStrObject(strobject *so) {
	printf("%s\n", so->data);
}

void printBaseObject(baseobject *bo) {
	printf("----------------------\n");
	printf("%s\n", bo->data);
	printf("----------------------\n");
}

