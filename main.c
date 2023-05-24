#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "track.h"
#include "storage.h" 
#include "ignore.h"
#include "menu.h"
#include "log.h"

int main(int argc, char **argv) { 	

	if(argc == 1) {
		printHelpMenu();
		exit(0);
	} else if(strcmp("log", argv[1]) == 0) {		
		outputLogfile(argv, argc);
		exit(0);
	} else if(strcmp("status", argv[1]) == 0) {
		char *data = openIgnoreFile();
		int s = 0;
		char **ign = parseIgnoreFile(data, &s);
		printStage(ign, s);
		exit(0);
	} else if(argc >= 3) {
		// create repository specified directory
		if(strcmp("init", argv[1]) == 0) {	
			// read ignore file
			char *data = openIgnoreFile();
			int s = 0;
			char **ign = parseIgnoreFile(data, &s);
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
		} else if(strcmp("revert", argv[1]) == 0) {
			revertToCommit(argv[2]);
			exit(0);
		} else if(strcmp("rollback", argv[1]) == 0) {
			rollbackToCommit(argv[2]);
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
