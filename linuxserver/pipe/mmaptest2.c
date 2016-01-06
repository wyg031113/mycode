#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>

int main()
{
	int filefd = open("app.txt", O_RDWR);
	char *data = (char*)mmap(NULL, 10000, PROT_READ|PROT_WRITE, MAP_SHARED, filefd, 0);
	printf("%s\n", data);
	printf("##################\n");
	int i;
	for(i = 0; i < 1000; i++)
	{
		printf("data:%s\n", data);
		sleep(2);
	}
	munmap(data, 10000);
	close(filefd);
	return 0;
}
