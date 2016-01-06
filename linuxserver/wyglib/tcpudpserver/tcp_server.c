#include <stdio.h>
#include <unistd.h>
#define __USE_GNU
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#define CHECK(x) if((x)==-1){printf("%s(%s:%d)error:%s\n", __FILE__, __FUNCTION__, __LINE__, strerror(errno));}
#include <string.h>
#include <stdlib.h>
#include <errno.h>
int tcp_server(char *ip, unsigned short port)
{
	struct sockaddr_in addr;
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	if(ip == NULL)
		addr.sin_addr.s_addr = INADDR_ANY; 
	else
		addr.sin_addr.s_addr = inet_addr(ip); 
	addr.sin_port = htons(port);
	int sock;
	CHECK(sock=socket(PF_INET, SOCK_STREAM, 0));
	int opt = 2;
	int len = sizeof(opt);
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, len);
	CHECK(bind(sock, (struct sockaddr*)&addr, sizeof(addr)));

	CHECK(listen(sock, 5));
	return sock;
}
