#include <inc.h>

int main(int argc, char **argv)
{
	int flags;
	mqd_t mqd;
	flags = O_RDWR;
	unsigned int prio;
	if(argc != 4)
		err_quit("Usage:./mqsendmsg mqname msg priority");
	prio = atoi(argv[3]);
	CHECK(mqd = mq_open(argv[1], flags));
	CHECK(mq_send(mqd, argv[2], strlen(argv[2]), prio));
	mq_close(mqd);
	return 0;
}
