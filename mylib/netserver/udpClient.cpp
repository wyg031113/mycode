#include<stdio.h>
#include<iostream>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
using namespace std;
#define SER_IP "192.168.0.15"
#define PORT 12345
int main()
{
	cout<<"Welcom to upd Client..."<<endl;
	int sd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sd < 0)
	{
		cerr<<"get socket failed!"<<endl;
		return -1;
	}
	
	struct sockaddr_in ser_addr;
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_port = htons(PORT);
	ser_addr.sin_addr.s_addr = inet_addr(SER_IP);
	bzero(&ser_addr.sin_zero, 8);

	char buf[256] = "I am a udp client";
	socklen_t slen = sizeof(struct sockaddr);	
	int nsend = sendto(sd, buf, strlen(buf), 0, (struct sockaddr*)&ser_addr, slen);
	printf("Send %d bytes\n", nsend);
	int nrecv = recvfrom(sd, buf, 256, 0, (struct sockaddr*)&ser_addr, &slen);
	buf[nrecv]='\0';
	printf("nRecv %d bytes: %s\n", nrecv, buf);
	return 0;
}
