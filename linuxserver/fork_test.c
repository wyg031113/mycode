#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
static void handle_child(int sig)
{
	pid_t pid;
	int stat;
	while((pid=waitpid(-1,&stat, WNOHANG))>0)
	{
		printf("child:%d, exit:%d\n", pid, WEXITSTATUS(stat));
	}
}
int main()
{
	signal(SIGCHLD, handle_child);
	pid_t pid;
	pid = fork();
	if(pid == -1)
	{
		printf("Fork failed!!\n");
	}
	else if(pid == 0)
	{
		printf("I am child!!\n");
		exit(123);
	}
	else
	{
		printf("I am father!\n");
		sleep(2);
		exit(0);
	}
}
