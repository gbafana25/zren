#ifndef LOG_H
#define LOG_H

#define LOGFILE ".rep/LOG"

void recordCommit(char *cid, char *msg);
char *slugMesage(char *msg);
void logBase();

#endif
