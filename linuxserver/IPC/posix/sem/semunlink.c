#include <inc.h>

int main(int argc, char *argv[])
{
	if(argc != 2)
		err_quit("usage:semunlink <name>"); 
	sem_unlink(argv[1]);
	return 0;
}
