#include <inc.h>
#include "summsg.h"
int main(int argc, char *argv[])
{
	if(argc != 2)
		err_quit("Usage:client ser_msq_name");
	srand(time(0));
	int mqd;
	CHECK(mqd = msgget(ftok(argv[1], 1), 0));
	system("ipcs -q");
	struct SumReq srq;
	struct SumRep srp;
	printf("client started!\n");
	while(1)
	{
		srq.type = SERTYPE;
		srq.pid = getpid();
		srq.x = rand()%100;
		srq.y = rand()%100;
		msgsnd(mqd, &srq, sizeof(srq) - sizeof(srq.type),0);
		if(msgrcv(mqd, &srp, sizeof(srp)-sizeof(srp.type), srq.pid, 0)>0)
			printf("%d + %d = %d\n", srq.x, srq.y, srp.sum);
		sleep(3);
	}
	return 0;
}
