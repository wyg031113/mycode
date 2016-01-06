#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#define CHECK(x) if((x)==-1){printf("error:%s\n", strerror(errno));}

int main()
{
	int fd[2];
	CHECK(socketpair(AF_UNIX, SOCK_STREAM, 0, fd));
	char buf[] = "Hello, pipe.";
	char str[512];
	int ret;
	CHECK(ret=write(fd[0], buf, strlen(buf)));
	CHECK(ret=read(fd[1], str, 512));
	str[ret] = '\0';
	printf("Read:%s\n", str);

	int fd2 = dup(fd[1]);
	CHECK(ret=write(fd2, buf, strlen(buf)));
	
	int filefd = open("app.txt", O_RDWR | O_CREAT, 0644);	
	int stdoutbak = dup(1);
	CHECK(dup2(filefd, 1));
	printf("do you see!!!\n");
	CHECK(ret=read(fd[0], str, 512));
	str[ret] = '\0';

	CHECK(dup2(stdoutbak, 1));
	printf("Read:%s\n", str);

	char data1[100] = "hello world\n";
	char data2[100] = "hello everyday\n";
	char data3[100] = "this is end!\n";
	struct iovec iv[3];
	iv[0].iov_base = data1;
	iv[0].iov_len = strlen(data1);
	iv[1].iov_base = data2;
	iv[1].iov_len = strlen(data2);
	iv[2].iov_base = data3;
	iv[2].iov_len = strlen(data3);
	writev(1, iv, 3);

	iv[0].iov_len = iv[1].iov_len = iv[2].iov_len = 10;
	
	CHECK(readv(0, iv, 3));
	writev(1, iv, 3);
	close(fd[0]);
	close(fd[1]);
	close(filefd);
	return 0;
}
