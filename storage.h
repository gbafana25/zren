#ifndef STORAGE_H
#define STORAGE_H

#include "track.h"

void writeCommitFile(strobject *head, char *filename);
// read and apply commit to base
void readCommitFile(char *filename, baseobject *bo);
// scan directory for files and get base ("1st") commit
void initRepository();
void copyFile(char *filename); 
void genCommitId(char *id);
void createCommit(char *dirname);
void getBaseFile(char *filename, baseobject *b);

#endif
