#ifndef STORAGE_H
#define STORAGE_H

#include <time.h>
#include <stdbool.h>
#include "track.h"


void writeCommitFile(strobject *head, char *filename, int res);
// read and apply commit to base
void readCommitFile(char *filename, baseobject *bo);
// scan directory for files and get base ("1st") commit
void initRepository(char *dirname, char **ign, int i_size);
void copyFile(char *filename); 
void genCommitId(char *id);
void createCommit(char **ign, int isize, char *commit_msg);
int getBaseFile(char *filename, baseobject *b);
void getMostRecent(baseobject *bo, char *name, char *commit);
void addCommitMessage(char *msg, char *commit_id);
void addFile(char *name);
void rollbackToCommit(char *cid, char **ign, int i_size);
void stageFiles(char **opt, char **ign, int i_size, int opt_size);
bool isAlreadyStaged(char *opt);
void getSubdir(char *dir);

#endif
