#include "callback.h"
#include "util.h"
void CallbackQueue::schedule(CallbackBase *cb)
{
	queue_.push(cb);
}

void CallbackQueue::perform()
{
	CallbackBase *cb = queue_.front();
	cb->execute();
	queue_.pop();
	delete cb;
}

bool CallbackQueue::idle()
{
	return queue_.empty();
}
CallbackQueue::~CallbackQueue()
{
	while(!queue_.empty())
	{
		CallbackBase *cb = queue_.front();
		queue_.pop();
		delete cb;
	}
}

PollQueue::PollQueue()
{
	CHECK2(epollfd_, epoll_create(1024));
}
PollQueue::~PollQueue()
{
	close(epollfd_);
}
void PollQueue::poll(int fd, Type tp, CallbackBase *cb)
{
	struct epoll_event ev;
	bool exist = false;
	if(tp == Readable)
	{
		if(readfd_.find(fd) != readfd_.end())
		{
			delete readfd_[fd];
			exist = true;
		}
		readfd_[fd] = cb;

		ev.events = EPOLLIN | EPOLLET;
		ev.data.fd = fd;
		if(writefd_.find(fd) != writefd_.end())
		{
			ev.events |= EPOLLOUT;
			exist = true;
		}
	}
	else if(tp == Writable)
	{
		if(writefd_.find(fd) != writefd_.end())
		{
			delete writefd_[fd];
			exist = true;
		}
		writefd_[fd] = cb;

		ev.data.fd = fd;
		ev.events = EPOLLOUT | EPOLLET;
		if(readfd_.find(fd) != writefd_.end())
		{
			ev.events |= EPOLLIN;
			exist = true;
		}

	}
	CHECK(epoll_ctl(epollfd_, exist ? EPOLL_CTL_MOD : EPOLL_CTL_ADD, fd, &ev));
}

void PollQueue::wait(int tv)
{
	int i, nfds;	
	struct epoll_event events[1024];
	CHECK2(nfds, epoll_wait(epollfd_, events, 1024, tv));
	if(nfds == 0)
		cout<<"Timed out!!"<<endl;
	for(i = 0; i < nfds; i++)
	{
		if(events[i].events & EPOLLIN)
		{
			cout<<"EPOLLIN"<<endl;
			readfd_[events[i].data.fd]->execute();
		}
		else if(events[i].events & EPOLLOUT)
		{
			cout<<"EPOLLOUT"<<endl;
			writefd_[events[i].data.fd]->execute();
		}

	}
}

void PollQueue::cancel(int fd, Type tp)
{
	std::map<int, CallbackBase*>::iterator ite;
	if(tp == Readable)
	{
		if((ite=readfd_.find(fd)) != readfd_.end())
		{
			delete readfd_[fd];
			readfd_.erase(ite);
		}
	
	}
	else if(tp == Writable)
	{
		if((ite=writefd_.find(fd)) != writefd_.end())
		{
			delete writefd_[fd];
			writefd_.erase(ite);
		}
	}
	if( (readfd_.find(fd) == readfd_.end()) && (writefd_.find(fd) == writefd_.end()))
	{
		epoll_ctl(epollfd_, EPOLL_CTL_DEL, fd, NULL);
	}
}

CallbackBase *PollQueue::get_callback(int fd, Type tp)
{
	return NULL;
}
