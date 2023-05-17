#ifndef STORAGE_H
#define STORAGE_H

#include <time.h>
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
void getMostRecent(baseobject *bo);
void addCommitMessage(char *msg, char *commit_id);
void addFile(char *name);
// 'hard resets' back to commit (deletes all following commits)
void revertToCommit(char *cid);
// just sets head to commit id
void rollbackToCommit(char *cid);
void stageFiles(char **opt, char **ign, int i_size, int opt_size);

#endif
