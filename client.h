#ifndef CLIENT_H 
#define CLIENT_H 


#define LOG_DIR ".rep/logs/"
#define COMMIT_DIR ".rep/branches/"
#define BASE_DIR ".rep/base/"

#define PACKFILE_PREFIX "packfile-"
#define REMOTE_FILE ".rep/REMOTE"

typedef struct {
	char id[25];
	char branch[25];
	time_t timestamp;
	char action[15];
	char msg[255];
	size_t size;

} localRepoInfo;

void sendCommitInfo(localRepoInfo *info, char *url);
// read .rep/HEAD
localRepoInfo getCurrentCommit();
void packDir(localRepoInfo *info);
void packBase(localRepoInfo *info);
void setRemote(char *url);
void getRemote(char *url);

#endif
