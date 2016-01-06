#ifndef __RBTREE_H__
#define __RBTREE_H__
#include <cstdio>
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


class RBTree
{
public:
	enum Color{RED, BLACK};
	struct Node
	{
		Node *parent;
		Node *left;
		Node *right;
		Color color;
		int value;
		Node(Color c = RED, int v = 0):parent(NULL), left(NULL), right(NULL), value(v), color(c){}
		~Node(){}

	};
	RBTree(){ NIL = new Node(BLACK); root = NIL; }
	~RBTree()
	{
		rb_destory(root);
		delete NIL;
	}
	Node *rb_successor(Node *nd);
	Node *rb_presuccessor(Node *nd);
	Node *rb_minimum(Node *nd);
	Node *rb_maximum(Node *nd);
	Node *rb_search(int v);
	int rb_insert(int v);
	int rb_insert(Node *nd);
	Node *rb_delete(int v);
	Node *rb_delete(Node *nd);
	void travel(Visitor &vis);
	void travel(Node *root, Visitor &vis);
private:
	void left_rotate(Node *nd);
	void right_rotate(Node *nd);
	void rb_insert_fixup(Node *nd);
	void rb_delete_fixup(Node *nd);
	void rb_destory(Node *root);
	Node *NIL;
	Node *root;
};
#endif /*__RETREE_H__*/
