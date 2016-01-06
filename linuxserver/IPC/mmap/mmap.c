#include <inc.h>
int main()
{
	char *addr;
	CHECK((addr=(char*)mmap(NULL, 1024, PROT_WRITE|PROT_READ, MAP_SHARED|MAP_ANONYMOUS, -1, 0)));
	if(fork()==0)
	{
		sleep(1);
		printf("%c\n", addr[0]);
	}
	else
	{
		addr[0] = 'y';
		waitpid();
	}
	return 0;
}
