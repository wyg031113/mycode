#include <inc.h>
#include "summsg.h"
void waitchild(int sig)
{
	printf("a son exited!\n");
	pid_t pid;
	int stat;
	while((pid=waitpid(-1, &stat, WNOHANG))>0)
		printf("son:%d exited\n", pid);
}
int main(int argc, char *argv[])
{
	if(argc != 2)
		err_quit("Usage:server name");
	int mqd;
	signal(SIGCHLD, waitchild);
	CHECK(mqd = msgget(ftok(argv[1], 1), IPC_CREAT|0666));
	system("ipcs -q");
	struct SumReq srq;
	struct SumRep srp;
	printf("Server started!\n");
	while(1)
	{
		int ret = msgrcv(mqd, &srq, sizeof(srq)-sizeof(srq.type), SERTYPE, 0);
		if(ret==-1 && errno==EINTR)
			continue;
		printf("a request\n");
		if(fork()==0)
		{
			printf("Child process\n");
			srp.type = srq.pid;
			srp.sum = srq.x + srq.y;
			msgsnd(mqd, &srp, sizeof(srp) - sizeof(srp.type), 0);
			exit(0);
		}
		else
		{
			printf("Father listening...\n");
		}
	}
	return 0;
}
