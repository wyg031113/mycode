#include <stdio.h>
#include <malloc.h>

struct Node
{
	int value;
	struct Node *left;
	struct Node *right;
};

/**
 * 插入节点，保持二叉查找树。
 * root: 根节点指针
 * v:    要插入的数
 */
int bst_insert(struct Node **root, int v)
{
	struct Node *nd = (struct Node*)malloc(sizeof(struct Node));
	nd->left = nd->right = NULL;
	nd->value = v;

	if(nd == NULL)
		return -1;

	if(*root == NULL)
		*root = nd;
	else
	{
		struct Node *p = *root;
		struct Node *q = NULL;
		while(p)
		{
			q = p;
			if(v < p->value)
				p = p->left;
			else
				p = p->right;
		}
		if(v < q->value)
			q->left = nd;
		else
			q->right = nd;
	}
	return 0;
}

typedef void (*node_op_t)(int);

void travel(struct Node *root, node_op_t op)
{
	if(root != NULL)
	{
		travel(root->left, op);
		op(root->value);
		travel(root->right, op);
	}
}

void op_print(int value)
{
	printf("%d ", value);
}

int sum;
void op_sum(int value)
{
	sum += value;
}

int main()
{
	printf("请输入若干个数(ctrl+D结束):\n");
	int v;
	struct Node *root = NULL;
	while(scanf("%d", &v)==1)
	{
		bst_insert(&root, v);
	}

	printf("中序遍历结果:\n");
	travel(root, op_print);
	printf("\n");

	travel(root, op_sum);
	printf("求和结果: %d\n", sum);
	return 0;
}
