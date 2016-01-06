#include<inc.h>
#define MAXNITEMS 1000000
#define MAXNTHREADS 100
int nitems;
struct {
	pthread_mutex_t mutex;
	int buff[MAXNITEMS];
	int nput;
	int nval;
}shared = {PTHREAD_MUTEX_INITIALIZER};

void *producer(void *), *consumer(void *);

int main(int argc, char *argv[])
{
	int i, nthreads, count[MAXNTHREADS];
	pthread_t tid_producer[MAXNTHREADS], tid_consume;
	if(argc != 3)
		err_quit("Usage:procon <#Items> <$threads>");
	nitems = min(atoi(argv[1]), MAXNITEMS);
	nthreads = min(atoi(argv[2]), MAXNTHREADS);

	for(i = 0; i < nthreads; i++)
	{
		count[i] = 0;
		pthread_create(&tid_producer[i], NULL, producer, &count[i]);
	}

	for(i = 0; i < nthreads; i++)
	{
		pthread_join(tid_producer[i], NULL);
		printf("Thread %d: count:%d\n", i, count[i]);
	}

	pthread_create(&tid_consume, NULL, consumer, NULL);
	pthread_join(tid_consume, NULL);
	return 0;
}

void *producer(void *arg)
{
	printf("Producer.\n");
	int *p = (int*)arg;

	while(1)
	{
		pthread_mutex_lock(&shared.mutex);
		if(shared.nput >= nitems)
		{
			pthread_mutex_unlock(&shared.mutex);
			return NULL;
		}
		shared.buff[shared.nput++] = shared.nval++;
		pthread_mutex_unlock(&shared.mutex);
		++*p;
	}
}

void *consumer(void *arg)
{
	int i;
	for(i = 0; i < nitems; i++)
		if(i != shared.buff[i])
			printf("buff[%d] = %d\n", i, shared.buff[i]);
	printf("Consumer.\n");
}

