#include<stdio.h>
#include<stdlib.h>
#include<time.h>
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
	srand(time(0));
//	int key = rand() % 25 + 1;
	int key = 24;
		for(j = 0; buf[j]; j++)
		{
			if(buf[j] >= 'A' && buf[j] <='Z')
			{
				buf[j] += key;
				if(buf[j] > 'Z')
					buf[j] = buf[j] - 'Z' + 'A' -1;
			}
			if(buf[j] >= 'a' && buf[j] <='z')
			{
				buf[j] += key;
				if(buf[j] > 'z')
					buf[j] = buf[j] - 'z' + 'a' - 1;
			}
		}
		printf("key = %d: %s\n", key, buf);
}
