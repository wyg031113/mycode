#include <inc.h>

int main(int argc, char *argv[])
{
	if(argc != 2)
		err_quit("Usage: mqunlink <name>");
	int mqd;
	CHECK(mqd = msgget(ftok(argv[1],1), 0));
	CHECK(msgctl(mqd, IPC_RMID, NULL));
	system("ipcs -q");
	exit(0);
}
