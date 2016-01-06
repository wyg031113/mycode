#include<stdio.h>
#include <setjmp.h>
jmp_buf ebuf;
void f2(void);
int main(void)
{
	int i;
	printf("1");
  	i=setjmp(ebuf);
  	if(i==0)
   	{
    	f2();
     	printf("This will not be printed.");
    }
	printf("%d",i);
	return 0;
}
void f2(void)
{
  printf("2");
  longjmp(ebuf,3);
}
