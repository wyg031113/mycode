#include <inc.h>
#define MSG_LEN 16
struct mymsg
{
	long type;
	char msg[MSG_LEN];
};
int main(int argc, char **argv)
{
	int flags;
	int mqd;
	flags = O_RDWR;
	unsigned int prio;
	if(argc != 4)
		err_quit("Usage:./mqsendmsg mqname msg priority");
	prio = atoi(argv[3]);
	CHECK(mqd = msgget(ftok(argv[1], 1), flags));
	struct mymsg msg;
	msg.type = prio;
	strncpy(msg.msg, argv[2], MSG_LEN);
	CHECK(msgsnd(mqd, &msg, strlen(msg.msg),0));
	return 0;
}
