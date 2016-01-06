#include<string.h>
#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#define PORT 12345
#define IP "127.0.0.1"
#define MAXQUE 6
#define BUFF_SIZE 1024
#include <stdlib.h>

char buf[BUFF_SIZE];
int main(int argc, char *argv[])
{
	if(argc<2)
	{
		printf("error,need a param.\nUSAGE:./tcp_server SendBufSize\n");
		exit(-1);
	}
	int ser_sd;
	struct sockaddr_in addr;
 	ser_sd = socket(AF_INET, SOCK_STREAM, 0);
	if(ser_sd == -1)
	{
		printf("Socket failed!\n");
		return -1;
	}
	
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = INADDR_ANY;
//	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	bzero(&addr.sin_zero, 8);

	
	int opt = 1;
	int len = sizeof(opt);
	setsockopt(ser_sd, SOL_SOCKET, SO_REUSEADDR, &opt, len);
	if(bind(ser_sd, (struct sockaddr*)&addr, sizeof(struct sockaddr)) == -1)
	{
		printf("Bind failed!\n");
		return -1;
	}
	//set send buffer size
	int rcvbuf = atoi(argv[1]);
	int getrcvbuf;
	socklen_t socklen;

	setsockopt(ser_sd, SOL_SOCKET, SO_RCVBUF, &rcvbuf, sizeof(rcvbuf));
	getsockopt(ser_sd, SOL_SOCKET, SO_RCVBUF, &getrcvbuf, &socklen);
	printf("After set socket snd buffser, this size is: %d\n", getrcvbuf);
	listen(ser_sd, MAXQUE);
	printf("Server started, listened on [0.0.0.0:12345]\n");
	struct sockaddr_in cli_addr;
	int clilen=sizeof(struct sockaddr_in);
	int fd = accept(ser_sd, (struct sockaddr*)&cli_addr, &clilen);
	while(recv(fd, buf, BUFF_SIZE-1, 0)>0)
		;
	
	close(fd);
	close(ser_sd);
	return 0;
}
