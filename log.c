#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

#include "log.h"

char *slugMessage(char *msg) {
	for(int i = 0; i < strlen(msg); i++) {
		if(msg[i] == ' ') {
			msg[i] = '-';
		}
	}
	return msg;
}

void recordCommit(char *cid, char *msg) {
	FILE *l = fopen(LOGFILE, "a+");
	time_t t = 0;
	t = time(NULL);
	char *slug = slugMessage(msg);
	//printf("%s\n", slug);
	fprintf(l,"%s %d %s\n", cid, t, slug);
	fclose(l);
	
}

void logBase() {
	FILE *l = fopen(LOGFILE, "a+");
	time_t t = 0;
	t = time(NULL);
	fprintf(l,"base %d base\n", t);
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
	FILE *l = fopen(LOGFILE, "r");
	char id[26];
	time_t timestamp = 0;
	char msg[256];
	struct tm *timestr; 
	while(fscanf(l, "%s %d %s\n", id, &timestamp, msg) != -1) {
		if(count == 2) {
			// convert time to human-readable format
			timestr = localtime(&timestamp);	
			printf("\033[33m%s\033[0m \033[91m%s\033[0m \n\t\033[;1m%s\033[0m\n", id, asctime(timestr), msg);
		} else if(showTime(opt, count) == true) {
			// convert time to human-readable format
			timestr = localtime(&timestamp);	
			printf("%s", asctime(timestr));
		} else if(showMessage(opt, count) == true) {
			printf(" %s", msg);
		} else if(showId(opt, count) == true) {
			printf(" %s", id);
		}
		printf("\n");
	}
}

