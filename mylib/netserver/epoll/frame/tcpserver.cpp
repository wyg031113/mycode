#include "tcpserver.h"
#include "util.h"
#include <malloc.h>
Tcpserver::Tcpserver(PollQueue *pollqueue, unsigned short port):
			pollqueue_(pollqueue), port_(port)
{
	//listen
	struct sockaddr_in ser_addr;
	int flag = 1, flag_len = sizeof(int);

	ser_addr.sin_family = AF_INET;
	ser_addr.sin_addr.s_addr = INADDR_ANY;
	ser_addr.sin_port = htons(port_);
	bzero(&ser_addr.sin_zero, 8);

	CHECK2(serfd_, socket(AF_INET, SOCK_STREAM, 0));
	CHECK(setsockopt(serfd_, SOL_SOCKET, SO_REUSEADDR, &flag, flag_len));
	CHECK(bind(serfd_, (struct sockaddr*)&ser_addr, sizeof(struct sockaddr)));
	CHECK(listen(serfd_, 5));
	CallbackBase * cb = new CallbackObj<Tcpserver>(this, &Tcpserver::accept_callback);
	pollqueue_->poll(serfd_, PollQueue::Readable, cb);
}

void Tcpserver::accept_callback(void)
{
	cout<<"Accept callback running..."<<endl;
	int clifd = ::accept(serfd_, NULL, NULL);
	CallbackBase * cb = new CallbackObj<Tcpserver>(this, &Tcpserver::accept_callback);
	pollqueue_->poll(serfd_, PollQueue::Readable, cb);
	
	Socket *s = new Socket(pollqueue_, clifd);
	CallbackBase * cbs = new CallbackObj<Socket>(s, &Socket::read_callback);
	pollqueue_->poll(clifd, PollQueue::Readable, cbs);
}

Socket::Socket(PollQueue *pollqueue, int fd):
		pollqueue_(pollqueue), fd_(fd)
{
	readbuf = NULL;
	writebuf = NULL;
	read_len = 0;
	write_len = 0;
}

void Socket::read_callback()
{
	cout<<"Socket read callback running"<<endl;
	if(writebuf == NULL)
	{
		writebuf = (unsigned char *)malloc(100);
		write_len = 0;
	}
	int ret = read(fd_, writebuf + write_len, 10);
	if(ret <= 0)
	{
		pollqueue_->cancel(fd_, PollQueue::Readable);
		pollqueue_->cancel(fd_, PollQueue::Writable);
		cout<<"canceld"<<endl;
		close(fd_);
		delete this;
		return;
	}
	write_len += ret;
	CallbackBase * cb = new CallbackObj<Socket>(this, &Socket::write_callback);
	pollqueue_->poll(fd_, PollQueue::Writable, cb);
	if(ret == 10)
	{
		cb = new CallbackObj<Socket>(this, &Socket::read_callback);
		pollqueue_->poll(fd_, PollQueue::Readable, cb);
	}


}

void Socket::write_callback()
{
	int ret = write(fd_, writebuf, write_len>10?write_len-6:write_len);
	if(ret < 0)
		return;
	write_len -= ret;
	if(write_len != 0)
	{
		cout<<"write again...: write_len = "<<write_len<<endl;
		memmove(writebuf, writebuf+ret, write_len);
		CallbackBase * cb = new CallbackObj<Socket>(this, &Socket::write_callback);
		pollqueue_->poll(fd_, PollQueue::Writable, cb);
	}
}

Socket::~Socket()
{
	if(readbuf != NULL)
		free(readbuf);
	if(writebuf != NULL)
		free(writebuf);
	cout<<"Socket destory...."<<endl;
}
