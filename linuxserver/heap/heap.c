#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX_SIZE 1024
int heap[MAX_SIZE];
int cnt;
void swap(int *a, int *b)
{
	*a ^= *b;
	*b = *b ^ *a;
	*a = *a ^ *b;
}
void heap_adjust(int i)
{
	while(i<=cnt)
	{
		int L = i << 1;
		int R = L +1;
		int tar = i;
		if(L<=cnt && heap[L] > heap[tar])
			tar = L;
		if(R<=cnt && heap[R] > heap[tar])
			tar = R;
		if(tar == i)
			break;

		swap(&heap[i], &heap[tar]);
		i = tar;
	}
}
int heap_remove_root()
{
	int t = heap[1];
	heap[1] = heap[cnt];
	cnt--;
	heap_adjust(1);
	return t;
}
void show()
{
	int i;
	for(i = 1; i <= cnt; i++)
		printf("%d ", heap[i]);
	printf("\n");
	int prev=0x7fffffff;
	int cur;
	for(;cnt>0;)
	{
		cur = heap_remove_root();
		printf("%d ", cur);
		if(cur > prev)
		{
			printf("Error!!\n");
			break;
		}
		prev = cur;

	}
	printf("\n");
}
void heap_insert(int k)
{
	if(cnt+1>=MAX_SIZE)
		return;
	cnt++;
	heap[cnt] = k;
	int i = cnt;
	int j;
	while(1)
	{
		j = i>>1;
		if(j==0 || heap[j]>=heap[i])
			break;
		swap(&heap[j], &heap[i]);
		i = j;

	}
}
void buid_heap()
{
	int i;
	for(i = cnt/2; i>=1; i--)
		heap_adjust(i);
}
int main()
{
	srand(time(NULL));
	int n =1000;
	cnt = n/2;
	int i;
	for(i = 1; i <= n/2; i++)
		heap[i] = rand()%100;
	buid_heap();
	for(; i <= n; i++)
		heap_insert(rand()%1000);
	show();
}
