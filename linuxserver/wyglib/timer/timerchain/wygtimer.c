#include "wygtimer.h"
#include <signal.h>
#include <stdio.h>
static struct Timer ListHead = {.next=&ListHead, .prev=&ListHead};
static int list_empty()
{
	return (ListHead.next == &ListHead);
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
static reset_timer();
static int beinited;
static void ticks()
{
	while(!list_empty())
	{
		struct Timer *t = ListHead.next;
		if(t->expire > time(NULL))
			break;
		del_timer(t);
		t->cb(t);
	}
	reset_timer();
}
static reset_timer()
{
	time_t diff = ListHead.next->expire - time(NULL);
	if(list_empty())
		alarm(0);
	else
	{
		if(diff<=0)
			alarm(1);
		else
			alarm(diff);
	}
}
static void timer_sig(int sig)
{
	ticks();	
}

void start_timer()
{
	signal(SIGALRM, timer_sig);
	ListHead.next = ListHead.prev = &ListHead;
	beinited = 1;
}

int add_timer(struct Timer *timer)
{

	if(!beinited)
		return -1;
	struct Timer *p = NULL;
	for(p = ListHead.next; p != &ListHead; p = p->next)
		if(p->expire >timer->expire)
			break;
	list_add_before(p, timer);
	reset_timer();
	return 0;
}

void del_timer(struct Timer *timer)
{
	if(!beinited)
		return;
	list_remove(timer);
}
int adjust_timer(struct Timer *timer)
{
	if(!beinited)
		return -1;
	reset_timer();
	del_timer(timer);
	return add_timer(timer);
}

void show_list()
{
	struct Timer *p = ListHead.next;
	printf("List:\n");
	while(p != &ListHead)
	{
		printf("Timer:%d\n", p->expire);
		p=p->next;
	}

}

void tcb(struct Timer *timer)
{
	printf("cb-Timer:%d\n", timer->expire);
}
void test_list()
{
	static struct Timer timer[4];
	time_t cur = time(NULL);
	timer[0].expire = cur+10;
	timer[1].expire = cur+1;
	timer[2].expire = cur+2;
	timer[3].expire = cur+10;
	int i;
	for(i = 0; i < 4; i++)
	{
		timer[i].cb = tcb;
		add_timer(&timer[i]);
	}
	show_list();
	show_list();
	/*timer[3].expire = 34;
	adjust_timer(&timer[3]);
	show_list();
	del_timer(&timer[1]);
	del_timer(&timer[3]);
	del_timer(&timer[0]);
	del_timer(&timer[2]);
	show_list();
	*/
}

int main()
{
	start_timer();
	test_list();
	while(1)
		pause();
}
