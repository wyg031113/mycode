#include <timer_wheel.h>
#include <pollio.h>
#include <tcp_server.h>
#include <debug.h>
#include <util.h>

void handle_client(struct Event e);
void say_hello(struct Timer *timer);
int listenfd;
int pipefd[2];
struct ClientData
{
	int fd;
	struct Timer *timer;
};
struct ClientData mcd[1024];
void handle_accept(struct Event e)
{
	if(e.event == POLL_READ)
	{
		struct sockaddr_in sa={0};
		socklen_t len = sizeof(sa);
		int connfd;
		CHECK(connfd = accept(listenfd, (struct sockaddr*)&sa, &len));
		DEBUGMSG("Accept a client[%s]:%d\n", inet_ntoa(sa.sin_addr), ntohs(sa.sin_port));
		set_handle(connfd, handle_client);
		submit_task(connfd, POLL_READ);

		struct Timer *timer = (struct Timer *)malloc(sizeof(struct Timer));
		timer->msec = 5000;
		timer->cb = say_hello;
		timer->data.d = connfd;
		mcd[connfd].timer = timer;
		mcd[connfd].fd = connfd;
		add_timer(timer);

	}
	else
		exit(-1);
}

void handle_client(struct Event e)
{
	DEBUGMSG("Handle Client\n");
	if(e.event == POLL_READ)
	{
		char buf[1024];
		int ret = recv(e.fd, buf, 1024, 0);
		if(ret <=0 )
		{
			del_timer(mcd[e.fd].timer);
			cancel_task(e.fd, POLL_READ);
			cancel_task(e.fd, POLL_WRITE);
			close(e.fd);
			DEBUGMSG("Client:%d closed!\n", e.fd);
			return;
		}
		adjust_timer(mcd[e.fd].timer);
		buf[ret] = '\0';
		printf("client:%d said:%s\n", e.fd, buf);
	}
}
void handle_pipe(struct Event e)
{
	DEBUGMSG("handle pipe\n");
	if(e.event == POLL_READ)
	{
		int fd;
		int ret = read(e.fd, &fd, 4);
		if(ret <=0)
		{
			cancel_task(e.fd, POLL_READ);
			cancel_task(e.fd, POLL_WRITE);
			return;
		}
		char buf[128];
		sprintf(buf, "Mr. %d, please say something!\n", fd);
		send(fd, buf, strlen(buf), 0);
	
	}
	else
		exit(-1);
}

void say_hello(struct Timer *timer)
{
	printf("Say: ");
	int fd = timer->data.d;
	int ret = write(pipefd[1], &fd, sizeof(fd));
	printf("write pipe:%d bytes, fd=%d\n", ret, fd);
	add_timer(timer);
}
int main()
{
	CHECK(listenfd = tcp_server(NULL, 12345));
	CHECK(pipe(pipefd));

	start_timer();

	set_handle(pipefd[0], handle_pipe);
	submit_task(pipefd[0], POLL_READ);
	//write(pipefd[1], "1234", 4);
	set_handle(listenfd, handle_accept);
	submit_task(listenfd, POLL_READ);
	run();
	return 0;
}
