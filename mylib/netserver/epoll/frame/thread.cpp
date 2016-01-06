#include "thread.h"
#include <stdlib.h>
void *Thread_Func(void *args)
{
	Thread *t = (Thread*)args;
	t->run();
	return NULL;
}
Thread::Thread(std::string name):
		name_(name),
		ts_(Thread_NotCreated)
{
}
Thread::~Thread()
{
}
void Thread::start(void)
{
	if(ts_ == Thread_NotCreated)
	{
		int rt = pthread_create(&tid_, NULL, Thread_Func, this);
		if(rt < 0)
			exit(-1);
		ts_ == Thread_Ready;
	}
}

void Thread::run(void)
{
	ts_ = Thread_Running;
	main();
	ts_ = Thread_Exited;
}

void Thread::join(void)
{
	int rv = pthread_join(tid_, &ret_);
	if(rv == -1)
	{
		;	
	}
}

pthread_t Thread::get_tid()
{
	return tid_;
}	
