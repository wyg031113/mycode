#include <inc.h>
struct Data
{
	int a;
	int b;
};
int main(int argc, char *argv[])
{
	int fd;
	int offset;
	int len;
	if(argc != 3)
		err_quit("Usage recordlock offset len");
	offset = atoi(argv[1]);
	len = atoi(argv[2]);
	CHECK(fd = open("test.dat", O_CREAT|O_RDWR, 0666));
	struct Data d={1,1};
	struct flock lock={F_WRLCK, SEEK_SET, offset, len}, lockinfo={0,SEEK_SET, 0, 4};
	CHECK(fcntl(fd, F_GETLK, &lockinfo));
	printf("lock:%d pid:%ld\n", lockinfo.l_type, lockinfo.l_pid);
	CHECK(fcntl(fd, F_SETLKW, &lock));
	printf("Set lock success\n");
	sleep(5);
	CHECK(read(fd, &d, sizeof(d)));
	printf("a=%d\n", d.a);
	d.a++;
	int ret;
	lseek(fd, 0, SEEK_SET);
	CHECK(ret=write(fd, &d, sizeof(d)));
	printf("write lock success\n");
	lock.l_type = F_UNLCK;
	CHECK(fcntl(fd, F_SETLKW, &lock));	
	close(fd);
	return 0;
}
