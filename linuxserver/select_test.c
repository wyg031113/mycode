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
#include <sys/select.h>
#define MAXFDS 4
int rfds[MAXFDS], wfds[MAXFDS], efds[MAXFDS];
int nr, nw, ne;
int maxfd;
fd_set readfds, writefds, exceptfds;
void reset_fds()
{
	int i;
	FD_ZERO(&readfds);
	for(i = 0; i < nr; i++)
		FD_SET(rfds[i], &readfds);

	
	FD_ZERO(&writefds);
	for(i = 0; i < nw; i++)
		FD_SET(wfds[i], &writefds);

	
	FD_ZERO(&exceptfds);
	for(i = 0; i < ne; i++)
		FD_SET(efds[i], &exceptfds);
}

void remove_fd(int fd, int *fds, int *n)
{
	if(maxfd == fd)
		maxfd--;
	int i;
	for(i = 0; i < *n; i++)
		if(fds[i] == fd)
		{
			fds[i] = fds[--*n];
			break;
		}
}

void remove_fd2(int pos, int *fds, int *n)
{
	fds[pos] = fds[--(*n)];
}

int add_fd(int fd, int *fds, int *n)
{
	if(*n >= MAXFDS)
		return -1;
	if(fd > maxfd)
		maxfd = fd;
	fds[(*n)++] = fd;
	return 0;
}
#define MAX_CLIENT_DATA
struct ClientData
{
	char *buf;
	int offset;
	int len;
	struct ClientData *next;
};
struct ClientDataHead
{
	struct ClientData *next;
	struct ClientData *tail;
};
struct ClientData client_data[65536];
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

	add_fd(sock, rfds, &nr);	
	while(1)
	{
		reset_fds();
		struct timeval tv;
		tv.tv_sec = 3;
		tv.tv_usec = 123;
		int ret = select(maxfd+1, &readfds, &writefds, &exceptfds, &tv);
		if(ret < 0)
		{
			printf("select ret:%d-->%s\n", ret, strerror(errno));
			break;
		}
		else if(ret == 0)
			{
				printf("Timed out!\n");
				continue;
			}
		else
		{
			int i;
			for(i = 0; i < nr; i++)
				if(FD_ISSET(rfds[i], &readfds))
				{
					if(rfds[i] == sock)
					{
						int connfd;
						CHECK(connfd=accept(rfds[i], NULL, 0));
						if(add_fd(connfd, rfds, &nr)==-1)
						{
							close(connfd);
							printf("reach maxfds!!!\n");
						}
						printf("Accept a client.\n");
						continue;
					}
					char rbuf[512];
					int nbytes = recv(rfds[i], rbuf, 512, 0);
					if(nbytes <= 0)
					{
						remove_fd2(i, rfds, &nr);
						i--;
					}
					else
					{
						if(add_fd(connfd, wfds, &nr)==-1)
						{
							close(connfd);
							printf("reach maxfds!!!\n");
						}
						else
						{
							struct ClientData *cd = (struct ClientData*)malloc(sizeof(struct ClientData));
							cd->buf = (char*)malloc(nbytes);
							if(cd==NULL || cd->buf==NULL)
							{
								printf("memory low...\n");
								exit(-1);
							}
							
							memcpy(cd->buf, rbuf, nbytes);
							cd->offset = 0;
							cd->len = nbytes;
							cd->next = NULL;
							if(client_data[connfd] == NULL)
								client_data[connfd] = cd;
							
						}
					}
				}
		}

	}
	close(sock);
	return 0;
}
