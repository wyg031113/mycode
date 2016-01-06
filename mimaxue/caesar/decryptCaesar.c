#include<stdio.h>
#include<string.h>
int main(int argc, char *argv[])
{
	if(argc < 2)
	{
		printf("Please input something.\n");	
		return -1;
	}
	unsigned char buf[128];
	int i, j;
	strcpy(buf, argv[1]);
	for(i = 1; i < 26; i++)
	{
		strcpy(buf, argv[1]);
		for(j = 0; buf[j]; j++)
		{
			if(buf[j] >= 'A' && buf[j] <='Z')
			{
				buf[j] += i;
				if(buf[j] > 'Z')
					buf[j] = buf[j] - 'Z' + 'A' -1;
			}
			if(buf[j] >= 'a' && buf[j] <='z')
			{
				buf[j] += i;
				if(buf[j] > 'z')
					buf[j] = buf[j] - 'z' + 'a' - 1;
			}
		}
		printf("key = %d: %s\n", i, buf);
	}
}
