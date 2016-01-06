#ifndef __THREAD_H__
#define __THREAD_H__
#include <pthread.h>
#include <string>
enum ThreadStatus {Thread_NotCreated, Thread_Ready, Thread_Running, Thread_Exited};
class Thread
{
public:
	Thread(std::string name);
	virtual ~Thread();
	void start(void);
	void run(void);
	void join();
	pthread_t get_tid();
	const std::string name_;
private:
	virtual void main(void) = 0;
	pthread_t tid_;
	ThreadStatus ts_;
	void *ret_;
};

#endif /* __THREAD_H__ */
