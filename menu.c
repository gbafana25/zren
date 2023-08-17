#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>

#include "menu.h"
#include "storage.h"
#include "log.h"

void printHelpMenu() {
	printf("zren help menu\n\n");
	printf("Available commands:\n\n");
	printf("init [directory] - initialize a new repository in the specified directory\n");
	printf("status - show files currently staged for commit\n");
	printf("commit [message] - save current changes\n");
	printf("add [filename] - add file to be tracked\n");
	printf("rollback [commit_id] - set head to given commit\n");
	printf("checkout [commit_id] - switch to specified branch\n");
	printf("log [msg|id|time] - show repository log, with optional flags to filter options\n");
	printf("remote [url] - set url for remote repository");
	printf("stage [filename] or --all - move file(s) to staging area to be committed\n\n");
	

}

void printStage(char **ign, int i_size) {
	FILE *stage = fopen(".rep/STAGE", "r");
	if(stage == NULL) {
		printf("Error: couldn't find stage file\n");
		return;
	}
	char *branch = getCurrentBranch();
	char logpath[strlen(LOG_DIR)+strlen(branch)+1];
	memset(&logpath, 0, sizeof(logpath));
	strcat(logpath, LOG_DIR);	
	strcat(logpath, branch);
	char name[256];
	printf("Currently staged files:\n\n");
	while(fscanf(stage, "%s\n", name) != -1) {
		printf("\t\033[;32m%s\033[0m\n", name);
			
		memset(&name, 0, sizeof(name));
	}
	printf("\n");
	printf("Changed files that are not staged:\n\n");
	DIR *d = opendir(".");
	struct dirent *file_list = readdir(d);
	while(file_list != NULL) {
		bool in_ignore = false;
		for(int i = 0; i < i_size; i++) {
			if(strncmp(file_list->d_name, ".", 1) == 0 || strncmp(file_list->d_name, "..", 2) == 0) {
				in_ignore = true;
				break;
			} else if(strcmp(ign[i], file_list->d_name) == 0) {
				in_ignore = true;
				break; 
			} else if(ign[i][0] == '*') {	
				int offset = strlen(file_list->d_name)-(strlen(ign[i])-1);
				//printf("%s %s\n", file_list->d_name+offset, ign[i]+1);
				if(strncmp(file_list->d_name+offset, ign[i]+1, strlen(ign[i])-1) == 0) {
					in_ignore = true;
					break;
				}
			}
		}
		if(!in_ignore) {
			struct stat st;
			stat(file_list->d_name, &st); 
			// get most recent commit
			
			FILE *log = fopen(logpath, "r");
			char n[256];
			char m[256];
			time_t t;
			while(fscanf(log, "%s %d %s\n", n, &t, m) != -1) {
				// get to last		
			}
			//printf("%d\n", t);
			fclose(log);

			//printf("%s %d\n", file_list->d_name, st.st_mtim.tv_sec); 
			if(st.st_mtim.tv_sec > t && !isAlreadyStaged(file_list->d_name)) {
				printf("\t\033[;31m%s\033[0m\n", file_list->d_name);
			}
			
		}
		
		file_list = readdir(d);
	}
	printf("\n");
}

