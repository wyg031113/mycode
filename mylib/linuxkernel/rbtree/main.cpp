#include "rbtree.h"
#include <cstdlib>
#include <cstdio>
#include <ctime>

void test1()
{
	RBTree rbt;
	printf("请输入若干个数(ctrl+D结束):\n");
	int v;
	RBTree bt;
	while(scanf("%d", &v)==1 && bt.rb_insert(v)==0)
		;
	bt.travel();
	printf("Travel complete!\n");
	bt.check();
}
void test3()
{
	RBTree rbt;
	for(int i = 0; i < 20; i++)
	{
		int t= rand();
//		printf("%d\n", t);
		rbt.rb_insert(t);
	}
	printf("insert complete\n");
	//rbt.travel();
	rbt.check();
	//rbt.print_tree();
}

void test_search()
{
	printf("Test search\n");
	RBTree rbt;
	int n = 2000;
	int vs[2000];
	int i;
	for(int i = 0; i < n; i++)
	{
		int t= rand();
		vs[i] = t;
//		printf("%d\n", t);
		rbt.rb_insert(t);
	}
	printf("insert complete\n");
	//rbt.travel();
	rbt.check();
//	vs[n-1]=99999;
	for(int i = 0; i < n; i++)
		if(rbt.rb_search(vs[i]) == NULL)
		{
			printf("Search %d failed!!\n", vs[i]);
			break;
		}
	rbt.check();
	//rbt.print_tree();
}
void test_delete()
{
	printf("Test delete\n");
	RBTree rbt;
	int n = 5000;
	static int vs[100000];
	int i;
	for(int i = 0; i < n; i++)
	{
		int t= rand()%n;
		vs[i] = t;
//		printf("%d\n", t);
		rbt.rb_insert(t);
	}
	printf("insert complete\n");
	//rbt.travel();
	rbt.check();
	//vs[n-1]=99999;
	RBTree::Node *x, *y;
	//shuff
	for(int i = 0; i <n; i++)
	{
		int pos = (rand()%(n-i))+i;
		int t = vs[i];
		vs[i] = vs[pos];
		vs[pos] = t;
	}
	for(int i = 0; i < n; i++)
	{
		x = rbt.rb_search(vs[i]);
		if(x == NULL)
		{
			printf("Search %d failed!!\n", vs[i]);
			break;
		}
		
		x = rbt.rb_delete(vs[i]);
		y = rbt.rb_search(vs[i]);
		if(y != NULL)
		{
			int k;
			for(k = i+1; k < n; k++)
				if(vs[k] == vs[i])
					break;
			if(k ==n)
			{
				printf("i=%d:delete %d failed!!\n", i, vs[i]);
				break;
			}
		}
		delete x;
		rbt.check();
	}
	rbt.check();
	//rbt.print_tree();
}
void test2()
{
	RBTree rbt;
	rbt.build_test_tree();
	rbt.travel();
	printf("\n");
	rbt.test();
	rbt.check();
}
int main()
{
	//srand(time(0));
	srand(4);
	for(int i = 0; i < 10;i++)
	{
		printf("Runn test %d...\n", i+1);
		//test1();
		//test2();
		test3();
		test_search();
		test_delete();
	}
	return 0;
}
