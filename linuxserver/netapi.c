#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main()
{
	printf("%x\n", htonl(0x12345678));
	printf("%x\n", ntohl(0x12345678));
	printf("%x\n", htons(0x1234));
	printf("%x\n", ntohs(0x1234));

	in_addr_t ia;
	struct in_addr in;
	ia = inet_addr("192.168.1.1");
	int ret=inet_aton("192.168.1.1", &in);
	printf("IP:%x\n", ia);
	printf("IP:%x ret=%d\n", in, ret);
	printf("IP:%s\n", inet_ntoa(in));
	return 0;
}
