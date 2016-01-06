#include<iostream>
#include<time.h>
#include<stdlib.h>
#include<stdio.h>
#include<list>
#include<errno.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string>
#include<map>
#include<set>
#include<queue>
using namespace std;

/*基于select的异步框架 纯虚基类 需要继承
 * 实现虚函数：
 *  handle_read(int fd);
 *  handle_write(fd);
 */
class AsynSelect
{
public:
	AsynSelect(bool block = true, int sec = 0, int usec = 0)
	{
		isBlock = block;
		tv.tv_sec = sec;
		tv.tv_usec = usec;
	}
	//添加删除读写集合里的描述符
	void addRfds(int fd) { rfds.insert(fd); }
	void addWfds(int fd) { wfds.insert(fd); }
	void removeRfds(int fd) { rfds.erase(fd); }
	void removeWfds(int fd) { wfds.erase(fd); }
	//实现了用read write系统函数的数据读写, 并且自动删除没用的描述符
	int  read_data(int fd, void *buf, int buf_len)
	{
		int nread = read(fd, buf, buf_len);
		if(nread <= 0)
		{
			removeRfds(fd);
			removeWfds(fd);
			close(fd);
			return -1;
		}
		return nread;
	}

	int write_data(int fd, const void *buf, int buf_len)
	{
		int nwrite = write(fd, buf, buf_len);
		if(nwrite <= 0)
		{
			removeWfds(fd);
			return -1;
		}
		return nwrite;
	}
	//纯虚函数, 子类实现，处理读写
	virtual void handle_read(int fd)=0;
	virtual void handle_write(int fd)=0;
	virtual void handle_timedout(){}
	//select 核心
	void run()
	{
		fd_set rfdset, wfdset;
		set<int> tr,tw;
		while(rfds.size()>0 || wfds.size()>0 )
		{
			FdIterator fdIte;
			FD_ZERO(&rfdset);
			FD_ZERO(&wfdset);
			tr = rfds;
			tw = wfds;
			int maxfd = 0;
			//加入读描述符
			for(fdIte = tr.begin(); fdIte != tr.end(); fdIte++)
			{
				FD_SET(*fdIte, &rfdset);
				maxfd = maxfd < *fdIte ? *fdIte : maxfd;
			}

			//加入写描述符
			for(fdIte = tw.begin(); fdIte != tw.end(); fdIte++)
			{
				FD_SET(*fdIte, &wfdset);
				maxfd = maxfd < *fdIte ? *fdIte : maxfd;
			}

			int ret = 0;
			if(!isBlock)	//非阻塞
			{
				struct timeval ttv = tv;
				ret = select(maxfd+1, &rfdset, &wfdset, NULL, &ttv);
			}
			else			//阻塞
			{
				ret = select(maxfd+1, &rfdset, &wfdset, NULL, NULL);
			}
			switch(ret)
			{
			case -1:		//出错
				printf("select error! errno-->%s\n",strerror(errno));
				break;
			case  0:		//超时
				printf("Select timed out!\n");
				handle_timedout();
				break;
			default: 		//处理读写
				for(fdIte = tr.begin(); fdIte != tr.end(); fdIte++)
					if(FD_ISSET(*fdIte, &rfdset))
						handle_read(*fdIte);

				for(fdIte = tw.begin(); fdIte != tw.end(); fdIte++)
					if(FD_ISSET(*fdIte, &wfdset))
						handle_write(*fdIte);


			}
		}
	}
private:
	set<int> rfds,wfds;
	typedef set<int>::iterator FdIterator;
	bool isBlock;
	struct timeval tv;
};

/*	TCP 服务器 构造函数中给出端口号 定义对象后,自动监听
 */
class TcpServer:public AsynSelect
{
public:
	//port--监听端口号 监听所有网卡
	//block--true表示使用超时机制,sec,usec要设置好对应时间
	TcpServer(int port, bool block = true, int sec = 1, int usec = 0):AsynSelect(block, sec, usec)
	{
		//1.创建监听socket, 加入到父类rfd中
		ser_fd = socket(AF_INET, SOCK_STREAM, 0);
		if(ser_fd < 0)
		{
			printf("TcpServer: get socket failed\n");
			exit(-1);
		}
		//2.创建监听地址和端口
		ser_addr.sin_family = AF_INET;
		ser_port = port;
		ser_addr.sin_port = htons(port);
		ser_addr.sin_addr.s_addr = INADDR_ANY;
		bzero(ser_addr.sin_zero, 8);
		//3.设置地址复用
		int opt=1;
		socklen_t optlen=sizeof(opt);
		setsockopt(ser_fd, SOL_SOCKET, SO_REUSEADDR, &opt, optlen);
		//4.绑定地址端口和socket
		if(bind(ser_fd, (struct sockaddr*)&ser_addr, sizeof(struct sockaddr))==-1)
		{
			printf("Bind failed! errno-->:%s\n", strerror(errno));
			exit(-1);
		}
		//5.监听socket
		if(listen(ser_fd, 8)  == -1)
		{
			printf("Listen failed! errno-->:%s\n", strerror(errno));
			exit(-1);
		}
		//6.加入select 异步IO
		addRfds(ser_fd);
		printf("Tcp Server started! listen on port %d.\n",ser_port);
	}

	//子类应该调用这个函数来接受连接
	//这里自动把客户端socket加入select
	int tcp_accept()
	{
		printf("tcp accept!\n");
		int fd = accept(ser_fd, NULL, NULL);
		if(fd == -1)
		{
			printf("Accept failed! errno-->:%s\n", strerror(errno));
			return(-1);
		}
		addRfds(fd);
		return fd;
	}

	//子类应该重写这个虚函数，并且调用tcp_accept()来接受新连接
	virtual void handle_accept()
	{
		tcp_accept();
		//addWfds(fd);
	}

	//子类重写这个虚函数，用于读取客户端的数据
	//当客户端有数据时，这个函数会被调用
	virtual void handle_read_data(int fd)
	{
		printf("Handle read data!\n");
		char buf[256];
		int nread = tcp_read(fd, buf, 256);
		if(nread <= 0)
		{
			return;
		}
		buf[nread] = 0;
		printf("read %d bytes: %s", nread, buf);
		addWfds(fd);
	}

	//这个函数不应该被重写， 除非想做高级扩展
	virtual void handle_read(int fd)
	{
		printf("Handle read!\n");
		if(fd == ser_fd)
		{
			handle_accept();
			return;
		}
		handle_read_data(fd);
	}

	//如果需要异步写就重写这个函数
	virtual void handle_write(int fd)
	{
		/*
		printf("Handle write!\n");
		char buf[256] = "Hello, test TcpServer write!\n";
		int nsend = tcp_write(fd, buf, strlen(buf));
		printf("send %d bytes: %s", nsend, buf);
		*/
		DataPacket *dp = mfds[fd].front();
		int nsend = send(fd, dp->data, dp->dataLen, 0);
		if(nsend <= 0)
			handle_disconnect(fd);
		else
		{
			dp->depress(nsend);
		    if(dp->dataLen == 0)
			{
				mfds[fd].pop();
				delete dp;
				if(mfds[fd].empty())
					removeWfds(fd);
			}
		}
	}

	//当在子类中要进行socket写时应该调用这个函数
	//可以自动从异步IO中移除无效的描述符
	int tcp_write(int fd, const void *buf, int dataLen)
	{
		int nwrite = send(fd, buf, dataLen, 0);
		if(nwrite<=0)
		{
			printf("Write fail! error-->%s\n",strerror(errno));
			handle_disconnect(fd);
			return -1;
		}
		removeWfds(fd);
		return nwrite;
	}
	void tcp_asyn_write(int fd, const void *buf, int dataLen)
	{
		if(mfds.find(fd) == mfds.end())
			mfds[fd] =	queue<DataPacket*>();
		mfds[fd].push(new DataPacket(buf, dataLen));
		addWfds(fd);
	}
	//当在子类中要进行socket读时应该调用这个函数
	//可以自动从异步IO中移除无效的描述符
	int tcp_read(int fd, void *buf, int dataLen)
	{
		int nread = recv(fd, buf, dataLen, 0);
		if(nread<=0)
		{
			printf("read fail! error-->%s\n",strerror(errno));
			handle_disconnect(fd);
			return -1;
		}
		return nread;
	}

	//如果在客户端断开时需要执行其他操作就需要重写这个函数
	virtual void tcp_disconnect(int fd)
	{
	}
	~TcpServer()
	{
		close(ser_fd);
	}
private:
	void handle_disconnect(int fd)
	{
		removeRfds(fd);
		removeWfds(fd);
		if(mfds.find(fd) != mfds.end())
		{
			while(!mfds[fd].empty())
			{
				delete mfds[fd].front();
				mfds[fd].pop();
			}
			mfds.erase(fd);
        }
		close(fd);
		tcp_disconnect(fd);
	}
private:
	int ser_fd;
	struct sockaddr_in ser_addr;
	int ser_port;
	class DataPacket
	{
	public:
		DataPacket(const void *buf, int size)
		{
			data = new char[size];
			memcpy(data, buf, size);
			dataLen = size;
		}
		~DataPacket()
		{
			delete data;
		}
		void depress(int len)
		{
			memmove(data, (char*)data+len, dataLen - len);
			dataLen -= len;
		}
		int dataLen;
		char *data;

	};
	map<int, queue<DataPacket*> >  mfds;

};

/*异步UDP 服务器
 */
class UdpServer:public AsynSelect
{
public:
	UdpServer(int port, bool block = true, int sec = 0, int usec = 0):AsynSelect(block, sec, usec)
	{
		//1.创建监听socket, 加入到父类rfd中
		ser_fd = socket(AF_INET, SOCK_DGRAM, 0);
		if(ser_fd < 0)
		{
			printf("UdpServer: get socket failed\n");
			exit(-1);
		}
		//2.地址 端口设置
		ser_addr.sin_family = AF_INET;
		ser_port = port;
		ser_addr.sin_port = htons(port);
		ser_addr.sin_addr.s_addr = INADDR_ANY;
		bzero(ser_addr.sin_zero, 8);
		//3.设置地址复用
		int opt=1;
		socklen_t optlen=sizeof(opt);
		setsockopt(ser_fd, SOL_SOCKET, SO_REUSEADDR, &opt, optlen);
		//4.绑定地址端口和socket
		if(bind(ser_fd, (struct sockaddr*)&ser_addr, sizeof(struct sockaddr))==-1)
		{
			printf("Bind failed! errno-->:%s\n", strerror(errno));
			exit(-1);
		}

		addRfds(ser_fd);
		printf("Udp Server started! listen on port %d.\n",ser_port);
	}

	virtual void handle_read(int fd)
	{
		printf("Handle read !\n");
		char buf[256];
		struct sockaddr_in cli_addr={0};
		socklen_t slen=sizeof(struct sockaddr);
		int nread = recvfrom(fd, buf, 256, 0, (struct sockaddr*)&cli_addr, &slen);
		if(nread <= 0)
		{
			removeRfds(fd);
			return;
		}
		buf[nread] = 0;
		printf("read %d bytes: %s\n", nread, buf);
		int nsend = sendto(fd, buf, strlen(buf), 0, (struct sockaddr*)&cli_addr, slen);
		printf("send %d bytes: %s\n", nsend, buf);
	}

	virtual void handle_write(int fd)
	{

	}
	~UdpServer()
	{
		close(ser_fd);
	}
private:
	int ser_fd;
	struct sockaddr_in ser_addr;
	int ser_port;
};


/*基于TCP的远程shell
 */
class CmdShell:public TcpServer
{
public:
	CmdShell(int port):TcpServer(port)
	{
	}
	virtual void handle_accept()
	{
		int fd = tcp_accept();
	}
	virtual void handle_read_data(int fd)
	{
		char msg[512];
		int msg_len = tcp_read(fd, msg, 512);
	}
	virtual void tcp_disconnect(int fd)
	{
	}
	private:
};
//TcpServer ts(4321, false, 10);
//UdpServer us(4321);
CmdShell cs(4321);
int main()
{
//	ts.run();
//	us.run();
//	cs.run();
	/**/
 	FILE *fp = popen("ps aux","r");
	char buf[1024];
	int fd = dup(fileno(fp));
	fclose(fp);
	//fscanf(fp,"%s",buf);
	int nread = read(fd, buf, 1024);
	buf[nread] = 0;
	printf("%s\n", buf);
	execlp("ls","ls", "-al", NULL);
	return 0;
}

