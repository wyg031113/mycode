#include <stdio.h>
#include <malloc.h>

struct Node
{
	int value;
	Node *left;
	Node *right;

	Node():left(NULL), right(NULL), value(0){}
	Node(Node *L, Node *R, int v):left(L), right(R), value(v){}
};

/**
 * visitor接口。
 */
class Visitor
{
public:
	virtual void visit(int value) = 0;
	virtual ~Visitor(){}
};

/**
 * 打印的实现。
 */
class PrintVisitor: public Visitor
{
public:
	void visit(int value)
	{
		printf("%d ", value);
	}
};

/**
 * 求和的实现。
 */
class SumVisitor: public Visitor
{
public:
	SumVisitor()
	{
		sum = 0;
	}
	void visit(int value)
	{
		sum += value;
	}
	int get_sum()
	{
		return sum;
	}
private:
	int sum;
};

/**
 * 二叉查找树。
 */
class BSTTree
{
public:
	BSTTree()
	{
		root = NULL;
	}
	/**
	* 插入节点，保持二叉查找树。
 	* root: 根节点指针
 	* v:    要插入的数
 	*/
	int bst_insert(int v)
	{
		Node *nd = new Node(NULL, NULL, v);

		if(nd == NULL)
			return -1;

		if(root == NULL)
			root = nd;
		else
		{
			Node *p = root;
			Node *q = NULL;
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
	
	void travel(Visitor &vst)
	{
		travel(root, vst);
	}
	void travel(Node *root, Visitor &vst)
	{
		if(root != NULL)
		{
			travel(root->left, vst);
			vst.visit(root->value);
			travel(root->right, vst);
		}
	}
private:
	Node *root;
};

int main()
{
	printf("请输入若干个数(ctrl+D结束):\n");
	int v;
	BSTTree bt;
	while(scanf("%d", &v)==1 && bt.bst_insert(v)==0)
		;
	
	PrintVisitor pv;
	printf("中序遍历结果:\n");
	bt.travel(pv);
	printf("\n");

	SumVisitor sv;
	bt.travel(sv);
	printf("求和结果: %d\n", sv.get_sum());

	return 0;
}
