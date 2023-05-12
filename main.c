#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "track.h"
#include "storage.h" 
#include "ignore.h"
#include "menu.h"

int main(int argc, char **argv) { 

	baseobject base;
	baseobject mod;
	
	if(argc == 3) {
		if(strcmp("init", argv[1]) == 0) {
			initRepository(argv[2]);
			// make a first commit after starting repo
			exit(0);
		}
	} else if(argc == 2) {
		if(strcmp("commit", argv[1]) == 0) {
			char *data = openIgnoreFile();
			int s = 0;
			char **ign = parseIgnoreFile(data, &s);
			createCommit(ign, s);
			exit(0); 
		} else {
			//printf("Wrong command - use init [dir] or commit\n");
			printHelpMenu();
			exit(0);

		}
	} else {
		//printf("Wrong command - use init [dir] or commit\n");
		printHelpMenu();
		exit(0);
	}
	
	
}
