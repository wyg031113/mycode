/*************************************************************************
    > File Name: thread_pool.h
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: 2015年12月23日 星期三 16时12分58秒
 ************************************************************************/
#ifndef __THREAD_POOL_H__
#define __THREAD_POOL_H__
#include <string>
using namespace std;

#define MAXTHREAD 20
#define MAXTASK 1000
class ThreadPool
{
private:
	static void* run_child(void *arg);
public:
	static ThreadPool *thread_pool_create(string ip, unsigned short port, int nthread = 8);
private:
	ThreadPool();
	int pipefd[MAXTHREAD][2];
	static ThreadPool *tp;
	string ip;
	unsigned short port;
	int maxthread;
};

class Worker
{
public:
	Worker(int ppfd);
	void run();
private:
	int pipefd;
};
#endif /*__THREAD_POOL_H__ */
