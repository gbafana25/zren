#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

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

char *createBranchName(char *base, char *path, bool add_trailing_slash, bool add_middle_slash) {
	int base_len = strlen(base);
	int path_len = strlen(path);
	int total_len = base_len + path_len + 1;

	if(add_trailing_slash) {
		total_len += 1;
	}

	if(add_middle_slash) {
		total_len += 1;
	}

	char *full_path = (char*)malloc(sizeof(char)*total_len);
	memset(full_path, 0, sizeof(full_path));

	strcpy(full_path, base);
	if(add_middle_slash) {
		strcat(full_path, "/");
	}
	strcat(full_path, path);
	if(add_trailing_slash) {
		strcat(full_path, "/");
	}
	

	return full_path;
}

char* createFilename(char* name, char *ext, char *base, bool is_commit_file) {
	int base_len = strlen(base);
	int namelen = strlen(name);
	int extlen = strlen(ext);
	int total_len = base_len + namelen + extlen + 1;

	if(is_commit_file) {
		total_len += 1;
	}
	//printf("Base: %d, Name: %d, Extension: %d\n", base_len, namelen, extlen);
	//char *full_path = (char*)malloc(sizeof(char)*(strlen(base)+strlen(name)+strlen(ext)+1));
	char *full_path = (char*)malloc(sizeof(char)*total_len);
	//char full_path[base_len+namelen+extlen+1];
	memset(full_path, 0, sizeof(full_path));

	strcat(full_path, base);//, base_len+1);
	if(is_commit_file) {
		strcat(full_path, "/");
	}
	strcat(full_path, name);//, namelen+1);
	strcat(full_path, ext);//, extlen+1);

	return full_path;

}

