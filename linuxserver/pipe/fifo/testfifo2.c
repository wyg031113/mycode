#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#define CHECK(x) do{if((x)==-1){fprintf(stderr,"%s(%s-%d):%s\n",__FILE__, __FUNCTION__, __LINE__, strerror(errno));}}while(0)
void client()
{
	int wd = open("wyg1", O_WRONLY, 0);
	printf("open wyg2 success\n");
	printf("Please input file name:\n");
	char buf[1024];
	fgets(buf, 1024, stdin);
	int len = strlen(buf);
	if(buf[len-1]=='\n')
		len--;
	write(wd,buf, len);
	close(wd);
	int ret;
	int rd = open("wyg2", O_RDONLY, 0);
	printf("open wyg1 success\n");
	while((ret=read(rd, buf, 1024))>0)
		write(STDOUT_FILENO, buf, ret);
	printf("\n");
	
	close(rd);

}

int main()
{
	mkfifo("wyg1", 0660);
	mkfifo("wyg2", 0660);
	client();		
}
