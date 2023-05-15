#include <stdio.h>
#include <time.h>
#include <string.h>

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
