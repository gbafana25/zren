#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>

#include "track.h"

#define REPO_DIR ".rep"
#define BASE_DIR ".rep/base/"
#define COMM_DIR ".rep/commits/"
#define ID_LEN 25

void genCommitId(char *id) {
	srand(time(0));
	char *alphabet = "abcdefghijklmnopqrstuvwxyz";
	for(int i = 0; i < ID_LEN; i++) {
		int ind = (rand() % strlen(alphabet));
		id[i] = alphabet[ind];
	}
	id[strlen(id)-1] = '\0';

}

// return success/failure code?
void writeCommitFile(strobject *head, char *filename) {
	// skip the head node
	strobject *n = head->next;
	//printf("%s\n", filename);

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
			//fwrite(n, sizeof(strobject), 1, commit);

			fwrite(&n->data, sizeof(char), 1, commit);
		} else {
			space_counter+=1;	
		}
		n = n->next;
	}
	// write custom end of file character
	fwrite(&endind, sizeof(endind), 1, commit);	
	fclose(commit);
}

void getBaseFile(char *filename, baseobject *base) {
	FILE *f = fopen(filename, "r");
	fseek(f, 0L, SEEK_END);
	size_t s = ftell(f);
	rewind(f);

	char *src = (char*)malloc(sizeof(char)*s);
	fread(src, sizeof(char), s, f);
	src[strlen(src)-1] = '\0';
	initBaseObject(base, src);
	fclose(f);
	//printf("%s\n", src);
}

void getMostRecent(baseobject *bo) {	
	printf("%s\n", bo->data);
}

void createCommit(char **ign, int i_size) {
	char cid[ID_LEN+1];
	char full_cpath[strlen(COMM_DIR)+ID_LEN+1];
	char *commitfile = (char*)malloc(sizeof(char)*4);
	memset(&full_cpath, 0, sizeof(full_cpath));
	memset(&cid, 0, sizeof(cid));
	
	genCommitId(cid);
	strcat(full_cpath, COMM_DIR);
	strcat(full_cpath, cid);
	//full_cpath[strlen(full_cpath)-1] = '\0';

	// get current commit (HEAD)
	bool is_base;
	FILE *head = fopen(".rep/HEAD", "r");
	char curr_commit[ID_LEN+1];
	fscanf(head, "%s", curr_commit);
	//printf("current: %s\n", curr_commit);
	fclose(head);
	if(strncmp("BASE", curr_commit, strlen("BASE")) == 0) {
		is_base = true;
	} else {
		is_base = false;
	}
	
	int d = mkdir(full_cpath, S_IRWXU);

	// keep generating commit id if current is duplicate
	while(d < 0) {
		memset(&cid, 0, sizeof(cid));
		memset(&full_cpath, 0, sizeof(full_cpath));
		genCommitId(cid);
		strcat(full_cpath, COMM_DIR);
		strcat(full_cpath, cid);
		full_cpath[strlen(full_cpath)-1] = '\0';

		d = mkdir(full_cpath, S_IRWXU);
	}

			
	printf("Creating commit %s...\n", cid);
	
		
	DIR *dirobj = opendir(BASE_DIR);	
	struct dirent *file_list = readdir(dirobj);
	

	while(file_list != NULL) {	
		// only track files in dir base for now
		if(file_list->d_type == DT_REG) {// && strcmp(file_list->d_name, "main") != 0) {
			// copy over file (w/ different extension
			//printf("%s\n", file_list->d_name);
			// read through each commit, applying changes to base, compare w/ most recent change and save	

			// check if in .ignore first
			bool end = false;
			for(int i = 0; i < i_size; i++) {
				if(ign[i][0] == '*') {
					// for file extensions
					int offset = strlen(file_list->d_name)-strlen(ign[i])-3;

					if(strncmp(file_list->d_name+offset, ign[i]+1, strlen(ign[i])-1) == 0) {
						printf("ignoring %s...\n", ign[i]);
						printf("\n");
						end = true;

					}
				} else if(strncmp(file_list->d_name, ign[i], strlen(file_list->d_name)-4) == 0) {
					printf("ignoring %s...\n", ign[i]);
					printf("\n");
					end = true;
				}
			}
			
			if(end == false) {
				// build full path to base file
				strobject head;
				initStrObject(&head, 'H', NULL, CHANGE); 
				char full[strlen(BASE_DIR)+strlen(file_list->d_name)+1];
				memset(&full, 0, sizeof(full));
				strcat(full, BASE_DIR);
				strcat(full, file_list->d_name);
				//full[strlen(full)-1] = '\0';
				//printf("%s\n", full);

				// 1. TODO: Get base (or most recent)
				baseobject b;	
				getBaseFile(full, &b);	
				// if base isn't most recent
				//getMostRecent(&b);

				// 2. get modified
			
				file_list->d_name[strlen(file_list->d_name)-4] = '\0';
				//printf("%s\n", file_list->d_name);
				baseobject mod;
			
				getBaseFile(file_list->d_name, &mod);
				//printf("%s\n", mod.data);

			
				findDiff(b.data, mod.data, &head);
			
				char *ext = ".chg";
				commitfile = (char*)realloc(commitfile, sizeof(char)*(strlen(full_cpath)+strlen(file_list->d_name)+strlen(ext)+4));
				//char commitfile[strlen(full_cpath)+strlen(file_list->d_name)+strlen(ext)+2];

				strcat(commitfile, full_cpath);
				strcat(commitfile, "/");
				strcat(commitfile, file_list->d_name);
				strcat(commitfile, ext);
				//commitfile[strlen(commitfile)-1] = '\0';
				printf("%s\n", commitfile);
			
				writeCommitFile(&head, commitfile);		
				// only this works for clearing commitfile
				commitfile[0] = '\0';
			}	
		}
		file_list = readdir(dirobj);
	}
	
	closedir(dirobj);
	//free(file_list);
	free(commitfile);
}



/*
directly apply to base, or build linked list?
*/
void readCommitFile(char *filename, baseobject *bo) {
	FILE *rtest = fopen(filename, "r");
	bo->pos = 0;
	// for reading, dont initialize as pointer
	//strobject re;
	char re;
	int num_spaces;
	char c;
	// loop exit triggered by ending hex character 
	while(1) {
		fread(&c, sizeof(char), 1, rtest);
		if(c == STR_IND) {
			fread(&re, sizeof(char), 1, rtest);
			//printf("%c\n", re.data);
			bo->data[bo->pos] = re;
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

void copyFile(char *filename) {
	// create new filename
	char *ext = ".bas";
	char *base_copy = (char*)malloc(sizeof(char)*strlen(BASE_DIR)+strlen(filename)+strlen(ext)+1);

	strcpy(base_copy, BASE_DIR);
	strcat(base_copy, filename);
	strcat(base_copy, ext);
	//printf("%s\n", base_copy);

	// load original file
	FILE *f = fopen(filename, "r");
	fseek(f, 0L, SEEK_END);
	size_t s = ftell(f);
	rewind(f);

	printf("%s: %d\n", base_copy, s);
	
	//char *src = (char*)malloc(sizeof(char)*s+1);
	char src[s+1];
	fread(src, sizeof(char), s, f);
	src[strlen(src)-1] = '\0';
	fclose(f);
		
	// create new base version
	FILE *b = fopen(base_copy, "w+");
	fwrite(src, sizeof(char), s, b);
	fclose(b);

	//free(src);
	free(base_copy);
	
}

void initRepository(char *dirname) {
	DIR *dirobj = opendir(dirname);	
	struct dirent *file_list = readdir(dirobj);
	int d = mkdir(REPO_DIR, S_IRWXU);
	if(d < 0) {
		printf("Repository already initialized for this directory.\n");
		printf("Exiting..\n");
		return;
	} else {
		mkdir(BASE_DIR, S_IRWXU);
		mkdir(COMM_DIR, S_IRWXU);
		// create file to store current commit
		FILE *head = fopen(".rep/HEAD", "w+");
		fprintf(head, "%s", "BASE");
		fclose(head);
	}
	while(file_list != NULL) {	
		// only track files in dir base for now
		if(file_list->d_type == DT_REG) {
			// copy over file (w/ different extension
			//printf("%s\n", file_list->d_name);
			copyFile(file_list->d_name);
		}
		file_list = readdir(dirobj);
	}
	
	closedir(dirobj);
}

