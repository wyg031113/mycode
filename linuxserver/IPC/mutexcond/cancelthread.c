#include <inc.h>
pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;
pthread_t tid1, tid2;
void *thread1(void *), *thread2(void *);

void thread_clean(void *arg)
{
	printf("Thread cleaning...\n");
}
int main()
{
	void *status;
	pthread_create(&tid1, NULL, thread1, NULL);
	sleep(1);
	pthread_create(&tid2, NULL, thread2, NULL);

	pthread_join(tid2, &status);
	if(status != PTHREAD_CANCELED)
		printf("Thread 2 status = %p\n", status);
	//printf("rw_refcount = %d, rw_nwaitreaders = %d", rwlock.rw_refcount, rwlock.rw_nwaitreaders);
	pthread_join(tid1, &status);
	printf("Thread 1 status = %p\n", status);
	return 0;	
}

void *thread1(void *arg)
{
	pthread_cleanup_push(thread_clean, NULL);
	pthread_exit(NULL);
	pthread_cleanup_pop(0);
	pthread_rwlock_rdlock(&rwlock);
	printf("Thread1() got a read lock\n");
	sleep(3);
	pthread_cancel(tid2);
	sleep(3);
	pthread_rwlock_unlock(&rwlock);
	return NULL;
}

void *thread2(void *arg)
{
	printf("Thread2() trying to obtain a write lock\n");
	pthread_rwlock_wrlock(&rwlock);
	printf("Thread2() got a write lock\n");
	sleep(1);
	pthread_rwlock_unlock(&rwlock);
	return NULL;
}
