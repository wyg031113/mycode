#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<string.h>
#include<iostream>
#include<netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<signal.h>
using namespace std;
int main(int argc, char *argv[])
{
	printf("This is client...\n");
	signal(SIGPIPE, SIG_IGN);
	if(argc < 3)
	{
		printf("Usage: ./testClient ser_ip port\n");
		return -1;
	}
	int sd = socket(AF_INET, SOCK_STREAM, 0);
	if(sd < 0)
	{
		cerr<<"get sd failed\n"<<endl;
		return -1;
	}

	struct sockaddr_in ser_addr;
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_port = htons(atoi(argv[2]));
	ser_addr.sin_addr.s_addr = inet_addr(argv[1]);
	bzero(ser_addr.sin_zero, 8);
	if(connect(sd, (struct sockaddr*)&ser_addr, sizeof(struct sockaddr))<0)
	{
		printf("Connect failed!\n");
		close(sd);
		return -1;
	}
	printf("连接服务器成功\n");
	char buf[256];
	//system("pause");
	struct timeval tv={3,0};
	setsockopt(sd, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(tv));

	int ts = 20;
/*	while(ts--){
	printf("请按任意键开始接收数据\n");
	getchar();
	int nrecv = recv(sd, buf, 256,0);
	buf[nrecv] = '\0';
	printf("nrecv = %d  data = %s\n", nrecv, buf);
	}
	strcpy(buf,"原来汉字也可以!");
	while(1){	
	printf("请按任意键开始发送数据\n");
	getchar();
*/
	struct timeval tv2={3,0};
	setsockopt(sd, SOL_SOCKET, SO_SNDTIMEO, (char*)&tv2, sizeof(tv2));
	int nsend  = send(sd, buf, strlen(buf), 0);

	//printf("nsend = %s\n", nsend, buf);
//	if(nsend<0)
//		break;
	//sleep(3);
	//}
	

	int nread = read(sd, buf, 255);
	buf[nread] = '\0';
	printf("nread = %d\n  buf =%s\n", nread, buf);	
	strcpy(buf,"Last write");
	write(sd, buf, strlen(buf));
	printf("请按回车结束程序\n");
	getchar();
	close(sd);
	return 0;
}
