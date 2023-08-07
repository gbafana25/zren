#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#include "server.h"

void startTcpServer() {
	int opt = 1;
	struct sockaddr_in serv, client;

	int soc = socket(AF_INET, SOCK_STREAM, 0);
	setsockopt(soc, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	
	memset(&serv, 0, sizeof(serv));
	serv.sin_family = AF_INET;
	serv.sin_port = htons(8010);
	serv.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(soc, (struct sockaddr *)&serv, sizeof(serv));
	listen(soc, 5);

	int cli_soc = accept(soc, (struct sockaddr *)&client, sizeof(client));


}
