#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>

#include "track.h"

#define REPO_DIR ".rep"
#define BASE_DIR ".rep/base/"
#define COMM_DIR ".rep/commits/"
#define FILE_REG ".rep/FILES"
#define DIR_FILE ".rep/PROJECT_DIR"
#define IDENT_FILE ".rep/COMMIT_IDENTS"
#define TIMESTAMP "TIMESTAMP"
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

void addCommitMessage(char *msg, char *commit_id) {
	//printf("%s\n", msg);
	char *fname = "MSG";
	char path[strlen(COMM_DIR)+strlen(commit_id)+strlen(fname)+2];
	memset(&path, 0, sizeof(path));
	strcat(path, COMM_DIR);
	strcat(path, commit_id);
	strcat(path, "/");
	strcat(path, fname);
	path[strlen(path)] = '\0';
	
	//printf("%s\n", path);
	FILE *m = fopen(path, "w+");
	fprintf(m, "%s", msg);
	fclose(m);

}

int *getTimeStamp(char *cid) {
	char *full = (char*)malloc(sizeof(char)*ID_LEN+(strlen(COMM_DIR)+strlen(TIMESTAMP)+2));
	//memset(&full, 0, sizeof(full));
	full[0] = '\0';
	
	strcat(full, COMM_DIR);
	strcat(full, cid);
	strcat(full, "/");
	strcat(full, TIMESTAMP);
	full[strlen(full)] = '\0';

	//printf("%s\n", full);
	FILE *tfile = fopen(full, "r");

	int *cr_date = (int*)malloc(sizeof(int)*6);
	//int cr_date[6];
	
	fscanf(tfile, "%d-%d-%d %d-%d-%d", &cr_date[0], &cr_date[1], &cr_date[2], &cr_date[3], &cr_date[4], &cr_date[5]);
	//printf("%d %d %d, %d %d %d\n", cr_date[0], cr_date[1], cr_date[2], cr_date[3], cr_date[4], cr_date[5]);
	
	
	fclose(tfile);
	return cr_date;
}

void addTimeStamp(char *cid) {	
	char *full = (char*)malloc(sizeof(char)*ID_LEN+(strlen(COMM_DIR)+strlen(TIMESTAMP)+2));
	
	strcat(full, COMM_DIR);
	strcat(full, cid);
	strcat(full, "/");
	strcat(full, TIMESTAMP);
	full[strlen(full)] = '\0';

	time_t t = time(NULL);
	struct tm *curr_time;
	curr_time = localtime(&t);

	FILE *tfile = fopen(full, "w+");
	// day-month-year hour-minute-second
	fprintf(tfile, "%d-%d-%d %d-%d-%d", curr_time->tm_mday,  curr_time->tm_mon, curr_time->tm_year+1900, curr_time->tm_hour, curr_time->tm_min, curr_time->tm_sec);
	fclose(tfile);
	
	free(full);
}

void revertToCommit(char *cid) {
	DIR *dirobj = opendir(COMM_DIR);	
	struct dirent *commits = readdir(dirobj);

	int *cr_date;
	char *full_commit;
	char *second_full;
	while(commits != NULL) {
		if(strncmp(cid, commits->d_name, strlen(cid)) == 0) {
			printf("reverting to commit %s...\n", commits->d_name);	
			FILE *head = fopen(".rep/HEAD", "w+");
			fprintf(head, "%s", commits->d_name);
			fclose(head);
			cr_date = getTimeStamp(commits->d_name);

			printf("%d %d %d, %d %d %d\n", cr_date[0], cr_date[1], cr_date[2], cr_date[3], cr_date[4], cr_date[5]);
			full_commit = (char*)malloc(sizeof(char)*(strlen(commits->d_name)+1));
			strcpy(full_commit, commits->d_name);
			
			break;
		}
		commits = readdir(dirobj);
	}
	closedir(dirobj);
	// do another pass over directory to delete folders
	
	DIR *second = opendir(COMM_DIR);
	struct dirent *scnd = readdir(second);
	while(scnd != NULL) {
		second_full = (char*)malloc(sizeof(char)*(strlen(COMM_DIR)+strlen(scnd->d_name)+1));
		strcpy(second_full, COMM_DIR);
		strcat(second_full, scnd->d_name);
		if(strcmp(full_commit, scnd->d_name) != 0 && strcmp(scnd->d_name, ".") != 0 && strcmp(scnd->d_name, "..") != 0) {
			int *this_date = getTimeStamp(scnd->d_name);
			printf("%d %d %d, %d %d %d\n", this_date[0], this_date[1], this_date[2], this_date[3], this_date[4], this_date[5]);

			//printf("%s\n", second_full);
			if(cr_date[2] < this_date[2]) {
				DIR *sf = opendir(second_full);	
				struct dirent *old = readdir(sf);
				while(old != NULL) {
					if(old->d_type == DT_REG) {
						char *d = (char*)malloc(sizeof(char)*(strlen(second_full)+strlen(old->d_name)+2));
						strcpy(d, second_full);
						strcat(d, "/");
						strcat(d, old->d_name);
						unlink(old->d_name);
						free(d);
					}
					old = readdir(sf);
				}	
				rmdir(second_full);
				return;
			} else if(cr_date[1] < this_date[1]) {
				DIR *sf = opendir(second_full);	
				struct dirent *old = readdir(sf);
				while(old != NULL) {
					if(old->d_type == DT_REG) {
						char *d = (char*)malloc(sizeof(char)*(strlen(second_full)+strlen(old->d_name)+2));
						strcpy(d, second_full);
						strcat(d, "/");
						strcat(d, old->d_name);
						unlink(old->d_name);
						free(d);
					}
					old = readdir(sf);
				}
				rmdir(second_full);
				return;
			} else if(cr_date[0] < this_date[0]) {
				DIR *sf = opendir(second_full);	
				struct dirent *old = readdir(sf);
				while(old != NULL) {
					if(old->d_type == DT_REG) {
						char *d = (char*)malloc(sizeof(char)*(strlen(second_full)+strlen(old->d_name)+2));
						strcpy(d, second_full);
						strcat(d, "/");
						strcat(d, old->d_name);
						unlink(old->d_name);
						free(d);
					}
					old = readdir(sf);
				}
				rmdir(second_full);
				return;
			} else if(cr_date[3] < this_date[3]) {
				DIR *sf = opendir(second_full);	
				struct dirent *old = readdir(sf);
				while(old != NULL) {
					if(old->d_type == DT_REG) {
						char *d = (char*)malloc(sizeof(char)*(strlen(second_full)+strlen(old->d_name)+2));
						strcpy(d, second_full);
						strcat(d, "/");
						strcat(d, old->d_name);
						unlink(old->d_name);
						free(d);
					}
					old = readdir(sf);
				}
				rmdir(second_full);
				return;
			} else if(cr_date[4] < this_date[4]) {
				DIR *sf = opendir(second_full);	
				struct dirent *old = readdir(sf);
				while(old != NULL) {
					if(old->d_type == DT_REG) {
						char *d = (char*)malloc(sizeof(char)*(strlen(second_full)+strlen(old->d_name)+2));
						strcpy(d, second_full);
						strcat(d, "/");
						strcat(d, old->d_name);
						unlink(old->d_name);
						free(d);
					}
					old = readdir(sf);
				}
				rmdir(second_full);
				return;
			} else if(cr_date[5] < this_date[5]) {
				DIR *sf = opendir(second_full);	
				struct dirent *old = readdir(sf);
				
				while(old != NULL) {
					if(old->d_type == DT_REG) {
						char *d = (char*)malloc(sizeof(char)*(strlen(second_full)+strlen(old->d_name)+2));
						strcpy(d, second_full);
						strcat(d, "/");
						strcat(d, old->d_name);
						unlink(d);
						free(d);
					}
					old = readdir(sf);
				}
				rmdir(second_full);
				return;
			}
		}
		scnd = readdir(second);
		memset(&second_full, 0, sizeof(second_full));
	}
	closedir(second);

	printf("Error: could not find commit starting with %s...\n", cid);
}

char *getProjectDir() {
		
	FILE *d = fopen(DIR_FILE, "r");
	fseek(d, 0L, SEEK_END);
	size_t s = ftell(d);
	rewind(d);
	//char dir[s+1];
	char *dir = (char*)malloc(sizeof(char)*(s+1));
	//fread(dir, sizeof(char), s, d);
	fscanf(d, "%s", dir);

	fclose(d);
	return dir;
}

void setProjectDir(char *dirname) {
	/*
	char projdir[256];
	getcwd(projdir, 256);
	*/
	FILE *d = fopen(DIR_FILE, "w+");
	fprintf(d, "%s", dirname);
	fclose(d);
}

void createFileRegistry(char *repo_dir) {
	DIR *dirobj = opendir(repo_dir);	
	struct dirent *file_list = readdir(dirobj);	

	FILE *reg = fopen(FILE_REG, "w+");
	
	while(file_list != NULL) {
		fprintf(reg, "%s\n", file_list->d_name);
		file_list = readdir(dirobj);
	}

	fclose(reg);
		
}

// add any new files to base (before commit)
void addFile(char *name) {
	FILE *reg = fopen(FILE_REG, "a");
	
	fprintf(reg, "%s\n", name);
	fclose(reg);
	copyFile(name);
	
	
}

void readCommitFile(char *filename, baseobject *bo) {
	FILE *rtest = fopen(filename, "r");
	if(rtest != NULL) {

		fseek(rtest, 0L, SEEK_END);
		size_t s = ftell(rtest);
		rewind(rtest);

		if(s != strlen(bo->data)) {
			bo->data = (char*)realloc(bo->data, sizeof(char)*(strlen(bo->data)+s+1));
		}
		bo->pos = 0;
		char re;
		int num_spaces;
		char c;
		// loop exit triggered by ending hex character 
		while(1) {
			fread(&c, sizeof(char), 1, rtest);
			if(c == STR_IND) {
				fread(&re, sizeof(char), 1, rtest);
				bo->data[bo->pos] = re;
				bo->pos+=1;
			} else if(c == SPACE_IND) {
				fread(&num_spaces, sizeof(int), 1, rtest);
				bo->pos += num_spaces;
				num_spaces = 0;
			} else if(c == END_FILE) {
				break;
			}
		
		}	
	
		fclose(rtest);
		//bo->data[strlen(bo->data)-1] = '\0';
	}

}

// return success/failure code?
void writeCommitFile(strobject *head, char *filename, int res) {
	if(res == -1) {
		return;
	}
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

int getBaseFile(char *filename, baseobject *base) {
	FILE *f = fopen(filename, "r");
	if(f == NULL) {
		printf("%s not found, skipping..\n", filename);
		return -1;
	}
	fseek(f, 0L, SEEK_END);
	size_t s = ftell(f);
	rewind(f);

	char src[s+1];
	fread(src, sizeof(char), s, f);
	initBaseObject(base, src);
	fclose(f);
	return 0;
}

char *getMostRecent(baseobject *bo, char *base_name, char *curr_commit) {	
	DIR *old = opendir(".rep/commits/");
	struct dirent *commits = readdir(old);
	char *ex1 = ".";
	char *ex2 = "..";
	char *newext = "chg";
	while(commits != NULL) {
		if(strncmp(commits->d_name, ex1, 1) == 0 || strncmp(commits->d_name, ex2, 2) == 0 || strncmp(commits->d_name, curr_commit, strlen(curr_commit)) == 0) {
			commits = readdir(old);
			continue;

		} if(commits->d_type == DT_DIR) {
			// apply commit to baseobject
			
			// build path
			size_t s = strlen(COMM_DIR)+strlen(commits->d_name)+strlen(base_name)+strlen(newext)+3; 
			char full_path[s];
			strncat(full_path, COMM_DIR, strlen(COMM_DIR));
			strncat(full_path, commits->d_name, strlen(commits->d_name));
			strcat(full_path, "/");
			strncat(full_path, base_name, strlen(base_name)-3);
			strcat(full_path, newext);

			readCommitFile(full_path, bo);		
			//full_path[strlen(full_path)-1] = '\0';

			commits = readdir(old);
			full_path[0] = '\0';
			
		} 	
	}
	closedir(old);
	//free(full_path);
	return bo->data;
}

void createCommit(char **ign, int i_size, char *commit_msg) {
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

	FILE *hd = fopen(".rep/HEAD", "w");
	fprintf(hd, "%s", cid);
	fclose(hd);
			
	printf("Creating commit %s: %s...\n", cid, commit_msg);
	addCommitMessage(commit_msg, cid);
	char *scandir = getProjectDir();	
	//printf("%s\n", scandir);
		
	DIR *dirobj = opendir(BASE_DIR);	
	struct dirent *file_list = readdir(dirobj);
	addTimeStamp(cid);	

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
						printf("ignoring %s (Rule: ignore %s)...\n", file_list->d_name, ign[i]);
						printf("\n");
						end = true;

					}
				} else if(strncmp(file_list->d_name, ign[i], strlen(file_list->d_name)-4) == 0) {
					printf("ignoring %s (Rule: ignore %s)...\n", file_list->d_name, ign[i]);
					printf("\n");
					end = true;
				}
			}
			
			if(end == false) {
				// build full path to base file
				//printf("%s\n", file_list->d_name);
				strobject head;
				initStrObject(&head, 'H', NULL, CHANGE); 
				char full[strlen(BASE_DIR)+strlen(file_list->d_name)+1];
				memset(&full, 0, sizeof(full));
				strcat(full, BASE_DIR);
				strcat(full, file_list->d_name);
				//full[strlen(full)-1] = '\0';

				// 1. Get base (or most recent)
				baseobject b;	
				getBaseFile(full, &b);	
				// if base isn't most recent
				if(is_base == false) {
					createFileRegistry(scandir);
					char *latest = getMostRecent(&b, file_list->d_name, cid);
					// 2. get modified
			
					file_list->d_name[strlen(file_list->d_name)-4] = '\0';
					baseobject mod;
			
					int r = getBaseFile(file_list->d_name, &mod);

					findDiff(latest, mod.data, &head);
			
					char *ext = ".chg";
					commitfile = (char*)realloc(commitfile, sizeof(char)*(strlen(full_cpath)+strlen(file_list->d_name)+strlen(ext)+4));

					strcat(commitfile, full_cpath);
					strcat(commitfile, "/");
					strcat(commitfile, file_list->d_name);
					strcat(commitfile, ext);
				
					
					printf("Applying changes to %s\n", file_list->d_name);
					writeCommitFile(&head, commitfile, r);		
					// only this works for clearing commitfile
					commitfile[0] = '\0';
				} else {


					// 2. get modified
			
					file_list->d_name[strlen(file_list->d_name)-4] = '\0';
					baseobject mod;
			
					int r = getBaseFile(file_list->d_name, &mod);

					
					findDiff(b.data, mod.data, &head);
			
					char *ext = ".chg";
					commitfile = (char*)realloc(commitfile, sizeof(char)*(strlen(full_cpath)+strlen(file_list->d_name)+strlen(ext)+4));

					strcat(commitfile, full_cpath);
					strcat(commitfile, "/");
					strcat(commitfile, file_list->d_name);
					strcat(commitfile, ext);
					//commitfile[strlen(commitfile)-1] = '\0';
				
					printf("Applying %s\n", file_list->d_name);
					writeCommitFile(&head, commitfile, r);		
					// only this works for clearing commitfile
					commitfile[0] = '\0';
				}
			}	
		}
		file_list = readdir(dirobj);
	}
	
	closedir(dirobj);
	//free(file_list);
	//free(commitfile);
}


void copyFile(char *filename) {
	// create new filename
	char *ext = ".bas";
	char *base_copy = (char*)malloc(sizeof(char)*(strlen(BASE_DIR)+strlen(filename)+strlen(ext)+1));

	strcpy(base_copy, BASE_DIR);
	strcat(base_copy, filename);
	strcat(base_copy, ext);

	// load original file
	FILE *f = fopen(filename, "r");
	if(f == NULL) {
		printf("%s not found, skipping..\n", filename);
		return;
	}
	fseek(f, 0L, SEEK_END);
	size_t s = ftell(f);
	rewind(f);

	printf("Copying: %s | Size: %d\n", filename, s);
	
	char src[s+1];
	fread(src, sizeof(char), s, f);
	src[strlen(src)-1] = '\0';
	fclose(f);
		
	// create new base version
	FILE *b = fopen(base_copy, "w+");
	fwrite(src, sizeof(char), s, b);
	fclose(b);

	
}

void initRepository(char *dirname, char **ign, int i_size) {
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
		createFileRegistry(dirname);
		setProjectDir(dirname);
	}
	while(file_list != NULL) {	
		// only track files in dir base for now
		if(file_list->d_type == DT_REG) {
			// do intial scan for files, put name into FILES
			// copy over file (w/ different extension
			bool copy = true;	
			for(int i = 0; i < i_size; i++) {
				//printf("%s: %s\n", file_list->d_name, ign[i]);
				int offset = strlen(file_list->d_name)-(strlen(ign[i])-1);
				
				if(ign[i][0] == '*') {
					if(strncmp(file_list->d_name+offset, ign[i]+1, strlen(ign[i])-1) == 0) {	
						copy = false;
						break;
					}
				} else if(strcmp(file_list->d_name, ign[i]) == 0) {
					copy = false;	
					break;
				}
			}
			if(copy) {
				copyFile(file_list->d_name);
			}
		}
		file_list = readdir(dirobj);
	}
	
	closedir(dirobj);
}

