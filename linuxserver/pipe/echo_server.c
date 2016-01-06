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

int main(int argc, char *argv[])
{
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
	int fd[2];
	CHECK(pipe(fd));
	CHECK(splice(connfd, NULL, fd[1], NULL, 65535, SPLICE_F_MORE|SPLICE_F_MOVE));
	CHECK(splice(fd[0], NULL, connfd, NULL, 65535, SPLICE_F_MORE|SPLICE_F_MOVE));

	close(connfd);
	close(fd[0]);
	close(fd[1]);
	return 0;
}
