#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ignore.h"

char *openIgnoreFile() {
	FILE *ig = fopen(IGNORE_FILE, "r");
	fseek(ig, 0L, SEEK_END);
	size_t s = ftell(ig);
	rewind(ig);

	char *data = (char*)malloc(sizeof(char)*s);
	fread(data, sizeof(char), s, ig);
	data[strlen(data)-1] = '\0';
	return data;
}

bool inIgnore(char *filename, char **ign, int i_size) {
	for(int i = 0; i < i_size; i++) {
		if(strncmp(filename, ".", 1) == 0 || strncmp(filename, "..", 2) == 0) {
			//in_ignore = true;
			//break;
			return true;
		} else if(strcmp(ign[i], filename) == 0) {
			//in_ignore = true;
			//break; 
			return true;
		} else if(ign[i][0] == '*') {	
			int offset = strlen(filename)-(strlen(ign[i])-1);
			//printf("%s %s\n", file_list->d_name+offset, ign[i]+1);
			if(strncmp(filename+offset, ign[i]+1, strlen(ign[i])-1) == 0) {
				//in_ignore = true;
				//break;
				return true;
			}
		}
	}
	return false;
}

char **parseIgnoreFile(char *data, int *s) {
	int count = 0;
	int im_ind = 0;
	char **ign;
	// first get line count to allocate array
	int linecount = 0;
	for(int i = 0; i < strlen(data); i++) {
		if(data[i] == '\n' || data[i] == '\0') {
			linecount++;
		}
	}
	linecount+=1;
	ign = malloc(sizeof(char*)*linecount);
	*s = linecount;

	for(int i = 0; i < strlen(data); i++) {
		if(data[i] == '\0') {
			break;
		}
		int start = i;
		while(i < strlen(data)) {
			if(data[i] == '\n') {
				break;
			}
			count++;	
			i++;
		}
		if(count != 0) {
			
			ign[im_ind] = (char*)malloc(sizeof(char)*(count+1));
			strncpy(ign[im_ind], data+start, count);
			ign[im_ind][strlen(ign[im_ind])] = '\0';
		
			count = 0;
			im_ind++;
		}
	}
	return ign;
}

