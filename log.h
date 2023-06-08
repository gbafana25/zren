#ifndef LOG_H
#define LOG_H

#define MAIN_LOG ".rep/logs/MAIN"
#define LOG_DIR ".rep/logs/"

void logAction(char *cid, char *msg, char *action, char *logfile);
void slugMesage(char *msg);
void deslugMessage(char *msg);
void logBase();
void outputLogfile(char **opt, int count);
char *getCurrentBranch();

#endif
