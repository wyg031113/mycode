#include "types.h"
#include "icmp.h"
#include "ip.h"

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

void ping(char *dst_ip, long intval, int times)
{
	ubyte buf[1024];
	IP_PACKET *ip = new(buf) IP_PACKET;
	ICMP_PACKET *icmp = new(buf+IP_HEAD_LEN)ICMP_PACKET;
	icmp->type = ICMP_TYPE_REQUEST;
	icmp->sequence = htons(1);
	icmp->id = htons((u_short)getpid());
	icmp->timestamp2 = 1234567;
	icmp->calc_checksum();
	int s;
	CHECK(s = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP));

	int one = 1;
	const int *val = &one;
	setsockopt(s, IPPROTO_IP, IP_HDRINCL, val, sizeof(one));

	struct sockaddr_in dst_addr;
	dst_addr.sin_family = AF_INET;
	dst_addr.sin_port = htons(0);
	dst_addr.sin_addr.s_addr = inet_addr(dst_ip);
	bzero(&dst_addr.sin_zero, 8);

	int ret;
	ip->protocol = IP_PROTO_ICMP;
	ip->dst = inet_addr(dst_ip);
	ip->src = inet_addr("127.0.0.1");

	int i = 0;
	ushort k = 0;
	while(times==0 || i++ < times)
	{

		k++;
		icmp->sequence = htons(k);
		icmp->calc_checksum();
		CHECK(ret=sendto(s, buf, IP_HEAD_LEN+sizeof(ICMP_PACKET), 0, (struct sockaddr*)&dst_addr, sizeof(struct sockaddr)));	
		printf("send:%d\n", k);
		usleep(intval*1000);
	}
	printf("ret=%d\n", ret);

	close(s);
}


int main(int argc, char *argv[])
{
	if(argc < 4)
	{
		printf("Usage:./ping  dst_ip intval count\n");
		exit(0);
	}
	ping(argv[1],atoi(argv[2]), atoi(argv[3]));
	return 0;
}

