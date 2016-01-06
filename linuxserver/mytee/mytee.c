#include <stdio.h>
#include <unistd.h>
#define __USE_GNU
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <errno.h>
#define CHECK(x) if((x)==-1){printf("%s(%s:%d)error:%s\n", __FILE__, __FUNCTION__, __LINE__, strerror(errno));}

int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		printf("Usage:%s <file>\n", argv[0]);
		return -1;
	}

	int filefd;
	CHECK(filefd = open(argv[1], O_CREAT|O_WRONLY|O_TRUNC, 0666));
	int pipefd_stdout[2];
	int pipefd_file[2];
	CHECK(pipe(pipefd_stdout));
	CHECK(pipe(pipefd_file));
	CHECK(splice(STDIN_FILENO, NULL, pipefd_stdout[1], NULL, 32768, SPLICE_F_MOVE|SPLICE_F_MORE));
	CHECK(tee(pipefd_stdout[0], pipefd_file[1], 32768, SPLICE_F_NONBLOCK));
	CHECK(splice(pipefd_stdout[0], NULL, STDOUT_FILENO, NULL, 32768, SPLICE_F_MOVE|SPLICE_F_MORE));
	CHECK(splice(pipefd_file[0], NULL, filefd,  NULL, 32768, SPLICE_F_MOVE|SPLICE_F_MORE));
	close(filefd);
	close(pipefd_stdout[0]);
	close(pipefd_stdout[1]);
	close(pipefd_file[0]);
	close(pipefd_file[1]);
}
