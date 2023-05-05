#ifndef STORAGE_H
#define STORAGE_H

#include "track.h"

void writeCommit(strobject *head, char *filename);
// read and apply commit to base
void readCommit(char *filename, baseobject *bo);

#endif
