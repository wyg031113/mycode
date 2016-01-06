#include <inc.h>
#define NBUF 10
#define SEM_MUTEX "mutex"
#define SEM_NEMPTY "nempty"
#define SEM_NSTORED "nstored"
struct
{
	int buf[NBUF];
	int head;
	int tail;
	sem_t *mutex, *nempty, *nstored;
}shared;
void *consume(void *arg)
{
	unsigned int i = 0;
	while(i++<1000)
	{
		sem_wait(shared.nstored);
		sem_wait(shared.mutex);
		printf("Consumer\n");
		if(shared.tail != shared.buf[shared.tail%NBUF])
			printf("Error:shared.buf[%d] == %d\n", shared.tail, shared.buf[shared.tail%NBUF]);
		shared.tail++;
		sem_post(shared.mutex);
		sem_post(shared.nempty);
	//	printf("consume i=%d\n", i);
//		sleep(2);
	}
	return NULL;
}

void ctrlc(int sig)
{
	printf("delete sems\n");
	sem_unlink(SEM_MUTEX);
	sem_unlink(SEM_NEMPTY);
	sem_unlink(SEM_NSTORED);
	exit(0);
}
void *produce(void *arg)
{
	unsigned int i = 0;
	while(i++<1000)
	{
		sem_wait(shared.nempty);
		sem_wait(shared.mutex);
		printf("Producer\n");
		shared.buf[shared.head%NBUF] = shared.head;
		shared.head++;
		sem_post(shared.mutex);
		sem_post(shared.nstored);
	//	printf("i=%d\n", i);
		
	//	printf("produce:shared.buf[%d] == %d\n", i, shared.buf[i%NBUF]);
//		sleep(1);
	}
	return NULL;

}

int main(int argc, char *argv[])
{	
	int i, np, nc;
	pthread_t tid_produce[1024], tid_consume[1024];

	if(argc != 3)
		err_quit("Usage:mulitprocon nproducer nconsumer");
	np = atoi(argv[1]);
	nc = atoi(argv[2]);
	np = min(np, 1024);
	nc = min(nc, 1024);

	CHECK((shared.mutex=sem_open(SEM_MUTEX, O_RDWR|O_CREAT, 0666, 1))!=SEM_FAILED);	
	CHECK((shared.nempty=sem_open(SEM_NEMPTY, O_RDWR|O_CREAT, 0666, NBUF))!=SEM_FAILED);	
	CHECK((shared.nstored=sem_open(SEM_NSTORED, O_RDWR|O_CREAT, 0666, 0))!=SEM_FAILED);	

	for(i = 0; i < np; i++)
		pthread_create(&tid_produce[i], NULL, produce, NULL);
	for(i = 0; i < nc; i++)
		pthread_create(&tid_consume[i], NULL, consume, NULL);

	signal(SIGINT, ctrlc);	

	for(i = 0; i < np; i++)
		pthread_join(tid_produce[i], NULL);
	for(i = 0; i < nc; i++)
		pthread_join(tid_consume[i], NULL);

	sem_unlink(SEM_MUTEX);
	sem_unlink(SEM_NEMPTY);
	sem_unlink(SEM_NSTORED);
	printf("head=%d tail=%d\n", shared.head, shared.tail);
	exit(0);
}
