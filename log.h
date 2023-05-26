#ifndef LOG_H
#define LOG_H

#define LOGFILE ".rep/LOG"

void logAction(char *cid, char *msg, char *action);
void slugMesage(char *msg);
void deslugMessage(char *msg);
void logBase();
void outputLogfile(char **opt, int count);

#endif
