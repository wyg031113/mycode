#include <inc.h>

int main(int argc, char **argv)
{
	int c, flags;
	mqd_t mqd;
	flags = O_RDWR | O_CREAT;
	struct mq_attr attr={0};
	while( (c=getopt(argc, argv, "em:z:")) !=-1)
	{
		switch(c)
		{
			case 'e':
				flags |= O_EXCL;
				break;
			case 'm':
				attr.mq_maxmsg = atol(optarg);
				break;
			case 'z':
				attr.mq_msgsize = atol(optarg);
				break;
				
		}
	}
	
	if(optind != argc -1)
		err_quit("Usage mqcreate [-e] [-m maxmsg -z msgsize] <name>");	
	if((attr.mq_maxmsg != 0 && attr.mq_msgsize == 0) || (attr.mq_maxmsg == 0 && attr.mq_msgsize !=0))
		err_quit("must specify both -m maxmsg and -z msgsize");
	CHECK(mqd = mq_open(argv[optind], flags, 0660, attr.mq_maxmsg != 0?&attr:NULL));

	struct mq_attr ma;
	CHECK(mq_getattr(mqd, &ma));
	printf("mq_flags:%d\nmq_maxmsg:%d\nmq_maxsize:%d\nmq_curmsgs:%d\n", ma.mq_flags, ma.mq_maxmsg, ma.mq_msgsize, ma.mq_curmsgs);
	mq_close(mqd);
	return 0;
}
