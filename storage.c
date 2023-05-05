#include <stdio.h>

#include "track.h"

// return success/failure code?
void writeCommit(strobject *head, char *filename) {
	// skip the head node
	strobject *n = head->next;

	// writing commit to file works
	FILE *commit = fopen(filename, "w+");
	
	int space_counter = 0;
	char strind = STR_IND;
	char spind = SPACE_IND;
	char endind = END_FILE;

	while(n != NULL) {
		if(n->t != STAY) {
			if(space_counter != 0) {
				// write space object
				fwrite(&spind, sizeof(char), 1, commit);
				fwrite(&space_counter, sizeof(int), 1, commit);
			}
			// write string object
			space_counter = 0;
			fwrite(&strind, sizeof(strind), 1, commit);
			fwrite(n, sizeof(strobject), 1, commit);
		} else {
			space_counter+=1;	
		}
		n = n->next;
	}
	// write custom end of file character
	fwrite(&endind, sizeof(endind), 1, commit);	
	fclose(commit);
}

/*
TODO: directly apply to base, or build linked list?
pass either as parameter
*/
void readCommit(char *filename, baseobject *bo) {
	FILE *rtest = fopen("commit", "r");
	bo->pos = 0;
	// for reading, dont initialize as pointer
	strobject re;
	int num_spaces;
	char c;
	// loop exit triggered by ending hex character 
	while(1) {
		fread(&c, sizeof(char), 1, rtest);
		if(c == STR_IND) {
			fread(&re, sizeof(strobject), 1, rtest);
			//printf("%c\n", re.data);
			bo->data[bo->pos] = re.data;
			bo->pos+=1;
		} else if(c == SPACE_IND) {
			fread(&num_spaces, sizeof(int), 1, rtest);
			//printf("%d\n", num_spaces);
			bo->pos += num_spaces;
			num_spaces = 0;
		} else if(c == END_FILE) {
			break;
		}
		
	}	
	
	fclose(rtest);
	printf("%s\n", bo->data);

}
