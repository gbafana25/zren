#ifndef SERVER_H
#define SERVER_H

typedef struct {
	char id[25];
	int time;
	char action[15];
	char msg[25];
	int size;

} repoInfo;

void startTcpServer();

#endif
