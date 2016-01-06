#ifndef __TCPSERVER_H__
#define __TCPSERVER_H__
#include <string>
#include <malloc.h>
#include "callback.h"
class Socket
{
public:
	Socket(PollQueue *pollqueue, int fd);
	void read_callback();
	void write_callback();
private:
	unsigned char *readbuf;
	unsigned int read_len;

	unsigned char *writebuf;
	unsigned int write_len;
	int fd_;
	PollQueue *pollqueue_;
	~Socket();
};
class Tcpserver
{
public:
	Tcpserver(PollQueue *pollqueue, unsigned short port);
	void accept_callback(void);
private:
	unsigned short port_;
	int serfd_;
	PollQueue *pollqueue_;
};
#endif /* __TCPSERVER_H__ */
