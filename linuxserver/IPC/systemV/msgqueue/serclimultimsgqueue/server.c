#include <inc.h>
#include "summsg.h"
volatile int flag = 1;
void ctrlc(int sig)
{
	flag = 0;	
}
int main(int argc, char *argv[])
{
	if(argc != 2)
		err_quit("Usage:server name");
	int mqd;
	CHECK(mqd = msgget(ftok(argv[1], 1), IPC_CREAT|0666));
	system("ipcs -q");
	struct SumReq srq;
	struct SumRep srp;
	signal(SIGINT, ctrlc);
	printf("Server started!\n");
	while(flag)
	{
		int ret = msgrcv(mqd, &srq, sizeof(srq)-sizeof(srq.type), SERTYPE, 0);
		if(ret==-1 && errno==EINTR)
			continue;
		srp.type = 1;
		srp.sum = srq.x + srq.y;
		msgsnd(srq.climqd, &srp, sizeof(srp) - sizeof(srp.type), 0);
	}
	msgctl(mqd, IPC_RMID, NULL);
	return 0;
}
