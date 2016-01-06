#include <stdio.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/shm.h>

int main()
{
	int fd = semget('b', 1, IPC_CREAT|0660);
	struct sembuf sb = {0, 1, 0};
	struct sembuf sb2 = {0, -1, 0};
	
	semop(fd, &sb, 1);
	int sm = shmget('x', 1024, IPC_CREAT|0660);
	char *share = (char*)shmat(sm, NULL, 0);
	int i;
	for(i = 0; i < 5; i++)
	{
		sprintf(share, "Hello,Mr. %d, use sharem memory!\n", i);
		semop(fd, &sb, 1);
		sleep(2);
	}
	sleep(10);
	shmctl(sm, IPC_RMID, NULL);
	close(sm);
	semctl(fd, 0,IPC_RMID, 0);
	close(fd);
}
