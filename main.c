#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "track.h"
#include "storage.h" 
#include "ignore.h"

int main(int argc, char **argv) { 

	baseobject base;
	baseobject mod;
	//strobject test_string;
	
	if(argc == 3) {
		if(strcmp("init", argv[1]) == 0) {
			initRepository(argv[2]);
			exit(0);
		}
	} else if(argc == 2) {
		if(strcmp("commit", argv[1]) == 0) {
			char *data = openIgnoreFile();
			int s = 0;
			char **ign = parseIgnoreFile(data, &s);
			createCommit(ign, s);
			exit(0);
		} else if(strcmp("ig", argv[1]) == 0) {
			char *data = openIgnoreFile();
			int s = 0;
			char **ign = parseIgnoreFile(data, &s);	
			/*
			for(int i = 0; i < s; i++) {
				printf("%s\n", ign[i]);
			}
			*/
			
			exit(0);
		}
	} else {
		printf("Wrong command - use init [dir] or commit\n");
		exit(0);
	}

	/*
	initStrObject(head, 'H', NULL, CHANGE); 
	findDiff(base.data, mod.data, head);
	writeCommitFile(head, "commit");		

	readCommitFile("commit", &base);
	*/
	
}
