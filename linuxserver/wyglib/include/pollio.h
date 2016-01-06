#ifndef __POLLIO_H__
#define __POLLIO_H__
#include <stdio.h>
enum IOEvent{POLL_READ=1, POLL_WRITE=2,POLL_EXCEPT=4, POLL_ERROR=8};
struct Event
{
	enum IOEvent event;
	int fd;
};
typedef void (*handle_t)(struct Event e);
struct Task
{
	handle_t handle;
	enum IOEvent ioe;
};

void init_poll();
int submit_task(int fd, enum IOEvent ioe);
int cancel_task(int fd, enum IOEvent ioe);
int run();
void destroy_poll();
#endif /*__POLLIO_H__*/
