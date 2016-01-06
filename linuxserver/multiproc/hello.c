#include <stdio.h>
int main()
{
	printf("This is a test!\n");
	char buf[1024];
	int ret = read(3, buf, 1024);
	if(ret > 0 )
		printf("%s\n", buf);
	return 0;
}
