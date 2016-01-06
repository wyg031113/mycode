#ifndef __IP_H__
#define __IP_H__

#include "types.h"
#define IP_IP4_VERSION 4
#define IP_HEAD_LEN_WORDS 5  //5个4字节0101
#define IP_DIFF_SER 0
#define IP_FLAG 0x2
#define IP_TTLS  64

#define IP_PROTO_UDP 17
#define IP_PROTO_TCP 6
#define IP_PROTO_ICMP 1
#define IP_HEAD_LEN 20
#include <arpa/inet.h>
#include <string.h>
struct IP_PACKET
{
	ushort head_len:4;
	ushort version:4;
	ubyte  diff_ser;
	ushort tot_len;

	ushort ident;
	ushort flag_offset;
	ubyte  ttl;
	ubyte  protocol;
	ushort checksum;

	uint   src;
	uint   dst;
	
	IP_PACKET()
	{
		memset(this, 0, sizeof(IP_PACKET));
		this->version	 = IP_IP4_VERSION; 
		this->head_len 	 = IP_HEAD_LEN_WORDS;
		this->ttl 		 = IP_TTLS;
	}
	void *operator new(size_t size, void *p)
	{
		return p;
	}

};

struct ForgedHead
{
	uint src_ip;
	uint dst_ip;
	ubyte zero;
	ubyte protocol;
	ushort len;
};
#endif
