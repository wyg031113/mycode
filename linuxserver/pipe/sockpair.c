#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/socket.h>
#define CHECK(x) do{if((x)==-1){fprintf(stderr,"%s(%s-%d):%s\n",__FILE__, __FUNCTION__, __LINE__, strerror(errno));}}while(0)
void server(int *pipe)
{
	char buf[1024];
	int ret;
	CHECK(ret=read(pipe[0], buf, 1023));
	buf[ret] = 0;
	printf("client request:%s\n", buf);
	int fd = open(buf, O_RDONLY);
	if(fd<0)
	{
		snprintf(buf, 1024,"%s", strerror(errno));
		write(pipe[0], buf, strlen(buf));
	}
	else
	{
		while((ret=read(fd, buf, 1024))>0)
			write(pipe[0], buf, ret);
		close(fd);
		close(pipe[0]);
		close(pipe[1]);
		wait(NULL);
	}
}

void client(int *pipe)
{
	printf("Please input file name:\n");
	char buf[1024];
	fgets(buf, 1024, stdin);
	int len = strlen(buf);
	if(buf[len-1]=='\n')
		len--;
	write(pipe[1],buf, len);
	int ret;
	while((ret=read(pipe[1], buf, 1024))>0)
		write(STDOUT_FILENO, buf, ret);
	printf("\n");
	close(pipe[0]);
	close(pipe[1]);

}
int main()
{
	int pipe[2];
	CHECK(socketpair(AF_UNIX, SOCK_STREAM, 0, pipe));
	pid_t pid;
	CHECK(pid = fork());
	if(pid == 0)
	{
		client(pipe);
		exit(0);
	}
	server(pipe);
	
}
