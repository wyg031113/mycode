#include <stdio.h>
#include <unistd.h>
#include <sys/sem.h>

int main()
{
	int fd = semget('b', 1, IPC_CREAT|0660);
	struct sembuf sb = {0, -1, 0};
	struct sembuf sb2 = {0, 1, 0};
	semop(fd, &sb, 1);
	close(fd);

	int sm = shmget('x', 1024, IPC_CREAT|0660);
	char *share = (char*)shmat(sm, NULL, 0);
	int i;
	for(i = 0; i < 5; i++)
	{
		semop(fd, &sb, 1);
		printf("%s", share);
	}
	close(sm);
}
