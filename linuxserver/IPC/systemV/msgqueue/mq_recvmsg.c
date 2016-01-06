#include <inc.h>
#define MSG_SIZE 16
struct mymsg
{
	long type;
	char msg[MSG_SIZE];
};
int main(int argc, char **argv)
{
	int flags = 0;
	int mqd = 0;
	flags = O_RDWR;
	if(argc != 3)
		err_quit("Usage:./mqrecvmsg mqname priority");
	CHECK(mqd = msgget(ftok(argv[1],1), flags));
	int type = atoi(argv[2]);
	int ret;
	struct mymsg msg;
	CHECK(ret=msgrcv(mqd, &msg, MSG_SIZE, type, 0));
	msg.msg[ret] = 0;
	printf("recv:%s  type:%d\n", msg.msg, msg.type);
	return 0;
}
