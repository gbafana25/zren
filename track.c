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
	so->next = (struct strobject *)n;	
	so->t = tp;
}


void insertStrObject(strobject *so, strobject *new) {
	strobject *temp = (struct strobject_struct *)so->next;
	so->next = (struct strobject *)new;
	new->next = (struct strobject *)temp;

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

char* createFilename(char* name, char *ext, char *base) {
	int base_len = strlen(base);
	int namelen = strlen(name);
	int extlen = strlen(ext);
	printf("Base: %d, Name: %d, Extension: %d\n", base_len, namelen, extlen);

	//char *full_path = (char*)malloc((base_len+namelen+extlen+1)*sizeof(char));
	char *full_path = (char*)malloc(sizeof(char)*(strlen(base)+strlen(name)+strlen(ext)+1));
	//printf("here\n");
	//char full_path[base_len+namelen+extlen+1];
	memset(full_path, 0, sizeof(full_path));

	strcat(full_path, base);//, base_len+1);
	strcat(full_path, name);//, namelen+1);
	strcat(full_path, ext);//, extlen+1);

	return full_path;

}

