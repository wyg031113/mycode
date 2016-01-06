#include<stdio.h>
int main()
{
	char a = 'e';
	unsigned int c;
	for(c = 0; c <= 255; c++)
		printf("%c ", a ^ c);
	printf("\n");
	return 0;
}

