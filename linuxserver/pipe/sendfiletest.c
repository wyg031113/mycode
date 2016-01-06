#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <arpa/inet.h>
#define CHECK(x) if((x)==-1){printf("%s(%s:%d)error:%s\n", __FILE__, __FUNCTION__, __LINE__, strerror(errno));}
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char *argv[])
{
	if(argc < 2)
	{
		printf("Usage:./sendfiletest filename\n");
		exit(-1);
	}

	int filefd, f2;
	CHECK(filefd = open(argv[1], O_RDONLY));
	//CHECK(f2 = open("app.txt", O_RDWR));
	struct stat stat_buf;
	fstat(filefd, &stat_buf);
	
	struct sockaddr_in addr;
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY; 
	addr.sin_port = htons(12345);
	int sock;
	CHECK(sock=socket(PF_INET, SOCK_STREAM, 0));
	int opt = 2;
	int len = sizeof(opt);
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, len);
	CHECK(bind(sock, (struct sockaddr*)&addr, sizeof(addr)));

	CHECK(listen(sock, 5));
	int connfd;
	CHECK(connfd=accept(sock, NULL, 0));
	CHECK(sendfile(connfd, filefd, NULL, stat_buf.st_size));
	close(connfd);
	return 0;
}
