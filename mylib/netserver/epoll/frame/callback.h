#ifndef __CALLBACK_H__
#define __CALLBACK_H__
#include <queue>
#include <map>
#include "util.h"
class CallbackBase
{
friend class CallbackQueue;
friend class PollQueue;
public:
	CallbackBase(){}
	virtual void execute() = 0;
protected:
	virtual ~CallbackBase(){};
};

class CallbackQueue
{
public:
	CallbackQueue(){}
	~CallbackQueue();
	void schedule(CallbackBase *cb);
	void perform();
	bool idle();
private:
	std::queue<CallbackBase *> queue_;
};

class PollQueue
{
public:
	enum Type{Readable, Writable};
	PollQueue();
	~PollQueue();
	void poll(int fd, Type tp, CallbackBase *cb);
	void wait(int tv);
	void cancel(int fd, Type tp);
	bool idle(){ return readfd_.empty() && writefd_.empty(); }
	CallbackBase *get_callback(int fd, Type tp);
private:
	std::map<int, CallbackBase *> readfd_, writefd_;
	int epollfd_;
};

template<class C>
class CallbackObj:public CallbackBase
{
public:
	typedef void (C::*method_t)(void);
	//template<typename T>
	CallbackObj(C *obj, method_t  method):obj_(obj), method_(method){};
	void execute()
	{
		(obj_->*method_)();
	}
	~CallbackObj(){cout<<"CallbackObj destroy..."<<endl;}
private:
	C *const obj_;
	method_t method_;
};
#endif /* __CALLBACK_H__ */
