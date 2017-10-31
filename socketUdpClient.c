#include "main.h"

void createClientUdpSocket(int32_t *__sock){
	struct sockaddr_in	__serv; 
	int			len;

	if((*__sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket");
		exit(0);
	}
//	if(setsockopt(*__sock, SOL_SOCKET, SO_BROADCAST, (void *) &broadcastPermission, sizeof(broadcastPermission)) < 0)perror("setsockopt() failed");
	__serv.sin_family=AF_INET;
	__serv.sin_port=htons(9999);
	__serv.sin_addr.s_addr=htonl(INADDR_ANY);
	if(bind(*__sock, (struct sockaddr*)&__serv, sizeof(__serv)) < 0) {
		perror("bind");
		close(*__sock);
		exit(0);
	}
}

