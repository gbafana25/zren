#include <stdio.h>

#include "menu.h"

void printHelpMenu() {
	printf("zren help menu\n\n");
	printf("Available commands:\n\n");
	printf("init [directory] - initialize a new repository in the specified directory\n");
	printf("commit [message] - save current changes\n");
	printf("add [filename] - add file to be tracked\n");
	printf("rollback [commit_id] - set head to given commit");
	printf("revert [commit_id] - hard reset back to given commit, deletes data from commits made afterwards\n");
	printf("log [msg|id|time] - show repository log, with optional flags to filter options\n");
	printf("stage [filename] or --all - move file(s) to staging area to be committed\n\n");

}
