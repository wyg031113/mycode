#ifndef __TCP_H__
#define __TCP_H__

#include "types.h"
#include "ip.h"
#include <string.h>
#define TCP_HEAD_LEN 20
#define TCP_HEAD_LEN_WORDS 5
struct TCP_PACKET
{
	ushort src_port;
	ushort dst_port;

	uint   seq;

	uint   ack;
	
	ubyte reserve1:4;
	ubyte   offset:4;

	uint   FIN:1;
	uint   SYN:1;
	uint   RST:1;
	uint   PSH:1;
	uint   ACK:1;
	uint   URG:1;
	ubyte reserve2:2;
	ushort window;

	ushort checksum;
	ushort urg_ptr;
	
	void *operator new(size_t size, void *p)
	{
		return p;
	}
	TCP_PACKET()
	{
		memset(this, 0, sizeof(TCP_PACKET));
		this->offset = TCP_HEAD_LEN_WORDS;
	}
	void calc_checksum(IP_PACKET* ip, ushort real_len)
	{

		this->checksum = 0;	
		ForgedHead fh = {0};
		fh.src_ip = ip->src;
		fh.dst_ip = ip->dst;
		fh.protocol = 6;
		fh.len		= htons(real_len);
		ubyte *fhb = (ubyte*)&fh;
		ubyte *hdb = (ubyte*)this;
		uint sum = 0;
		int i;
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
