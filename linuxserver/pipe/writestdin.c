#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main()
{
	char *str = "Hello world\n";
	write(0, str, strlen(str));
	char buf[123];
	int ret = read(1, buf, 123);
	if(ret>0)
	{
		buf[ret] = 0;
		printf("read someting:%s\n", buf);
	}

}
