#include <stdio.h>
#include <unistd.h>

int main()
{
	FILE* fp = popen("ls -al", "r");

	char buf[1024];
	char *p;
	while((p=fgets(buf, 1024, fp)))
		fputs(buf, stdout);
	pclose(fp);

	return 0;
}
