#include <inc.h>
#include "summsg.h"
int main(int argc, char *argv[])
{
	if(argc != 2)
		err_quit("Usage:client sername");
	srand(time(0));
	int mqd;
	CHECK(mqd = msgget(IPC_PRIVATE, 0));
	int mqdser;
	CHECK(mqdser = msgget(ftok(argv[1],1), 0));
	system("ipcs -q");
	struct SumReq srq;
	struct SumRep srp;
	printf("client started!\n");
	int i = 6;
	while(i--)
	{
		srq.type = SERTYPE;
		srq.climqd = mqd;
		srq.x = rand()%100;
		srq.y = rand()%100;
		msgsnd(mqdser, &srq, sizeof(srq) - sizeof(srq.type),0);
		if(msgrcv(mqd, &srp, sizeof(srp)-sizeof(srp.type), 0, 0)>0)
			printf("%d + %d = %d\n", srq.x, srq.y, srp.sum);
		sleep(3);
	}
	msgctl(mqd, IPC_RMID, NULL);
	return 0;
}
