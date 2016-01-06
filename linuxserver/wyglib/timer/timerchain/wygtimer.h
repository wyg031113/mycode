#ifndef __WYGTIMER_H__
#define __WYGTIMER_H__
#include <sys/time.h>
struct Timer;
typedef  void (*timer_callback_t)(struct Timer *timer);
struct Timer
{
	time_t expire;
	timer_callback_t cb;
	union UserData
	{
		int d;
		void *ptr;
	}data;
	struct Timer *next;
	struct Timer *prev;
};

int add_timer(struct Timer *timer);
void del_timer(struct Timer *timer);
int adjust_timer(struct Timer *timer);
void start_timer();
#endif /* __WYGTIMER_H__*/
