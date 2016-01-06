#ifndef __RBTREE_H__
#define __RBTREE_H__
#include <cstdio>
/**
 * 红黑树，实现了前趋 后继 查找 最大 最小 中序遍历 插入 删除 销毁 红黑树判断(check) 左旋 右旋 打印树。
 * 问题1.存储数据只能是int类型。 可以实现一个基类，或者模板，使得可以存放任意类型。
 * 问题2.销毁时，直接删除来节点，对节点中的数据应该由用户定义删除方式。
 * 问题3.申请、释放内存速度很慢，需要优化，对节点应该做缓存，可以重载new delete,或者在RBTree中缓存。
 * 问题4.实现foreach visitor模式，对节点进行访问。
 *
 */
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
		Node(Node *P, Node *L, Node *R, Color cl, int v):parent(P), left(L), right(R), color(cl), value(v){}
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
	void build_test_tree();
	void travel();
	void travel(Node *root);
	void test();
	void check();//判断是不是红黑树
	void print_tree();
private:
	void left_rotate(Node *nd);
	void right_rotate(Node *nd);
	void rb_insert_fixup(Node *nd);
	void rb_delete_fixup(Node *nd);
	int travel_check(Node *root);
	void rb_destory(Node *root);
	void print_tree(Node *root, int deep);
	Node *NIL;
	Node *root;
};
#endif /*__RETREE_H__*/
