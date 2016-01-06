#ifndef __TIMER_WHEEL_H__
#define __TIMER_WHEEL_H__
#define TimeSlot 100 /*100ms*/
#include <sys/time.h>
struct Timer;
typedef void(*TimerCallback)(struct Timer *timer);
struct Timer
{
	unsigned long msec;
	unsigned long tms;
	TimerCallback cb;
	union UserData
	{
		int d;
		void *p;
	}data;
	struct Timer *next;
	struct Timer *prev;
};
void start_timer();
void add_timer(struct Timer *timer);
void del_timer(struct Timer *timer);
void adjust_timer(struct Timer *timer);
#endif /*__TIMER_WHEEL_H__*/
