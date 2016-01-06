#include <sys/resource.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
int daemonize()
{
	pid_t pid = fork();
	if(pid < 0)
	{
		printf("fork failed!\n");
		return -1;
	}
	else if(pid > 0)
	{
		printf("main process exited.\n");
		exit(0);
	}
	else
	{
		umask(0);
		if(setsid()<0)
		{
			printf("setsid failed\n");
			exit(-1);
		}
		if(chdir("/")<0)
		{
			printf("chdir failed\n");
			exit(-1);
		}

		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		close(STDERR_FILENO);
		open("/dev/null", O_RDONLY);
		open("/dev/null", O_RDWR);
		open("/dev/null", O_RDWR);
		return 0;
	}
}
int main()
{
	//daemonize();
	daemon(0,0);
	struct rlimit lmt;
	getrlimit(RLIMIT_NOFILE, &lmt);
	printf("DataSegmentSize:%d %d\n",lmt.rlim_cur, lmt.rlim_max); 
	char pwd[1024];
	
	chroot(".");
	chdir("/");
	char *s = getcwd(NULL, 1024);
	printf("PWD=%s\n", s); 
	free(s);
	int fd = open("/testlimit.c", O_RDONLY);
	int ret;
	while((ret=read(fd, pwd, 1023))>0)
	{
		pwd[ret]=0;
		printf("%s", pwd);
	}
	close(fd);
	sleep(20);
	return 0;
}

