#include <inc.h>
#include "summsg.h"
int main(int argc, char *argv[])
{
	if(argc != 2)
		err_quit("Usage:server name");
	int mqd;
	CHECK(mqd = msgget(ftok(argv[1], 1), IPC_CREAT|0666));
	system("ipcs -q");
	struct SumReq srq;
	struct SumRep srp;
	printf("Server started!\n");
	while(1)
	{
		msgrcv(mqd, &srq, sizeof(srq)-sizeof(srq.type), SERTYPE, 0);
		srp.type = srq.pid;
		srp.sum = srq.x + srq.y;
		msgsnd(mqd, &srp, sizeof(srp) - sizeof(srp.type), 0);
	}
	return 0;
}
