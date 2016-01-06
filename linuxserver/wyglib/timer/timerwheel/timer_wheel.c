#include "timer_wheel.h"
#include <signal.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#define MAXSLOT 1024
static struct Timer slot[MAXSLOT];
static int cur;
static int list_empty(struct Timer *ListHead)
{
	return (ListHead->next == ListHead);
}

/*add e2 before e1*/
static void list_add_before(struct Timer *e1, struct Timer *e2)
{
	e2->next = e1;
	e2->prev = e1->prev;
	e1->prev->next = e2;
	e1->prev = e2;
}
/*remove e*/
static void list_remove(struct Timer *e)
{
	e->prev->next = e->next;
	e->next->prev = e->prev;
}

void tick(int sig)
{
	int eno = errno;
//	printf("Tick..\n");
	cur = (cur + 1) % MAXSLOT;
	struct Timer *p = slot[cur].next;
	while(p != &slot[cur])
	{
		p->tms--;
		if(p->tms == 0)
		{
			del_timer(p);
			struct Timer *q = p->next;
			p->cb(p);
			p = q;
		}
		else
			p = p->next;
	}
	errno = eno;
}
void start_timer()
{
	int i;
	for(i = 0; i < MAXSLOT; i++)
		slot[i].next = slot[i].prev = &slot[i];
	signal(SIGALRM, tick);
	struct itimerval itv;
	itv.it_interval.tv_sec = 0;
	itv.it_interval.tv_usec = TimeSlot*1000;
	itv.it_value.tv_sec = 0;
	itv.it_value.tv_usec = TimeSlot*1000;
	setitimer(ITIMER_REAL, &itv, NULL);
}
void add_timer(struct Timer *timer)
{
	int cnt = (timer->msec / TimeSlot)+((timer->msec % TimeSlot)!=0?1:0);
	timer->tms = cnt/MAXSLOT + ((cnt%MAXSLOT) !=0?1:0);
	int pos = (cnt + cur) % MAXSLOT;
	list_add_before(slot[pos].prev, timer);
}

void del_timer(struct Timer *timer)
{
	list_remove(timer);
}
void adjust_timer(struct Timer *timer)
{
	del_timer(timer);
	add_timer(timer);
}


