#include<stdio.h>
#include<iostream>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
using namespace std;

#define PORT 4567
int main()
{
	cout<<"Welcom to upd Server..."<<endl;
	int sd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sd < 0)
	{
		cerr<<"get socket failed!"<<endl;
		return -1;
	}
	
	struct sockaddr_in ser_addr, cli_addr;
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_port = htons(PORT);
	ser_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&ser_addr.sin_zero, 8);

	char buf[256];
	socklen_t slen = sizeof(struct sockaddr);	
	if(bind(sd, (struct sockaddr*)&ser_addr, sizeof(struct sockaddr)) == -1)
	{
		cerr<<"bind failed!!"<<endl;
		close(sd);
		return -1;
	}
	int nrecv = recvfrom(sd, buf, 256, 0, (struct sockaddr*)&cli_addr, &slen);
	buf[nrecv] = '\0';
	printf("Recv %d bytes: %s\n", nrecv, buf);
	return 0;
}
