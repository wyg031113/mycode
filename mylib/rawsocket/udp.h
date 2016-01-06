#ifndef __UDP_H__
#define __UDP_H__

#include "types.h"
#include "ip.h"
#define UDP_HEAD_LEN 8
struct UDP_PACKET
{
	ushort src_port;
	ushort dst_port;

	ushort   len;
	ushort checksum;

	void *operator new(size_t size, void *p)
	{
		return p;
	}

	UDP_PACKET()
	{
		memset(this, 0, sizeof(UDP_PACKET));
	}
	void calc_checksum(IP_PACKET* ip)
	{

		ForgedHead fh = {0};
		fh.src_ip = ip->src;
		fh.dst_ip = ip->dst;
		fh.protocol = 17;
		fh.len		= len;
		ubyte *fhb = (ubyte*)&fh;
		ubyte *hdb = (ubyte*)this;
		int real_len = ntohs(len);
		uint sum = 0;
		int i;

		this->checksum = 0;
		for(i = 0; i+1 < 12; i+=2)
			sum += (fhb[i]<<8) + fhb[i+1];
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
