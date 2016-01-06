#include <inc.h>

int main(int argc, char **argv)
{
	int flags;
	mqd_t mqd;
	flags = O_RDWR;
	if(argc != 2)
		err_quit("Usage:./mqrecvmsg mqname");
	CHECK(mqd = mq_open(argv[1], flags));
	struct mq_attr ma;
	mq_getattr(mqd, &ma);
	char *buf = (char*)malloc(ma.mq_msgsize);
	int prio;
	CHECK(mq_receive(mqd, buf, ma.mq_msgsize, &prio));
	printf("recv:%s  prio:%d\n", buf, prio);
	free(buf);
	mq_close(mqd);
	return 0;
}
