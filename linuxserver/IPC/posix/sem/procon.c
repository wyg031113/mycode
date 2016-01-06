#include <inc.h>
#define NBUF 10
#define SEM_MUTEX "mutex"
#define SEM_NEMPTY "nempty"
#define SEM_NSTORED "nstored"
struct
{
	int buf[NBUF];
	sem_t *mutex, *nempty, *nstored;
}shared;
void *consume(void *arg)
{
	unsigned int i = 0;
	while(1)
	{
		sem_wait(shared.nstored);
		sem_wait(shared.mutex);
		printf("Consumer\n");
		if(i != shared.buf[i%NBUF])
			printf("Error:shared.buf[%d] == %d\n", i, shared.buf[i]);
		sem_post(shared.mutex);
		sem_post(shared.nempty);
		i++;
	//	printf("consume i=%d\n", i);
		sleep(2);
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
	while(1)
	{
		sem_wait(shared.nempty);
		sem_wait(shared.mutex);
		printf("Producer\n");
		shared.buf[i%NBUF] = i;
		sem_post(shared.mutex);
		sem_post(shared.nstored);
	//	printf("i=%d\n", i);
		
	//	printf("produce:shared.buf[%d] == %d\n", i, shared.buf[i%NBUF]);
		i++;
		sleep(1);
	}
	return NULL;

}

int main()
{	
	pthread_t tid_produce, tid_consume;
	CHECK((shared.mutex=sem_open(SEM_MUTEX, O_RDWR|O_CREAT, 0666, 1))!=SEM_FAILED);	
	CHECK((shared.nempty=sem_open(SEM_NEMPTY, O_RDWR|O_CREAT, 0666, NBUF))!=SEM_FAILED);	
	CHECK((shared.nstored=sem_open(SEM_NSTORED, O_RDWR|O_CREAT, 0666, 0))!=SEM_FAILED);	
	pthread_create(&tid_produce, NULL, produce, NULL);
	pthread_create(&tid_consume, NULL, consume, NULL);
	signal(SIGINT, ctrlc);	
	pthread_join(tid_produce, NULL);
	pthread_join(tid_consume, NULL);
	sem_unlink(SEM_MUTEX);
	sem_unlink(SEM_NEMPTY);
	sem_unlink(SEM_NSTORED);
}
