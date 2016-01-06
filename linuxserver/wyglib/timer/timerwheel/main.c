#include <stdlib.h>
#include <stdio.h>
#include "timer_wheel.h"
void cb_test(struct Timer *timer)
{
	printf("cb test run....\n");
	add_timer(timer);
}
int main()
{
	start_timer();
	struct Timer tm[10];
	tm[0].msec = 3000;
	tm[0].cb = cb_test;
	tm[1].msec = 50000;
	tm[1].cb = cb_test;
	add_timer(&tm[0]);
	add_timer(&tm[1]);
	while(1)
		pause();
}
