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
			char *data = openIgnoreFile();
			int s = 0;
			char **ign = parseIgnoreFile(data, &s);
			initRepository(argv[2], ign, s);
			exit(0);
		} else if(strcmp("commit", argv[1]) == 0) {
			char *data = openIgnoreFile();
			int s = 0;
			char **ign = parseIgnoreFile(data, &s);
			createCommit(ign, s, argv[2]);
			exit(0); 
		} else if(strcmp("add", argv[1]) == 0) {
			addFile(argv[2]);
			exit(0);
		} else {
			printHelpMenu();
			exit(0);

		}
	} else {
		printHelpMenu();
		exit(0);
	}
	
	
}
