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

