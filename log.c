#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "log.h"

void slugMessage(char *msg) {
	for(int i = 0; i < strlen(msg); i++) {
		if(msg[i] == ' ') {
			msg[i] = '-';
		}
	}
}

void deslugMessage(char *msg) {
	for(int i = 0; i < strlen(msg); i++) {
		if(msg[i] == '-') {
			msg[i] = ' ';
		}
	}
}

char *getCurrentBranch() {
	char *br = (char*)malloc(sizeof(char)*(51)); 
	char commit[50];
	FILE *head = fopen(".rep/HEAD", "r");
	fscanf(head, "%s %s", commit, br);	
	fclose(head);
	return br;
}

void logAction(char *cid, char *msg, char *action, char *logfile) {
	FILE *l = fopen(logfile, "a+");
	time_t t = 0;
	t = time(NULL);
	slugMessage(msg);
	//printf("%s\n", slug);
	fprintf(l,"%s %d %s %s\n", cid, t, action, msg);
	fclose(l);
	
}

void logBase() {
	FILE *l = fopen(MAIN_LOG, "a+");
	time_t t = 0;
	t = time(NULL);
	fprintf(l,"base %d commit base\n", t);
	fclose(l);
}

bool showMessage(char **opt, int count) {
	for(int i = 0; i < count; i++) {
		if(strcmp(opt[i], "msg") == 0) {
			return true;
		}
	}
	return false;
}

bool showId(char **opt, int count) {
	for(int i = 0; i < count; i++) {
		if(strcmp(opt[i], "id") == 0) {
			return true;
		}
	}
	return false;
}

bool showTime(char **opt, int count) {
	for(int i = 0; i < count; i++) {
		if(strcmp(opt[i], "time") == 0) {
			return true;
		}
	}
	return false;
}

// log command will include options to filter data
void outputLogfile(char **opt, int count) {
	char *branch = getCurrentBranch();
	char logpath[strlen(LOG_DIR)+strlen(branch)+1];
	memset(&logpath, 0, sizeof(logpath));
	strcat(logpath, LOG_DIR);	
	strcat(logpath, branch);
	FILE *l = fopen(logpath, "r");
	char id[26];
	char action[45];
	time_t timestamp = 0;
	char msg[256];
	struct tm *timestr; 
	while(fscanf(l, "%s %d %s %s\n", id, &timestamp, action, msg) != -1) {
		if(count == 2) {
			// convert time to human-readable format
			timestr = localtime(&timestamp);
			deslugMessage(msg);
			printf("%s \033[33m%s\033[0m \033[91m%s\033[0m \n\t\033[;1m%s\033[0m\n", action, id, asctime(timestr), msg);
		} else if(showTime(opt, count) == true) {
			// convert time to human-readable format
			timestr = localtime(&timestamp);	
			printf("%s", asctime(timestr));
		} else if(showMessage(opt, count) == true) {
			deslugMessage(msg);
			printf(" %s", msg);
		} else if(showId(opt, count) == true) {
			printf(" %s", id);
		}
		printf("\n");
	}
	fclose(l);
}

