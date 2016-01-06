#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#define SERVER_FIFO "/tmp/ser_fifo"
#define CHECK(x) do{ if((x)==-1){fprintf(stderr, "%s:(%s-%d)", __FILE__, __FUNCTION__, __LINE__); fprintf(stderr, "%s", strerror(errno));}}while(0)
#define MAXLEN 1024

int main()
{
	int rd, wr;
	char *ptr, buff[MAXLEN], fifoname[MAXLEN], cmd[MAXLEN];
	int len;
	pid_t pid = getpid();
	snprintf(fifoname, MAXLEN, "/tmp/fifo.%d", pid);
	mkfifo(fifoname, 0666);
	
	CHECK(wr = open(SERVER_FIFO, O_WRONLY));
	printf("Please input filename:\n");
	fgets(buff, MAXLEN, stdin);
	int fnlen = strlen(buff);
	if(buff[fnlen-1] =='\n')
		buff[--fnlen] = '\0';
	snprintf(cmd, MAXLEN, "%d %s", pid, buff);
	write(wr, cmd, strlen(cmd));

	CHECK(rd = open(fifoname, O_RDONLY));
	while( (len = read(rd, buff, MAXLEN-1)) > 0)
		write(STDOUT_FILENO, buff, len);
	close(rd);
	close(wr);
	unlink(fifoname);
	return 0;
}
