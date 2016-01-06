#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
void tmr(int sig)
{
	printf("Timer\n");
	//alarm(2);
}

int main()
{
	signal(SIGALRM, tmr);
	struct itimerval itv;
	itv.it_interval.tv_sec = 0;
	itv.it_interval.tv_usec = 500000;
	itv.it_value.tv_sec = 0;
	itv.it_value.tv_usec = 500000;
	//setitimer(ITIMER_REAL, &itv, NULL);
	alarm(2);
	alarm(10);
	int i = 0;
	while(1)
	{
		printf("%d while.\n", ++i);
		pause();
	}
}
