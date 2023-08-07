#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "client.h"


void sendCommitInfo(localRepoInfo *info) {
	char packfile_name[strlen(PACKFILE_PREFIX)+strlen(info->id)];
	memset(&packfile_name, 0, sizeof(packfile_name));	
	strcat(packfile_name, PACKFILE_PREFIX);
	strcat(packfile_name, info->id);
	packfile_name[strlen(packfile_name)] = '\0';
	
	FILE* pfile = fopen(packfile_name, "rb");
	void *pack_data = malloc(sizeof(info->size));
	//char pack_data[info->size];
	//void pack_data[info->size];
	fread(&pack_data, info->size, 1, pfile);
	
	printf("%d\n", info->size);
	fclose(pfile);
	
	/*
	int sock;
	struct sockaddr_in cli;

	sock = socket(AF_INET, SOCK_STREAM, 0);

	cli.sin_family = AF_INET;
	cli.sin_port = htons(8010);

	inet_pton(AF_INET, "127.0.0.1", &cli.sin_addr);
	connect(sock, (struct sockaddr *)&cli, sizeof(cli));	
	*/

}

localRepoInfo getCurrentCommit() {
	localRepoInfo info;
	FILE *head = fopen(".rep/HEAD", "r");
	char current_id[25];
	char id[25];
	char branch[15];
	char action[25];
	char msg[256];
	time_t timestamp;
	// get current head
	fscanf(head, "%s %s", current_id,  branch);
	fclose(head);
	//printf("%s\n", id);
	strcpy(info.id, current_id);
	strcpy(info.branch, branch);

	// get rest of commit info from log
	char full_path[strlen(LOG_DIR)+strlen(branch)];		
	memset(&full_path, 0, sizeof(full_path)); 
	strcat(full_path, LOG_DIR);
	strcat(full_path, branch);
	full_path[strlen(full_path)] = '\0';
	//printf("%s\n", full_path);

	FILE *log = fopen(full_path, "r");
	while(fscanf(log, "%s %d %s %s", id, &timestamp, action, msg) != -1) {
		//printf("%s\n", id);
		if(strcmp(id, current_id) == 0) {
			info.timestamp = timestamp;
			strcpy(info.action, action);
			strcpy(info.msg, msg);
			break;
		}
	}
	//printf("%s %d %s\n", id, timestamp, action);	
	fclose(log);

	return info;
}

void packDir(localRepoInfo *info) {
	// change into commit directory
	char *base_cmd = "tar cvf packfile-";
	char full_path[strlen(COMMIT_DIR)+strlen(info->branch)+strlen(info->id)+1];
	// archive name: packfile-[commit id]	
	// final command:  tar cvf packfile-[id] .
	char full_command[strlen(base_cmd)+strlen(info->id)+2];
	memset(&full_command, 0, sizeof(full_command));

	strcat(full_command, base_cmd);
	strcat(full_command, info->id);
	strcat(full_command, " .");
	full_command[strlen(full_command)] = '\0';

	memset(&full_path, 0, sizeof(full_path));
	strcat(full_path, COMMIT_DIR);
	strcat(full_path, info->branch);
	strcat(full_path, "/");
	strcat(full_path, info->id);
	full_path[strlen(full_path)] = '\0';

	//printf("%s %s\n", full_path, full_command);
	chdir(full_path);
	system(full_command);
	// get packfile size and add to struct
	char packfile_name[strlen(PACKFILE_PREFIX)+strlen(info->id)];
	strcat(packfile_name, PACKFILE_PREFIX);
	strcat(packfile_name, info->id);
	//printf("%s\n", packfile_name);
	struct stat st;
	stat(packfile_name, &st);
	//size_t s = st.st_size;
	/*
	FILE* pfile = fopen(packfile_name, "r");
	fseek(pfile, 0L, SEEK_END);
	size_t s = ftell(pfile);
	rewind(pfile);
	fclose(pfile);
	*/
	info->size = st.st_size;
	//printf("%d\n", info->size);

}
