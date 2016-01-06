#include "util.h"

#define SERPORT 8888
#define LISTEN_BACKLOG 5
#define MAX_EVENTS 3

int setnonblocking(int sockfd)
{
	CHECK(fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFD, 0) | O_NONBLOCK));
	return 0;
}
int main()
{
	int serfd, clifd, epollfd;
	int ret, nfds;
	int flag = 1, flag_len = sizeof(int);
	struct sockaddr_in ser_addr, cli_addr;
	socklen_t addr_len;
	struct epoll_event ev, events[MAX_EVENTS];

	ser_addr.sin_family = AF_INET;
	ser_addr.sin_addr.s_addr = INADDR_ANY;
	ser_addr.sin_port = htons(SERPORT);
	bzero(&ser_addr.sin_zero, 8);
		
	CHECK2(serfd, socket(AF_INET, SOCK_STREAM, 0));
	CHECK(setsockopt(serfd, SOL_SOCKET, SO_REUSEADDR, &flag, flag_len));
	CHECK(bind(serfd, (struct sockaddr *)&ser_addr, sizeof(struct sockaddr)));
	CHECK(listen(serfd, LISTEN_BACKLOG));

	CHECK2(epollfd, epoll_create(MAX_EVENTS));	
	ev.events = EPOLLIN;
	ev.data.fd = serfd;
	CHECK(epoll_ctl(epollfd, EPOLL_CTL_ADD, serfd, &ev));	
	cout<<"start epoll..."<<endl;
	printf("Listened on [%s]:%d\n", inet_ntoa(ser_addr.sin_addr), ntohs(ser_addr.sin_port));
	for(;;)
	{
		int i;
		CHECK2(nfds, epoll_wait(epollfd, events, MAX_EVENTS, 10000));
		if(nfds == 0)
			cout<<"Timed out!"<<endl;

		for(i = 0; i < nfds; i++)
		{
			if(events[i].data.fd == serfd)
			{
				addr_len = sizeof(struct sockaddr);
				CHECK2(clifd, accept(serfd, (struct sockaddr *)&cli_addr, &addr_len));
				printf("Accept a client: [%s]:%d\n", inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));
				ev.events = EPOLLIN | EPOLLET;
				ev.data.fd = clifd;
				setnonblocking(clifd);
				if(epoll_ctl(epollfd, EPOLL_CTL_ADD, clifd, &ev) == -1)
				{
					close(clifd);
					continue;
				}
			}
			else
			{
				if(events[i].events & EPOLLIN)
				{
					printf("file:%d can read\n", events[i].data.fd);
					char buf[512];
					int nrec = recv(events[i].data.fd, buf, 10, 0);
					if(nrec <= 0)
					{
						cout<<"recv failed!"<<endl;
						CHECK(epoll_ctl(epollfd, EPOLL_CTL_DEL, events[i].data.fd, NULL));
					}
					else
					{
						buf[nrec] = '\0';
						cout<<buf<<endl;
						events[i].events = EPOLLIN;	
						CHECK(epoll_ctl(epollfd, EPOLL_CTL_MOD, events[i].data.fd, &events[i]));
					}
				}
				else if(events[i].events & EPOLLOUT)
				{
					static	char buf[4097] = "Welcom\n";
					int len = 6;
					int rect;
					while(len)
					{
							printf("running\n");
						rect = write(events[i].data.fd, buf, len);
							printf("after send\n");
						if(rect != len)
							printf("buff full\n");
						if(rect <=0)
						{
							printf("Nonblocking\n");
							printf("errno=%s\n", strerror(errno));
							break;
						
						}
						len -= rect;
						len = 4097;
					}
					if(rect <= 0)
						printf("write failed\n");
					else
						printf("write success\n");
					events[i].events = EPOLLIN;	
					//CHECK(epoll_ctl(epollfd, EPOLL_CTL_MOD, events[i].data.fd, &events[i]));
				}
			}
		}
	}
/*
	for(;;)
	{
		addr_len = sizeof(struct sockaddr);
		CHECK2(clifd, accept(serfd, (struct sockaddr *)&cli_addr, &addr_len));
		send(clifd, "12345", 5, 0);
		close(clifd);
	}
*/
	close(serfd);
	return 0;
}
