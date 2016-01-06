#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#define CHECK(x) do{if((x)==-1){fprintf(stderr,"%s(%s-%d):%s\n",__FILE__, __FUNCTION__, __LINE__, strerror(errno));}}while(0)
void server()
{
	int rd = open("wyg1", O_RDONLY, 0);
	printf("open wyg1 success\n");
	char buf[1024];
	int ret;
	CHECK(ret=read(rd, buf, 1023));
	buf[ret] = 0;
	printf("client request:%s\n", buf);
	close(rd);
	int fd = open(buf, O_RDONLY);

	int wd = open("wyg2", O_WRONLY, 0);
	printf("open wyg2 success\n");
	if(fd<0)
	{
		snprintf(buf, 1024,"%s", strerror(errno));
		write(wd, buf, strlen(buf));
	}
	else
	{
		while((ret=read(fd, buf, 1024))>0)
			write(wd, buf, ret);
		close(fd);
		close(wd);
	}
}

int main()
{
	mkfifo("wyg1", 0660);
	mkfifo("wyg2", 0660);
	
	server();
	
}
