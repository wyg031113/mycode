#include<string.h>
#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#define PORT 12345
#define IP "127.0.0.1"
#define MAXQUE 6
int main()
{
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
	listen(ser_sd, MAXQUE);
	struct sockaddr_in cli_addr;
	int clilen=sizeof(struct sockaddr_in);
	int fd = accept(ser_sd, (struct sockaddr*)&cli_addr, &clilen);
	printf("Recv conn from %s\n", inet_ntoa(cli_addr.sin_addr));
	char wel[]="Hello, Welcom to my computer!\r\n";
	write(fd,wel, strlen(wel)); 	
	close(fd);
	close(ser_sd);
	return 0;
}
