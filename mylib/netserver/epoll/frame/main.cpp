#include "thread.h"
#include <iostream>
#include "callback.h"
#include "tcpserver.h"
using namespace std;
class MyThread:public Thread
{
public:
	MyThread(std::string name):
			Thread(name)
	{
	}
	~MyThread()
	{
	}
	virtual void main()
	{
		cout<<"Hello world:"<<name_<<": "<<get_tid()<<endl;
		while(!cbq_.idle() || !pollqueue_.idle())
		{
			if(!cbq_.idle())
				cbq_.perform();
			if(!pollqueue_.idle() && !cbq_.idle())
				pollqueue_.wait(0);

			if(!pollqueue_.idle() && cbq_.idle())
				pollqueue_.wait(-1);
		}
	}
	void schedule(CallbackBase *cb)
	{
		cbq_.schedule(cb);
	}
	void poll(int fd, PollQueue::Type tp, CallbackBase *cb)
	{
		pollqueue_.poll(fd, tp, cb);
	}
	void wait(int tv)
	{
		pollqueue_.wait(tv);
	}
public:
	CallbackQueue cbq_;
	PollQueue pollqueue_;
};


class MyCallback:public CallbackBase
{
public:
	MyCallback():CallbackBase(){}
	virtual void execute()
	{
		cout<<"Hello, mycallback running..."<<endl;
	}
private:
	virtual ~MyCallback(){cout<<"~MyCallback() running.."<<endl;}
};
int main()
{
	MyThread mt("wyg");
	MyCallback *mc = new MyCallback;
	mt.schedule(mc);
	mc = new MyCallback();
	mt.schedule(mc);
	
	Tcpserver(&mt.pollqueue_, 1234);
	mt.start();
	mt.join();
	return 0;
}
