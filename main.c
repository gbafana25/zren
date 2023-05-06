#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "track.h"
#include "storage.h" 

int main(int argc, char **argv) { 

	baseobject base;
	baseobject mod;
	//strobject test_string;
	
	if(argc == 3) {
		if(strcmp("init", argv[1]) == 0) {
			// TODO: initialize repository
			initRepository(argv[2]);
			exit(0);
		} else if(strcmp("commit", argv[1]) == 0) {
			createCommit(argv[2]);
			exit(0);
		}
	}

	/*
	initStrObject(head, 'H', NULL, CHANGE); 
	findDiff(base.data, mod.data, head);
	writeCommitFile(head, "commit");		

	readCommitFile("commit", &base);
	*/
	
}
