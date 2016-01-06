#ifndef __ICMP_H__
#define __ICMP_H__
#include "types.h"
#define ICMP_TYPE_REQUEST 8
#define ICMP_TYPE_REPLY   0
#include <string.h>
#include <netinet/in.h>
struct ICMP_PACKET
{
	ubyte type;
	ubyte code;
	ushort checksum;
	ushort id;
	ushort sequence;
	uint   timestamp;
	uint   timestamp2;
	ICMP_PACKET()
	{
		memset(this, 0, sizeof(ICMP_PACKET));
	}
	void *operator new(size_t size, void *p)
	{
		return p;
	}
	void calc_checksum()
	{

		
		ubyte *hdb = (ubyte*)this;
		int real_len = sizeof(ICMP_PACKET);
		uint sum = 0;
		int i;

		this->checksum = 0;
		
		for(i = 0; i+1 < real_len; i+=2)
			sum += (hdb[i]<<8) + hdb[i+1];
		if(real_len%2==1)
			sum += hdb[real_len-1]<<8;
		
		while(sum>>16)
			sum = (sum>>16) + (sum&0xFFFF);

		checksum = ~((ushort)sum);
		checksum = htons(checksum);
	}
};
#endif
