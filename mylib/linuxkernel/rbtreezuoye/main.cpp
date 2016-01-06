#include "rbtree.h"
#include <cstdlib>
#include <cstdio>
#include <ctime>

int main()
{
	RBTree rbt;
	printf("请输入若干个数(ctrl+D结束):\n");
	int v;
	RBTree bt;
	while(scanf("%d", &v)==1 && bt.rb_insert(v)==0)
		;
	PrintVisitor pv;
	SumVisitor sv;
	bt.travel(pv);
	bt.travel(sv);

	printf("sum:%d\n", sv.get_sum());
	return 0;
}
