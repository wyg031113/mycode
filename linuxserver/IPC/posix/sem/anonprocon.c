#include <inc.h>
#define NBUF 10
#define SEM_MUTEX "mutex"
#define SEM_NEMPTY "nempty"
#define SEM_NSTORED "nstored"
struct
{
	int buf[NBUF];
	sem_t mutex, nempty, nstored;
}shared;
void *consume(void *arg)
{
	unsigned int i = 0;
	while(1)
	{
		sem_wait(&shared.nstored);
		sem_wait(&shared.mutex);
		printf("Consumer\n");
		if(i != shared.buf[i%NBUF])
			printf("Error:shared.buf[%d] == %d\n", i, shared.buf[i]);
		sem_post(&shared.mutex);
		sem_post(&shared.nempty);
		i++;
	//	printf("consume i=%d\n", i);
		sleep(2);
	}
	return NULL;
}

void ctrlc(int sig)
{
	printf("delete sems\n");
	sem_destroy(&shared.mutex);
	sem_destroy(&shared.nempty);
	sem_destroy(&shared.nstored);
	exit(0);
}
void *produce(void *arg)
{
	unsigned int i = 0;
	while(1)
	{
		sem_wait(&shared.nempty);
		sem_wait(&shared.mutex);
		printf("Producer\n");
		shared.buf[i%NBUF] = i;
		sem_post(&shared.mutex);
		sem_post(&shared.nstored);
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
	CHECK(sem_init(&shared.mutex, 0, 1));
	CHECK(sem_init(&shared.nempty, 0, NBUF));
	CHECK(sem_init(&shared.nstored, 0,0 ));
	pthread_create(&tid_produce, NULL, produce, NULL);
	pthread_create(&tid_consume, NULL, consume, NULL);
	signal(SIGINT, ctrlc);	
	pthread_join(tid_produce, NULL);
	pthread_join(tid_consume, NULL);
	ctrlc(0);
}
