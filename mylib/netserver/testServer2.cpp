#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#define PORT 12345
#include<arpa/inet.h>
#include<errno.h>
using namespace std;
int main()
{
	cout<<"Hello world"<<endl;
	for(int i=0;i<140;i++)
	{
		errno = i;
		cout<<"Errno="<<errno<<"-->"<<strerror(errno)<<endl;
	}
	int ser_sd = socket(AF_INET, SOCK_STREAM, 0);
	if(ser_sd <= 0)
	{
		cerr<<"Get scoket failed!"<<endl;
		return -1;
	}
	printf("Server start...\n");
	struct sockaddr_in ser_addr, cli_addr;
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_port = htons(PORT);
	ser_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&ser_addr.sin_zero, 8);

	int opt=1;
	socklen_t  optlen=sizeof(opt);
	setsockopt(ser_sd, SOL_SOCKET, SO_REUSEADDR, &opt, optlen);
	if(bind(ser_sd, (struct sockaddr*)&ser_addr, sizeof(struct sockaddr)) < 0)
	{
		cerr<<"Bind failed!"<<endl;
		close(ser_sd);
		return -1;
	}

	if(listen(ser_sd, 8)<0)
	{
		cerr<<"listen failed\n"<<endl;
		close(ser_sd);
		return -1;
	}
	socklen_t sock_len = sizeof(struct sockaddr);
	int cli_fd = accept(ser_sd, (struct sockaddr*)&cli_addr, &sock_len);
	char buf[]="Welcom to server......\n";

	printf("请按回车开始发送数据\n");
	getchar();
	int nsend = send(cli_fd, buf, strlen(buf),0); 	
	cout<<"Send "<<nsend<<" bytes"<<endl;
	cout<<inet_ntoa(cli_addr.sin_addr)<<" "<<ntohs(cli_addr.sin_port)<<endl;

	printf("请按回车开始接收数据\n");
	getchar();
	int nrecv = recv(cli_fd, buf, strlen(buf), 0);
	printf("run here... recv=%d\n", nrecv);
	if(nrecv<0)
	{
		printf("recv fialed!retval = %d   errno = %d-->%s\n", nrecv, errno, strerror(errno));
		close(ser_sd);
		close(cli_fd);
		return -1;
	}
	buf[nrecv] = '\0';
	cout<<"Recv "<<nrecv<<" bytes, "<<buf<<endl;
	char la[]="You just say: ";
	int nwrite = write(cli_fd, la, strlen(la)); 
	int nread = read(cli_fd, la, strlen(la));
	la[nread] = '\0';
	printf("nwrite = %d   nread = %d   datais=%s\n",nwrite, nread, la);
	close(ser_sd);
	close(cli_fd);	
	return 0;
}
