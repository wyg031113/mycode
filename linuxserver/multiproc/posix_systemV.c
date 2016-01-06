#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/msg.h>
int main()
{
	int i;
		mqd_t mq = mq_open("/temp.1234", O_CREAT|O_RDWR, 0660, NULL);
		printf("mq=%d\n", mq);
		mq_close(mq);
		mq_unlink("/temp.1234");	
	int msgid;
	for(i = 0; i < 1; i++)
	{	
		//msgid = msgget(ftok("/tmp", 1),IPC_CREAT|0660 );
		printf("msgid=%d\n", msgid);
		msgctl(163840, IPC_RMID, NULL);
	}
	return 0;
}
