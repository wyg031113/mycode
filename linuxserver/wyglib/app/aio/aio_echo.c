/*************************************************************************
    > File Name: aio_echo.c
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: 2016年05月16日 星期一 19时14分37秒
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <aio.h>
#include <debug.h>
#include <tcp_server.h>
#define AIO_SIZE 1024

void free_aiocb(struct aiocb *acb);
struct aiocb *alloc_aiocb(int fd);
void aio_cb_write(sigval_t sigval);
typedef void (*aio_cb_func_t)(sigval_t sigval);
void init_aiocb(struct aiocb *acb, int fd, int n, aio_cb_func_t aio_func);
void aio_cb_read(sigval_t sigval)
{
	struct aiocb *acb = (struct aiocb*)sigval.sival_ptr;
	DEBUG_MSG("callback here\n");
	if(aio_error(acb) != 0)
	{
		DEBUG_MSG("call back failed!\n");
		close(acb->aio_fildes);
		free_aiocb(acb);
		return;
	}
	int ret = aio_return(acb);
	if(ret <= 0)
	{
		DEBUG_MSG("call back failed!\n");
		close(acb->aio_fildes);
		free_aiocb(acb);
		return;	
	}
	printf("aio return:%d value=%s\n", ret, acb->aio_buf);
	//sleep(1);
	
	//init_aiocb(acb, acb->aio_fildes, AIO_SIZE, aio_cb_read);
	acb->aio_nbytes = ret;
	acb->aio_sigevent.sigev_notify_function = aio_cb_write;
	CHECK2(aio_write(acb) == 0);
	CHECK2(aio_read(alloc_aiocb(acb->aio_fildes)) == 0);
	
}

void aio_cb_write(sigval_t sigval)
{
	struct aiocb *acb = (struct aiocb*)sigval.sival_ptr;
	DEBUG_MSG("callback here\n");
	if(aio_error(acb) != 0)
	{
		DEBUG_MSG("call back failed!\n");
	}
	free_aiocb(acb);
}

void init_aiocb(struct aiocb *acb, int fd, int n, aio_cb_func_t aio_func)
{
	acb->aio_fildes = fd;
	acb->aio_offset = 0;
	acb->aio_nbytes = n;
	acb->aio_sigevent.sigev_notify = SIGEV_THREAD;
	acb->aio_sigevent.sigev_notify_function = aio_func;

	acb->aio_sigevent.sigev_value.sival_ptr = acb;
	bzero((void*)acb->aio_buf, AIO_SIZE);

}
struct aiocb *alloc_aiocb(int fd)
{
	struct aiocb *acb = (struct aiocb*)malloc(sizeof(struct aiocb));
	if(acb == NULL)
		return NULL;

	bzero((char*)acb, sizeof(struct aiocb));

	acb->aio_buf = malloc(AIO_SIZE);
	if(acb->aio_buf == NULL)
	{
		free(acb);
		return NULL;
	}
	init_aiocb(acb, fd, AIO_SIZE, aio_cb_read);	
	
	return acb;
}


void free_aiocb(struct aiocb *acb)
{
	free((void*)acb->aio_buf);
	free(acb);
}
int main()
{
	int serfd = tcp_server("0.0.0.0", 54321);
	int clifd = -1;
	char tmp[36];
	//int fd = open("test.txt", O_RDONLY);
	//if(fd<0) printf("Open failed\n"), exit(-1);
	struct aiocb * acb;
	while(1)
	{
		errno = 0;
		CHECK(clifd = accept(serfd, NULL, NULL));
		//read(clifd, tmp, 36);
		CHECK2(aio_read(acb = alloc_aiocb(clifd)) == 0);
	//	sleep(1);
	//	printf("Aio error:%d\n", aio_error(acb));
	//	int ret = aio_return(acb);
	//	printf("aio return:%d\n %s errno=%d\n", ret, acb->aio_buf, errno);
	//	write(clifd, "hello\n", 6);
	//	close(clifd);
	}
	close(serfd);
	return 0;
}
