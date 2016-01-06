#include<stdio.h>
#include<stdlib.h>
#include<time.h>
int main()
{
	char buf[]="abcdefghijklmnopqrstuvwxyz";
	freopen("key.txt", "w", stdout);
	int i;
	srand(time(0));
	for(i = 0; i < 26; i++)
	{
		int j = rand()%(26 - i) + i;
		int t = buf[j];
		buf[j] = buf[i];
		buf[i] = t;
	}
	printf("%s\n", buf);
	return 0;
}
