#include <inc.h>
typedef unsigned long int ulong_t;
int main(int argc, char **argv)
{
	int c, flags;
	int mqd;
	flags = 0660 | IPC_CREAT;
	while( (c=getopt(argc, argv, "e")) !=-1)
	{
		switch(c)
		{
			case 'e':
				flags |= IPC_EXCL;
				break;
				
		}
	}
	
	if(optind != argc -1)
		err_quit("Usage mqcreate [-e]  <name>");	
	CHECK(mqd = msgget(ftok(argv[optind], 1), flags));	
	struct msqid_ds info;
	CHECK(msgctl(mqd, IPC_STAT, &info));
	printf("read-write: %03o, cbytes = %lu, qnum = %lu, qbytes = %lu\n",
			info.msg_perm.mode & 0777, (ulong_t) info.msg_cbytes, 
			(ulong_t) info.msg_qnum, (ulong_t)info.msg_qbytes);
	system("ipcs -q");
	return 0;
}
