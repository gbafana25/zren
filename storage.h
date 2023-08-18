#ifndef STORAGE_H
#define STORAGE_H

#include <time.h>
#include <stdbool.h>
#include "track.h"

#define REPO_DIR ".rep"
#define BASE_DIR ".rep/base/"
#define COMM_DIR ".rep/branches/main/"
#define BRANCH_DIR ".rep/branches/"
#define FILE_REG ".rep/FILES"
#define DIR_FILE ".rep/PROJECT_DIR"
#define IDENT_FILE ".rep/COMMIT_IDENTS"
#define TIMESTAMP "TIMESTAMP"
#define ID_LEN 25




void writeCommitFile(strobject *head, char *filename, int res);
// read and apply commit to base
void readCommitFile(char *filename, baseobject *bo);
// scan directory for files and get base ("1st") commit
void initRepository(char *dirname, char **ign, int i_size);
void copyFile(char *filename); 
void genCommitId(char *id);
void createCommit(char **ign, int isize, char *commit_msg);
int getBaseFile(char *filename, baseobject *b);
char *getMostRecent(baseobject *bo, char *name, char *commit, char *branch, bool is_revert);
void addCommitMessage(char *msg, char *commit_id);
void addFile(char *name);
void rollbackToCommit(char *cid, char **ign, int i_size);
void stageFiles(char **opt, char **ign, int i_size, int opt_size);
bool isAlreadyStaged(char *opt);
void getSubdir(char *dir);
void checkoutBranch(char *branch);
void revertFileContents(char **ign, int size, char *cid);

#endif
