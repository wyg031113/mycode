#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#define CHECK(x) do{if((x)==-1){fprintf(stderr,"%s(%s-%d):%s\n",__FILE__, __FUNCTION__, __LINE__, strerror(errno));}}while(0)

int main()
{
	int pipe1[2], pipe2[2];
	CHECK(pipe(pipe1));
	CHECK(pipe(pipe2));
	pid_t pid;
	CHECK(pid = fork());
	if(pid == 0)
	{
		close(pipe1[0]);	
		close(pipe2[1]);	
		printf("Please input file name:\n");
		char buf[1024];
		fgets(buf, 1024, stdin);
		int len = strlen(buf);
		if(buf[len-1]=='\n')
			len--;
		write(pipe1[1],buf, len);
		int ret;
		while((ret=read(pipe2[0], buf, 1024))>0)
			write(STDOUT_FILENO, buf, ret);
		printf("\n");
		close(pipe1[1]);
		close(pipe2[0]);
		exit(0);
	}
	close(pipe1[1]);	
	close(pipe2[0]);	
	char buf[1024];
	int ret;
	CHECK(ret=read(pipe1[0], buf, 1023));
	ret--;
	buf[ret] = 0;
	printf("client request:%s\n", buf);
	int fd = open(buf, O_RDONLY);
	if(fd<0)
	{
		snprintf(buf, 1024,"%s", strerror(errno));
		write(pipe2[1], buf, strlen(buf));
	}
	else
	{
		while((ret=read(fd, buf, 1024))>0)
			write(pipe2[1], buf, ret);
		close(fd);
		close(pipe1[0]);
		close(pipe2[1]);
		wait(NULL);
	}
}
