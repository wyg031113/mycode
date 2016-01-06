/*************************************************************************
    > File Name: thread_pool.cpp
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: 2015年12月23日 星期三 16时07分13秒
 ************************************************************************/
#include "thread_pool.h"
ThreadPool::ThreadPool()
{

}
ThreadPool* ThreadPool::thread_pool_create(string ip, int port, int nthread)
{
	if(nthread <= 0)
		throw "thread num must > 0!"
	this->ip = ip;
	this->port = port;
	this->maxthread = nthread;

}
