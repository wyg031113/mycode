#include "rbtree.h"
#include <assert.h>
#include <cstdlib>
void RBTree::build_test_tree()
{
	int n = 8;
	static Node nds[8];
	root = &nds[1];
	for(int i = 1; i < n; i++)
	{
		nds[i].color = BLACK;
		if(i==1)
			nds[i].parent = NIL;
		else
			nds[i].parent = &nds[i/2];
		if(i*2 < n)
			nds[i].left = &nds[i*2];
		else 
			nds[i].left = NIL;
		if(i*2+1 < n)
			nds[i].right = &nds[i*2+1];
		else 
			nds[i].right = NIL;
	}
	nds[1].value = 10;
	nds[2].value = 6;

	nds[3].value = 12;
	nds[4].value = 4;

	nds[5].value = 8;
	nds[6].value = 11;
	nds[7].value = 13;
	nds[7].color = RED;
}


void RBTree::travel()
{
	travel(root);
	printf("NIL addr:%x, COLOR=%s\n", NIL, NIL->color==BLACK?"BLACK":"RED");
}

void RBTree::travel(Node *root)
{
	if(root != NIL)
	{
		travel(root->left);
		printf("addr:%x value:%d color:%s parent:%x left:%x right%x\n",
			root, root->value, root->color==BLACK?"BLACK":"RED", root->parent, root->left, root->right);
		travel(root->right);
	}
}

RBTree::Node *RBTree::rb_minimum(Node *nd)
{
	if(nd == NIL)
		return NIL;
	while(nd->left != NIL)
		nd = nd->left;
	return nd;
}

RBTree::Node *RBTree::rb_maximum(Node *nd)
{
	if(nd == NIL)
		return NIL;
	while(nd->right != NIL)
		nd = nd->right;
	return nd;
}

RBTree::Node *RBTree::rb_successor(Node *nd)
{
	if(nd == NIL)
		return NIL;

	if(nd->right!=NIL)
		return rb_minimum(nd->right);

	while(nd->parent!=NIL && nd!=nd->parent->left)
		nd = nd->parent;
	return nd->parent;
}

RBTree::Node *RBTree::rb_presuccessor(Node *nd)
{
	if(nd == NIL)
		return NIL;

	if(nd->left != NIL)
		return rb_maximum(nd->left);
	while(nd->parent!=NIL && nd!=nd->parent->right)
		nd = nd->parent;
	return nd->parent;
}

void RBTree::test()
{
	Node *nd = root->right->right,*p;
	p = rb_successor(nd);
	if(p == NIL)
		printf("No successor\n");
	else
		printf("%d's successor:%d\n", nd->value, p->value);
	printf("Test left rotate\n");
	left_rotate(root);
	travel();
	printf("\n");

	printf("Test right rotate\n");
	right_rotate(root);
	travel();
	printf("\n");

}


void RBTree::left_rotate(Node *nd)
{
	assert(nd && nd!=NIL && nd->right!=NIL);
	Node *pnd = nd->parent;
	Node *rnd = nd->right;
	Node *rlnd = nd->right->left;
	if(nd == root)
	{
		root = rnd;
		root->parent = NIL;
	}
	else
	{
		if(nd == pnd->left)
			pnd->left = rnd;
		else
			pnd->right = rnd;
		rnd->parent = pnd;
	}
	rnd->left = nd;
	nd->parent = rnd;

	nd->right = rlnd;
	if(rlnd != NIL)
		rlnd->parent = nd;
}

void RBTree::right_rotate(Node *nd)
{
	assert(nd && nd!=NIL && nd->left!=NIL);
	Node *pnd = nd->parent;
	Node *lnd = nd->left;
	Node *lrnd = nd->left->right;

	if(root == nd)
	{
		root = lnd;
		root->parent = NIL;
	}
	else
	{
		if(nd == pnd->left)
			pnd->left = lnd;
		else
			pnd->right = lnd;
		lnd->parent = pnd;
	}

	lnd->right = nd;
	nd->parent = lnd;

	nd->left = lrnd;
	if(lrnd != NIL)
		lrnd->parent = nd;
}

int RBTree::rb_insert(int v)
{
	return rb_insert(new Node(RED, v));
}
int RBTree::rb_insert(Node *nd)
{
	Node *x, *y;
	y = NIL;
	x = root;

	while(x != NIL)
	{
		y = x;
		if(nd->value < x->value)
			x = x->left;
		else
			x = x->right;
	}
	
	nd->parent = y;
	if(y == NIL)
		root = nd;
	else if(nd->value < y->value)
			y->left = nd;
		 else
		 	y->right = nd;

	nd->left = NIL;
	nd->right = NIL;
	nd->color = RED;
	rb_insert_fixup(nd);
	return 0;
}

void RBTree::rb_insert_fixup(Node *nd)
{
	Node *y;
	while(nd->parent->color == RED)
	{
		if(nd->parent == nd->parent->parent->left)
		{
			y = nd->parent->parent->right;
			if(y->color == RED)//右叔叔是RED
			{
				nd->parent->color = BLACK;
				y->color = BLACK;
				nd->parent->parent->color = RED;
				nd = nd->parent->parent;
			}
			else //右叔叔是BLANK
			{
				if(nd == nd->parent->right)
				{
					nd = nd->parent;
					left_rotate(nd);
				}
				nd->parent->color = BLACK;
				nd->parent->parent->color = RED;
				right_rotate(nd->parent->parent);
			}
		}
		else
		{
			y = nd->parent->parent->left;
			if(y->color == RED)//左叔叔是RED
			{
				y->color = BLACK;
				nd->parent->color = BLACK;
				nd->parent->parent->color = RED;
				nd = nd->parent->parent;
			}
			else			  //左叔叔是BLANK
			{
				if(nd == nd->parent->left)
				{
					nd = nd->parent;
					right_rotate(nd);
				}

				nd->parent->color = BLACK;
				nd->parent->parent->color = RED;
				left_rotate(nd->parent->parent);
			}
		}
	}
	root->color = BLACK;
}

void RBTree::check()
{
	bool b1 = (root->color == BLACK);
	if(!b1)
	{
		printf("error:根不是黑色!\n");
		exit(-1);
	}
	int bh = travel_check(root);

}
void RBTree::print_tree()
{
	print_tree(root, 0);
}
void RBTree::print_tree(Node *root, int deep)
{
	if(NIL != root)
	{
		for(int i = 0; i < deep; i++)
			printf("-");
		printf("%d %s\n", root->value, root->color==RED?"RED":"BLACK");
		print_tree(root->left, deep+4);
		print_tree(root->right, deep+4);
	}
}
int RBTree::travel_check(Node *root)
{
	if(root!=NIL)
	{
		if(root->color!=RED && root->color!=BLACK)
		{
			printf("error:节点%d的颜色既不是黑色也不是红色！\n");
			exit(-1);
		}
		if(root->color==RED)
		{
			if(root->left->color==RED || root->right->color == RED)
			{
				printf("error:节点%d的颜色是红色，但两个儿子不都是黑色！\n", root->value);
				exit(-1);
			}
		}
		int bhl = travel_check(root->left);
		int bhr = travel_check(root->right);
		if(bhl != bhr)
		{
			printf("error:节点%d左子树黑高度%d,右子树黑高度%d\n",
					root->value, bhl, bhr);
			exit(-1);
		}
		if(root->color == BLACK)
			return bhl+1;
		else
			return bhl;
	}
	return 1;
}

RBTree::Node *RBTree::rb_delete(Node *nd)
{
	Node *x, *y = NIL;
	if(nd == NIL || nd == NULL)
		return NULL;
	if(nd->left == NIL || nd->right == NIL)
		y = nd;
	else
		y = rb_successor(nd);

	if(y->left != NIL)
		x = y->left;
	else
		x = y->right;

	x->parent = y->parent;
	if(y->parent == NIL)
		root = x;
	else if(y == y->parent->left)
		y->parent->left = x;
	else
		y->parent->right = x;

	if(y != nd)
		nd->value = y->value;

	if(y->color == BLACK)
		rb_delete_fixup(x);
	return y;
}

void RBTree::rb_delete_fixup(Node *nd)
{
	Node *w;
	while(nd != root && nd->color == BLACK)
	{
		if(nd == nd->parent->left)//nd是左孩子
		{
			w = nd->parent->right;
			if(w->color == RED)	//nd的右兄弟w是红色
			{
				nd->parent->color = RED;
				w->color = BLACK;
				left_rotate(nd->parent);
				w = nd->parent->right;
			}

			if(w->left->color == BLACK && w->right->color == BLACK)//nd的右兄弟w的两个孩子都是黑色
			{
				w->color = RED;
				nd = nd->parent;
			}
			else
			{
				if(w->right->color == BLACK)						//nd的右兄弟的右孩子是黑色
				{
					w->left->color = BLACK;
					w->color = RED;
					right_rotate(w);
					w = nd->parent->right;
				}
																	//nd的右兄弟的右孩子是红色
				w->color = nd->parent->color;
				nd->parent->color = BLACK;
				w->right->color = BLACK;
				left_rotate(nd->parent);
				nd = root;
			}
		}
		else
		{
			w = nd->parent->left;
			if(w->color == RED)
			{
				nd->parent->color = RED;
				w->color = BLACK;
				right_rotate(nd->parent);
				w = nd->parent->left;
			}
			
			if(w->left->color == BLACK && w->right->color == BLACK)
			{
				w->color = RED;
				nd = nd->parent;
			}
			else
			{
				if(w->left->color == BLACK)
				{
					w->right->color = BLACK;
					w->color =RED;
					left_rotate(w);
					w = nd->parent->left;
				}
				
				w->color = nd->parent->color;
				nd->parent->color = BLACK;
				w->left->color = BLACK;
				right_rotate(nd->parent);
				nd = root;
			}
		}
	}

	nd->color = BLACK;
}

RBTree::Node *RBTree::rb_search(int v)
{
	Node *p = root;
	while(p != NIL)
	{
		if(v == p->value)
			break;
		if(v < p->value)
			p = p->left;
		else
			p = p->right;
	}
	if(p == NIL)
		return NULL;
	return p;
}

void RBTree::rb_destory(Node *root)
{
	if(root != NIL)
	{
		rb_destory(root->left);
		rb_destory(root->right);
		delete root;
	}
}

RBTree::Node *RBTree::rb_delete(int v)
{
	return rb_delete(rb_search(v));
}
