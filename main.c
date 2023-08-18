#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "track.h"
#include "storage.h" 
#include "ignore.h"
#include "menu.h"
#include "log.h"
#include "client.h"

int main(int argc, char **argv) 
{ 	

	if(argc == 1) {
		printHelpMenu();
		exit(0);
	} else if(strcmp("log", argv[1]) == 0) {		
		outputLogfile(argv, argc);
		exit(0);
	} else if(strcmp("client", argv[1]) == 0) {
		char url[128];
		getRemote(url);
		//printf("%s\n", url);
		localRepoInfo info = getCurrentCommit();	
		if(strcmp(info.id, "BASE") == 0) {
			packBase(&info);
		} else {
			packDir(&info);
		}
	
		//printf("%s %d\n", info.id, info.timestamp);
		sendCommitInfo(&info, url);
		exit(0);
	} else if(strcmp("remote", argv[1]) == 0) {
		setRemote(argv[2]);	
		exit(0);
	} else if(strcmp("status", argv[1]) == 0) {
		char *data = openIgnoreFile();
		int s = 0;
		char **ign = parseIgnoreFile(data, &s);
		printStage(ign, s);
		exit(0);
	} else if(strcmp("checkout", argv[1]) == 0) {		
		if(strlen(argv[2]) > 8) {
			printf("requested branch id too long\n");
			exit(0);
		}
		checkoutBranch(argv[2]);		
		exit(0);
	} else if(argc >= 3) {
		// create repository specified directory
		if(strcmp("init", argv[1]) == 0) {	
			// read ignore file
			char *data = openIgnoreFile();
			int s = 0;
			char **ign = parseIgnoreFile(data, &s);
			/*
			if(strcmp(data, "") != 0) {
			} else {
				char **ign = '\0';
			}
			*/
				
			initRepository(argv[2], ign, s);
			exit(0);
		// commit a change 
		} else if(strcmp("commit", argv[1]) == 0) {
			if(strlen(argv[2]) > 255) {
				printf("commit message too long, exiting..\n");
				exit(0);
			}
			char *data = openIgnoreFile();
			int s = 0;
			char **ign = parseIgnoreFile(data, &s);
			createCommit(ign, s, argv[2]);
			exit(0); 
		} else if(strcmp("add", argv[1]) == 0) {
			addFile(argv[2]);
			exit(0);
		} else if(strcmp("rollback", argv[1]) == 0) {
			char *data = openIgnoreFile();
			int s = 0;
			char **ign = parseIgnoreFile(data, &s);
			rollbackToCommit(argv[2], ign, s);
			exit(0); 			
		} else if(strcmp("stage", argv[1]) == 0 && argc >= 3) {
			char *data = openIgnoreFile();
			int s = 0;
			char **ign = parseIgnoreFile(data, &s);
			stageFiles(argv, ign, s, argc);	
			exit(0); 
		}
	} else {
		printHelpMenu();
		exit(0);
	}
	
	
}
