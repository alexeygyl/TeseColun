#include "main.h"

/*
int sendInitRequest(){
	static int8_t	count = 1;
	if(count>=11)return -1;
	int iRC = 0;
	struct timeval	timeout;
	uint64_t	colun;
	timeout.tv_sec = 0;
	timeout.tv_usec = 500000;
	fd_set	fds;
	FD_ZERO(&fds);
	FD_SET(sock,&fds);
	if(args.channel == 1)txbuff[1] = 1;
	else if(args.channel == 2) txbuff[1] = 2;
	if(sendto(sock, txbuff, 2, 0, (struct sockaddr*)&clnt, sizeof(clnt))<0){
		perror("sendInitRequest sendto");
		count++;
		return sendInitRequest();
	}
	iRC = select(sock+1,&fds,NULL,NULL,&timeout);
	if(!iRC){
		printf("sendInitRequest timeout n%d\n",count);
		count++;
		return sendInitRequest();
	}else if(iRC<0){
		printf("sendInitRequest error n%d\n",count);
		count++;
		return sendInitRequest();
	}	
	if(recvfrom(sock,rxbuff,BUFFSIZE,0,NULL,NULL)<0){
		perror("sendInitRequest recvfrom");
		count++;
		return sendInitRequest();
	}
	if(rxbuff[0] != INIT_RESPONCE){	
		printf("sendInitRequest rxbuff not init responce n%d\n",count);
		count++;
		return sendInitRequest();
	}
	return 0;	
}
*/
