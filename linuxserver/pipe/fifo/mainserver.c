#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

#define SERVER_FIFO "/tmp/ser_fifo"
#define CHECK(x) do{ if((x)==-1){fprintf(stderr, "%s:(%s-%d)", __FILE__, __FUNCTION__, __LINE__); fprintf(stderr, "%s", strerror(errno));}}while(0)
#define MAXLEN 1024

int main()
{
	int rd, wr;
	char *ptr, buff[MAXLEN], fifoname[MAXLEN];
	int len;
	pid_t pid;
	mkfifo(SERVER_FIFO, 0622);
	
	CHECK(rd = open(SERVER_FIFO, O_RDONLY));
	CHECK(wr = open(SERVER_FIFO, O_WRONLY));
	while( (len = read(rd, buff, MAXLEN-1)) > 0)
	{
		if(buff[len-1] == '\n')
			buff[--len] = '\0';
		else
			buff[len] = '\0';
		if((ptr = strchr(buff, ' '))==NULL)
		{
			printf("Error request:%s\n", buff);
			continue;
		}

		*ptr++ = '\0';
		pid = atoi(buff);
		snprintf(fifoname, MAXLEN, "/tmp/fifo.%d", pid);
		int cfifo;
		if((cfifo = open(fifoname, O_WRONLY)) < 0)
		{
			printf("Can't open client fifo:%s\n", fifoname);
			continue;
		}

		int fd;
		char fdata[1024];
		int flen;
		if((fd = open(ptr, O_RDONLY)) < 0)
		{
			snprintf(fdata, 1024, "%s:%s\n", ptr, strerror(errno));
			write(cfifo, fdata, strlen(fdata));
			close(cfifo);
		}
		else
		{
			while((flen = read(fd, fdata, 1024)) > 0)
				write(cfifo, fdata, flen);
			close(cfifo);
			close(fd);
		}
	}
	unlink(SERVER_FIFO);
	return 0;
}
