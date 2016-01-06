#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>

int main()
{
	char *data = (char*)mmap(NULL, 10000, PROT_READ|PROT_WRITE, 
							MAP_SHARED|MAP_ANONYMOUS, -1, 0);
	printf("%s\n", data);
	printf("##################\n");
	int i;
	pid_t pid;
	pid = fork();
	for(i = 0; i < 1000; i++)
	{
		if(pid >0)
			data[0] = 'a'+i%16;
		else
			printf("%c\n", data[0]);
		sleep(2);
	}
	munmap(data, 10000);
	return 0;
}
