#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <errno.h>

#include "track.h"
#include "log.h"
#include "ignore.h"

#define REPO_DIR ".rep"
#define BASE_DIR ".rep/base/"
#define COMM_DIR ".rep/branches/main/"
#define BRANCH_DIR ".rep/branches/"
#define FILE_REG ".rep/FILES"
#define DIR_FILE ".rep/PROJECT_DIR"
#define IDENT_FILE ".rep/COMMIT_IDENTS"
#define TIMESTAMP "TIMESTAMP"
#define ID_LEN 25


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

bool isAlreadyStaged(char *opt) {
	FILE *stage = fopen(".rep/STAGE", "a+");
	char n[256];
	while(fscanf(stage, "%s\n", n) != -1) {
		//printf("%s\n", n);
		if(strcmp(n, opt) == 0) {
			return true;
		}
	}
	return false;
}

// add files to staging area
void stageFiles(char **opt, char **ign, int i_size, int opt_size) {
	// just add one file
	FILE *stage;
	if(strcmp(opt[2], "--all") != 0) {	
		stage = fopen(".rep/STAGE", "a+");
		for(int i = 2; i < opt_size; i++) {
			int is_staged = isAlreadyStaged(opt[i]);
			if(!is_staged) {
				fprintf(stage, "%s\n", opt[i]);	
			}
		}
		fclose(stage);
		return;
	} else {
		stage = fopen(".rep/STAGE", "w+");
		char d[256];
		size_t dir_size = 256;
		getcwd(d, dir_size);
		DIR *dirobj = opendir(d);
		struct dirent *file_list = readdir(dirobj);
		bool ignore = false;
		while(file_list != NULL) {
			bool is_staged = isAlreadyStaged(file_list->d_name);
			bool in_ignore = inIgnore(file_list->d_name, ign, i_size);	
			if(!is_staged && in_ignore && file_list->d_type == DT_REG) {
				
				fprintf(stage, "%s\n", file_list->d_name);
				
			}
				
			file_list = readdir(dirobj);
		}
		closedir(dirobj);
		
	
		fclose(stage);
		FILE *sub = fopen(".rep/SUBDIRS", "r");
		if(sub == NULL) {
			printf("No subfolders...\n");
			return;
		}
		char dir_str[256];
		while(fscanf(sub, "%s\n", dir_str) != -1) {
			//getcwd(full_dir, sizeof(full_dir));		
			strcat(dir_str, "/");
				
			//printf("%s\n", dir_str);
			DIR *sdir = opendir(dir_str);
			struct dirent *subflist = readdir(sdir); 
			while(subflist != NULL) {
				bool is_staged = isAlreadyStaged(subflist->d_name);
				if(!is_staged && strcmp(subflist->d_name, ".") != 0 && strcmp(subflist->d_name, "..") != 0 && subflist->d_type == DT_REG) {
					strcat(dir_str, subflist->d_name);
					//printf("%s\n", dir_str);
					FILE *st = fopen(".rep/STAGE", "a+");
					fprintf(st, "%s\n", dir_str);
					fclose(st);
					//printf("%s\n", subflist->d_name);
				}
				subflist = readdir(sdir);
			}
			memset(&dir_str, 0, sizeof(dir_str));
		
			
		}
		fclose(sub);

		return;
	}
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
	strobject *n = (struct strobject_struct *)head->next;

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
		n = (struct strobject_struct *)n->next;
	}
	// write custom end of file character
	fwrite(&endind, sizeof(endind), 1, commit);	
	fclose(commit);
}

int getBaseFile(char *filename, baseobject *base) {
	//printf("%s\n", filename);
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

char *getMostRecent(baseobject *bo, char *base_name, char *curr_commit, char *branch) {	
	char dirpath[strlen(BRANCH_DIR)+strlen(branch)+1];	
	memset(&dirpath, 0, sizeof(dirpath));
	strcat(dirpath, BRANCH_DIR);
	strcat(dirpath, branch);
	DIR *old = opendir(dirpath);
	struct dirent *commits = readdir(old);
	char *ex1 = ".";
	char *ex2 = "..";
	char *newext = ".chg";
	while(commits != NULL) {
		if(strncmp(commits->d_name, ex1, 1) == 0 || strncmp(commits->d_name, ex2, 2) == 0 || strncmp(commits->d_name, curr_commit, strlen(curr_commit)) == 0) {
			commits = readdir(old);
			continue;

		} if(commits->d_type == DT_DIR) {
			// apply commit to baseobject
			
			// build path
			size_t s = strlen(BRANCH_DIR)+strlen(commits->d_name)+strlen(base_name)+strlen(newext)+3; 
			char full_path[s];
			strncat(full_path, BRANCH_DIR, strlen(BRANCH_DIR));
			strncat(full_path, commits->d_name, strlen(commits->d_name));
			strcat(full_path, "/");
			strcat(full_path, base_name);
			strcat(full_path, newext);
			//printf("%s\n", full_path);

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
	char *branch = getCurrentBranch();
	char full_cpath[strlen(BRANCH_DIR)+strlen(branch)+ID_LEN+2];
	//char *commitfile = (char*)malloc(sizeof(char)*4);
	//char *commitfile;
	memset(&full_cpath, 0, sizeof(full_cpath));
	memset(&cid, 0, sizeof(cid));
	
	genCommitId(cid);
	strcat(full_cpath, BRANCH_DIR);
	strcat(full_cpath, branch);	
	strcat(full_cpath, "/");
	strcat(full_cpath, cid);
	//printf("%s\n", full_cpath);
	//full_cpath[strlen(full_cpath)-1] = '\0';

	// get current commit (HEAD)
	bool is_base;
	FILE *head = fopen(".rep/HEAD", "r");
	char curr_commit[ID_LEN+1];
	fscanf(head, "%s", curr_commit);
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
		strcat(full_cpath, BRANCH_DIR);
		strcat(full_cpath, branch);	
		strcat(full_cpath, "/");
		strcat(full_cpath, cid);
		full_cpath[strlen(full_cpath)-1] = '\0';

		d = mkdir(full_cpath, S_IRWXU);
	}

	
	FILE *hd = fopen(".rep/HEAD", "w");
	fprintf(hd, "%s %s", cid, branch);
	fclose(hd);
			
	printf("Creating commit %s: %s...\n", cid, commit_msg);
	//recordCommit(cid, commit_msg);
	if(strncmp(branch, "main", 4) == 0) {
		logAction(cid, commit_msg, "commit", MAIN_LOG);
	
	} else {
		char logpath[strlen(LOG_DIR)+strlen(branch)+1];
		memset(&logpath, 0, sizeof(logpath));
		strcat(logpath, LOG_DIR);
		strcat(logpath, branch);
		logAction(cid, commit_msg, "commit", logpath);
	}
	char *scandir = getProjectDir();	

	// create subdirs in commit first
	FILE *sb = fopen(".rep/SUBDIRS", "r");
	char subname[512];
	while(fscanf(sb, "%s\n", subname) != -1) {

		char csub[strlen(full_cpath)+strlen(subname)+2];
		memset(&csub, 0, sizeof(csub));
		strcat(csub, full_cpath);
		strcat(csub, "/");
		strcat(csub, subname);
		//printf("%s\n", csub);
		mkdir(csub, S_IRWXU); 


	}
		
	//DIR *dirobj = opendir(BASE_DIR);	
	//struct dirent *file_list = readdir(dirobj);
	FILE *stage = fopen(".rep/STAGE", "r");
	// max length for filenames
	char name[256];
	char *base_ext = ".bas";
	char *ext = ".chg";
	while(fscanf(stage, "%s\n", name) != -1) {
		// read through each commit, applying changes to base, compare w/ most recent change and save		
		
		// build full path to base file
		//printf("%s\n", file_list->d_name);
		strobject head;
		memset(&head, 0, sizeof(head));
		initStrObject(&head, 'H', NULL, CHANGE); 
		char full[strlen(BASE_DIR)+strlen(name)+strlen(base_ext)+1];
		memset(&full, 0, sizeof(full));
		strcat(full, BASE_DIR);
		strcat(full, name);
		strcat(full, base_ext);
			
		// 1. Get base (or most recent)
		baseobject b;	
		memset(&b, 0, sizeof(b));
		//printf("%s\n", full);
		
		getBaseFile(full, &b);	
													// 2. get modified
	
		baseobject mod;
		memset(&mod, 0, sizeof(mod));

		char commitfile[strlen(full_cpath)+strlen(name)+strlen(ext)+4];
		memset(&commitfile, 0, sizeof(commitfile));
		//commitfile = (char*)realloc(commitfile, sizeof(char)*(strlen(full_cpath)+strlen(name)+strlen(ext)+4));

		strcat(commitfile, full_cpath);
		strcat(commitfile, "/");
		strcat(commitfile, name);
		strcat(commitfile, ext);
		//printf("%s\n", commitfile);
		//printf("here %s\n", commitfile);
		if(is_base) {
			
			//full[strlen(full)-1] = '\0';

			
			int r = getBaseFile(name, &mod);

			
			findDiff(b.data, mod.data, &head);		
		
			printf("Applying %s\n", name);
			writeCommitFile(&head, commitfile, r);		
			// only this works for clearing commitfile
			commitfile[0] = '\0';		
		} else {
			//createFileRegistry(scandir);
			char *latest = getMostRecent(&b, name, cid, branch);
			//printf("%s\n", latest);
			int r = getBaseFile(name, &mod);
			findDiff(latest, mod.data, &head);

			printf("Applying %s\n", name);
			writeCommitFile(&head, commitfile, r);		
			// only this works for clearing commitfile
			commitfile[0] = '\0';
		}
		commitfile[0] = '\0';
		memset(&name, 0, sizeof(name));
		full[0] = '\0';
			
	}	

	fclose(stage);
	FILE *overwrite = fopen(".rep/STAGE", "w");
	fclose(overwrite);
	
	
}

void getSubdir(char *dir) {
	//printf("%s\n", dir);
	char b[512];
	DIR *sub = opendir(dir);
	//struct dirent *sub_list = readdir(sub);
	struct dirent *sub_list;
	if(!sub) {
		return;
	}
	while((sub_list = readdir(sub)) != NULL) {
		if(sub_list->d_type == DT_DIR && strcmp(sub_list->d_name, ".") != 0 && strcmp(sub_list->d_name, "..") != 0 && strcmp(sub_list->d_name, ".rep") && strcmp(sub_list->d_name, ".git") != 0) {
			//printf("%s\n", sub_list->d_name);
			FILE *dirlist = fopen(".rep/SUBDIRS", "a+");
			strcpy(b, dir);
			strcat(b, "/");
			strcat(b, sub_list->d_name);
			fprintf(dirlist, "%s\n", b);
			fclose(dirlist);
			getSubdir(b);
		} 
		
		
	}
	closedir(sub);
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
		mkdir(BRANCH_DIR, S_IRWXU);
		mkdir(COMM_DIR, S_IRWXU);
		mkdir(LOG_DIR, S_IRWXU);
		// create file to store current commit
		FILE *head = fopen(".rep/HEAD", "w+");
		// default branch will be main
		fprintf(head, "%s %s", "BASE", "main");
		fclose(head);
		createFileRegistry(dirname);
		setProjectDir(dirname);
		logBase();
	}
	while(file_list != NULL) {	
		// only track files in dir base for now
		
		
		if(file_list->d_type == DT_REG) {
			// do intial scan for files, put name into FILES
			// copy over file (w/ different extension
			bool copy = inIgnore(file_list->d_name, ign, i_size);;	
			
			if(copy) {
				copyFile(file_list->d_name);
			}
		}
		/*
			to track directories:
			have list of subdirs in .rep base
			go to separate recursive function hear that
			gathers folder names and builds full paths for each

			when creating commits, createCommit will read from 
			base dir and subdir list
		*/
		else if(file_list->d_type == DT_DIR && strcmp(file_list->d_name, ".") != 0 && strcmp(file_list->d_name, "..") != 0 && strcmp(file_list->d_name, ".git") != 0) {
			
			FILE *dirlist = fopen(".rep/SUBDIRS", "a+");
			fprintf(dirlist, "%s\n", file_list->d_name);
			fclose(dirlist);
			getSubdir(file_list->d_name);	
		}
		file_list = readdir(dirobj);
	}
	
	closedir(dirobj);
	FILE *dlist = fopen(".rep/SUBDIRS", "r");
	char dir_name[256];
	while(fscanf(dlist, "%s\n", dir_name) != -1) {
		DIR *f = opendir(dir_name);		
		struct dirent *file_list = readdir(f);
		while(file_list != NULL) {	
			char base_folder[strlen(BASE_DIR)+strlen(dir_name)+1];
			strcpy(base_folder, BASE_DIR);
			strcat(base_folder, dir_name);
			mkdir(base_folder, S_IRWXU);
		
			if(file_list->d_type == DT_REG) {
				// do intial scan for files, put name into FILES
				// copy over file (w/ different extension
				bool copy = inIgnore(file_list->d_name, ign, i_size);	
				if(copy) {
					char bse[strlen(dir_name)+strlen(file_list->d_name)+2]; 
					strcpy(bse, dir_name);
					strcat(bse, "/");
					strcat(bse, file_list->d_name);
					bse[strlen(bse)] = '\0';
					//printf("%s\n", bse);
					copyFile(bse);
				}
			}
			file_list = readdir(f);
		}

		//memset(&dir_name, 0, sizeof(dir_name));
	}
}

void rollbackToCommit(char *cid, char **ign, int i_size) {
	FILE *log = fopen(MAIN_LOG, "r");
	char current_dir[256];
	getcwd(current_dir, sizeof(current_dir));
	char msg[256];
	int t;
	char action[45];
	char id[25];
	char *commitfile = (char*)malloc(4*sizeof(char));
	char *ext = ".chg";
	char *base_ext = ".bas";
	bool is_current = false;
	char *prefix = "sub-";


	//printf("%s\n", full);
	char new_branch[strlen(BRANCH_DIR)+strlen(prefix)+10];

	while(fscanf(log, "%s %d %s %s\n", id, &t, action, msg) != -1) {
		if(strncmp(cid, id, strlen(cid)) == 0) {
			printf("%s\n", id);	
			
			// make new branch name
			strcpy(new_branch, BRANCH_DIR);
			strcat(new_branch, prefix);
			strncat(new_branch, id, 8);

			//printf("%s\n", new_branch+(strlen(new_branch)-12));
			// create new logfile
			char *logdir = ".rep/logs/";
			char newlog[strlen(logdir)+13];
			strcpy(newlog, logdir);
			strcat(newlog, new_branch+(strlen(new_branch)-12));
			//printf("%s\n", newlog);
			logAction(id, new_branch, "commit", newlog); 

			// log rollback in main logfile
			//printf("%s\n", getCurrentBranch());
			logAction(id, new_branch, "rollback", MAIN_LOG);
			
			strcat(new_branch, "/");

			rewind(log);
			mkdir(new_branch, S_IRWXU);
			break;

			/*	
			create new log file for new branch
			include starting commit id (in log or other file)
			*/

		}	
			
	}
	// copy changefiles from previous commits
	while(fscanf(log, "%s %d %s %s\n", id, &t, action, msg) != -1) {
			
		if(strcmp(id, "base") == 0) {
			// ignore base
			continue;
		} else {
			char new_ver[strlen(new_branch)+strlen(id)+1];
			memset(&new_ver, 0, sizeof(new_ver));
			strcat(new_ver, new_branch);
			strcat(new_ver, id);
			mkdir(new_ver, S_IRWXU);
			DIR *home = opendir("."); // project root
			struct dirent *hd = readdir(home);
			while(hd != NULL) {
				if(hd->d_type == DT_REG && inIgnore(hd->d_name, ign, i_size)) {	
					//printf("%s\n", hd->d_name);
					char fullchg[strlen(COMM_DIR)+ID_LEN+strlen(hd->d_name)+strlen(ext)+2];
					strcpy(fullchg, COMM_DIR);
					strcat(fullchg, id);
					strcat(fullchg, "/");
					strcat(fullchg, hd->d_name);
					strcat(fullchg, ext);
					//printf("%s\n", fullchg);
					FILE *old = fopen(fullchg, "r");
					if(old != NULL) {	
						fseek(old, 0L, SEEK_END);
						size_t size = ftell(old);
						rewind(old);
						char *buf = (char*)malloc((size+1)*sizeof(char));
						fread(buf, sizeof(char), size, old);
						fclose(old);

						// new location
						char new_chg[strlen(new_ver)+strlen(hd->d_name)+strlen(ext)+2];
						memset(&new_chg, 0, sizeof(new_chg));
						strcat(new_chg, new_ver);
						strcat(new_chg, "/");
						strcat(new_chg, hd->d_name);
						strcat(new_chg, ext);

						//printf("%s\n", new_chg);

						FILE *copy = fopen(new_chg, "w+");
						fwrite(buf, sizeof(char), size, copy);
						fclose(copy);

						memset(&buf, 0, sizeof(buf));
						memset(&new_chg, 0, sizeof(new_chg));
						memset(&fullchg, 0, sizeof(fullchg));
					}

				}
				hd = readdir(home);
			}
			closedir(home);
			if(strncmp(cid, id, strlen(cid)) == 0) {
				break;
			}
		}
	}
	
	//printf("%s\n", new_branch);
	
	
	fclose(log);
	
	//logAction(commits->d_name, msg, act);
}

void checkoutBranch(char *branch) {
	// check if branch exists
	DIR *b = opendir(".rep/branches/");
	struct dirent *brs = readdir(b);
	char full_branch[15];
	while(brs != NULL) {
		if(strncmp(brs->d_name+4, branch, strlen(branch)) == 0) {
			printf("%s\n", brs->d_name);
			strcpy(full_branch, brs->d_name);
			break;
		}
		brs = readdir(b);
	}
	char id[50];
	FILE *old_hd = fopen(".rep/HEAD", "r");
	fscanf(old_hd, "%s", id);
	printf("%s\n", id);
	fclose(old_hd);
	
	FILE *head = fopen(".rep/HEAD", "w");
	fprintf(head, "%s %s", id, full_branch);
	fclose(head);

}
