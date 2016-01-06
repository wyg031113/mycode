#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<string.h>
#include<iostream>
#include<netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<signal.h>
using namespace std;

#define BUFF_SIZE 4096*4

int main(int argc, char *argv[])
{
	printf("This is client...\n");
	signal(SIGPIPE, SIG_IGN);
	if(argc < 4)
	{
		printf("Usage: ./testClient ser_ip port sendbufsize\n");
		return -1;
	}
	int sd = socket(AF_INET, SOCK_STREAM, 0);
	if(sd < 0)
	{
		cerr<<"get sd failed\n"<<endl;
		return -1;
	}

	struct sockaddr_in ser_addr;
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_port = htons(atoi(argv[2]));
	ser_addr.sin_addr.s_addr = inet_addr(argv[1]);
	bzero(ser_addr.sin_zero, 8);
	int sendbuf = atoi(argv[3]);
	int getsendbuf;
	socklen_t socklen;

	setsockopt(sd, SOL_SOCKET, SO_SNDBUF, &sendbuf, sizeof(sendbuf));
	getsockopt(sd, SOL_SOCKET, SO_SNDBUF, &getsendbuf, &socklen);
	printf("After set socket snd buffser, this size is: %d\n", getsendbuf);

	if(connect(sd, (struct sockaddr*)&ser_addr, sizeof(struct sockaddr))<0)
	{
		printf("Connect failed!\n");
		close(sd);
		return -1;
	}
	printf("连接服务器成功\n");
	char buf[BUFF_SIZE];
	int nsend  = send(sd, buf, BUFF_SIZE, 0);

	close(sd);
	return 0;
}
