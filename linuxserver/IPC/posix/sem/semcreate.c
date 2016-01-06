#include <inc.h>

int main(int argc, char *argv[])
{
	int c, flags;
	sem_t *sem;
	unsigned int value;
	flags = O_RDWR|O_CREAT;
	value = 1;
	while((c=getopt(argc, argv, "ei:"))!=-1)
	{
		switch(c)
		{
			case 'e':
				flags |= O_EXCL;
				break;
			case 'i':
				value = atoi(optarg);
				break;
		}
	}
	if(optind != argc -1)
		err_quit("usage: semcreate [-e] [-i initalvalue] <name>");
	if((sem=sem_open(argv[optind], flags, 0666, value))==SEM_FAILED)
		err_quit("create sem failed! error:%s\n", strerror(errno));
	int x = 0;
	sem_getvalue(sem, &x);
	printf("%s:%d\n", argv[optind], x);
	sem_close(sem);
	return 0;
}
