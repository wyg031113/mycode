#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>

void handle_child(int sig)
{
	pid_t pid;
	int stat;
	printf("MyPID:%d MyFather:%d\n", getpid(), getppid());
	while((pid=waitpid(-1, &stat, WNOHANG))>0)
	{
		printf("child:%d, exit:%d\n", pid, WEXITSTATUS(stat));
	}
}
void handle_alarm(int sig)
{
	printf("pid:%d Alarm!!!\n", getpid());
}
int main()
{
	char *buf = (char*) malloc(1024);
	int fd = open("hello.c", O_RDONLY);
	printf("fd=%d\n", fd);
	strcpy(buf, "Hello world");
	signal(SIGCHLD, handle_child);
	signal(SIGALRM, handle_alarm);
	int i;
	pid_t pid;
//	for(i = 0; i<5&&((pid=fork())>0); i++)
//		;
//	for(i = 0; i < 4; i++)
		pid = fork();
	if(pid < 0)
		exit(-1);
	else if(pid == 0)
	{
		printf("son running!  %s\n", buf);
		execlp("./hello", "hello", NULL);
		free(buf);
		alarm(1);
		sleep(5);
		exit(0);
	}
	else
	{
		buf[0]='I';
		printf("Father running! %s\n", buf);
		free(buf);
		alarm(1);
		int k = 0;
		while(k++ < 3)
			sleep(1);
		//wait(NULL);
		exit(0);
	}

	return 0;
}
