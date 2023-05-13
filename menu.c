#include <stdio.h>

#include "menu.h"

void printHelpMenu() {
	printf("myvcs help menu\n\n");
	printf("Available commands:\n\n");
	printf("init [directory] - initialize a new repository in the specified directory\n");
	printf("commit [message] - save current changes\n\n");

}
