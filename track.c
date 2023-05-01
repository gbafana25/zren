#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "track.h"

void initBaseObject(baseobject *bo, char *data) {
	bo->data = (char*)malloc(sizeof(char)*strlen(data)+1);
	strcpy(bo->data, data);
	bo->pos = 0;
}

void initStrObject(strobject *so, char *data, strobject* n, int len) {
	so->data = (char*)malloc(sizeof(char)*len+1);
	strcpy(so->data, data);
	so->next = n;	
	so->len = len;
}

void initSpaceObject(spaceobject *so, int num, spaceobject* n) {
	so->num_spaces = num;
	so->next = n;	
}

void insertStrObject(strobject *so, strobject *new) {
	strobject *temp = so->next;
	so->next = new;
	new->next = temp;

}

void insertSpaceAfter(spaceobject *so, spaceobject *new) {
	spaceobject *temp = so->next;
	so->next = new;
	new->next = temp;

}

void applyStrChange(char *current, strobject *so, int *p) {
	for(int i = 0; i < strlen(so->data); i++) {
		if(i < strlen(current)) {
			current[i] = so->data[i];		
		} else {
			current = (char*)realloc(current, strlen(current)+1);
			strcat(current, &so->data[i]);
		}
		*p+=1;
	}
	//printf("%d\n", *p);
}

void findDiff(char *base, char *current, commit *ca) {
	int size = (strlen(base) >= strlen(current)) ? strlen(base) : strlen(current);
	char buf[size];
	int array_count = 0;
	int bcount = 0;
	int spcount = 0;
	
	for(int i = 0; i < size; i++) {
		if(i < strlen(base) || i < strlen(current)) {
			while(base[i] != current[i] && i < strlen(base)) {
				//printf("%c", current[i]);
				//strcat(buf, &current[i]);	
				buf[bcount] = current[i];
				bcount += 1;
				i+=1;
			}
			if(strlen(buf) != 0) {
				initStrObject(&ca[array_count].str, buf, NULL, strlen(buf));
				ca[array_count].t = STR;
				array_count++;
				memset(&buf, 0, sizeof(buf));
				bcount = 0;
			}
			while(base[i] == current[i]) {
				//printf(" ");
				spcount += 1;
				i+=1;
			}
			if(strlen(buf) != 0) {
				initSpaceObject(&ca[array_count].space, spcount, NULL);
				ca[array_count].t = SPACE;
				array_count++;
				spcount = 0;
			}
		} else if(i >= strlen(base) || i >= strlen(current)) {
			printf("%c", current[i]);	
		}
	}
	printf("\n");
}

void printStrObject(strobject *so) {
	printf("%s\n", so->data);
}

void printBaseObject(baseobject *bo) {
	printf("----------------------\n");
	printf("%s\n", bo->data);
	printf("----------------------\n");
}

void printSpaces(spaceobject *so) {
	printf("%d\n", so->num_spaces);
}

