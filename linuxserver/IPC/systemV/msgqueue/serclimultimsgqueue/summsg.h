#ifndef __SUMMSG_H__
#define __SUMMSG_H__

#include<unistd.h>
#define SERTYPE 1
struct SumReq
{
	long type;
	int climqd;
	int x;
	int y;
};

struct SumRep
{
	long type;
	int sum;
};
#endif /*__SUMMSG_H__*/
