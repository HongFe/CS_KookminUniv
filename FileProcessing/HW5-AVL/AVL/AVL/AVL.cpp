#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#define TRUE 1
#define FALSE 0

using std::ofstream;
using std::ifstream;
using namespace std;

// Element ����ü�� ��忡 
// �ԷµǴ� ���ڸ� ���� �� �ֵ��� ���ش�
struct Element {
	char key;
	Element(char k) { key = k; };
	Element(){};
};

// forward declaration
template <class T> class BST;

// ---------- TreeNode class -------------
template <class T>
class TreeNode
{
	friend class BST<T>;

public:
	TreeNode(T d);

private:
	T data;			//T will be Element
	TreeNode<T> *leftChild;		// link to the left Child
	TreeNode<T> *rightChild;	// link to the right Child
};

template <class T>
TreeNode<T>::TreeNode(T d)
{
	// data is given
	data = d;
	// the leftChild and rightChild field are NULL
	leftChild = rightChild = NULL;
}

// ---------- BST class -------------
template <class T>
class BST
{
public:
	BST();	// ������
	bool Insert(T keyval);	// Ʈ���� ��带 �߰��ϴ� �Լ���.
	void Inorder(TreeNode<T> *ptr);	// Ʈ���� Inorder ������� �д´�.
	void Show();	// Ʈ���� �����ش�. 
	int  Height(TreeNode<T> *p, int cnt); // Ʈ���� ���� ���� ���̸� �����Ѵ�

	// NON-AVL�� AVL�� ������ش�
	bool AVL(TreeNode<T> *pp, TreeNode<T> *p, TreeNode<T> *newnode);
	// �ش� node(p)�Ʒ��� ã�³��(n)�� ���� ����Ȯ��
	bool findT(TreeNode<T> *p, TreeNode<T> *n);
private:
	TreeNode<T>* root; // pointer to the root node
	int h = 0;	// ���� ���� ���̸� ���� ����
};


template<class T>
BST<T>::BST()
{ // BST constructor. Initialize root variable
	root = NULL; // root is NULL, an empty binary search tree
}

template <class T>
void  BST<T>::Show()
{
	Inorder(root);
	cout << endl;
}

// Ʈ���� Inorder ������� ǥ���Ѵ�.
template <class T>
void  BST<T>::Inorder(TreeNode<T> *ptr)
{
	int rh, lh;
	if (ptr){
		Inorder(ptr->leftChild);
		h = 0;
		rh = Height(ptr->rightChild, 0);	// ������ Ʈ�� ����
		h = 0;
		lh = Height(ptr->leftChild, 0);   // ���� Ʈ�� ����
		cout << ptr->data.key << "(" << lh - rh << ") "; // BF���
		Inorder(ptr->rightChild);
	}
}

// Ʈ���� ��带 �����Ѵ�.
template <class T>
bool  BST<T>::Insert(T keyval)
{
	TreeNode<T> *p = root, *pp = 0;

	// �����Ϸ��� ����� key ��ġ�� ����
	while (p){
		pp = p;
		// ���ο� key�� ���� key���� �۴ٸ� left child �˻�
		if (keyval.key < p->data.key)
			p = p->leftChild;
		// ���ο� key�� ���� key���� ũ�ٸ� right child �˻�
		else if (keyval.key > p->data.key)
			p = p->rightChild;
		// ���ο� key�� ���� key�� ���ٸ� �̹� �����ϹǷ� ����!
		else if (keyval.key == p->data.key){
			cout << "ERROR : already in tree" << endl;
			return false;
		}
	}
	p = new TreeNode<T>(keyval);	// ���ο� ��� ����

	// root�� ���� �ҽ�
	if (root){
		// �θ����� key���� �۴ٸ� left,
		// �θ����� key���� ũ�ٸ� right
		if (keyval.key< pp->data.key)
			pp->leftChild = p;
		else
			pp->rightChild = p;
	}
	// root�� �������� �ʴ´ٸ� ���ο� ��尡 root
	else
		root = p;

	// Ʈ���� ���� ���
	cout << "Height of tree : " << Height(root, 0) << endl;
	AVL(NULL, root, p);	// ���� ���Ե� ��� p�� 
	// ���ԵǾ� �ִ� ����Ʈ���� �˻��� �� ���
	return true;
}

// ---------- AVL ���� �Լ� -------------
template <class T>
bool BST<T> ::AVL(TreeNode<T> *pp, TreeNode<T> *p, TreeNode<T> *newnode){

	TreeNode<T> *tp = 0, *ttp = 0;
	int rh, lh;

	if (!p){
		return false;	// p�� �������� �ʴ´ٸ� ���� �� �ʿ� ���� 
	}
	AVL(p, p->rightChild, newnode);
	AVL(p, p->leftChild, newnode);

	h = 0;
	rh = Height(p->rightChild, 0);	// ������ Ʈ�� ����
	h = 0;
	lh = Height(p->leftChild, 0);   // ���� Ʈ�� ����


	// BF ���� �Ҿ���
	if ((lh - rh) >= 2){
		// L- �϶�
		if (AVL(p, p->leftChild, newnode) == true){
			tp = p->leftChild;
			// LL �϶�
			if (findT(tp->leftChild, newnode)){
				cout << "[" << p->data.key << "(" << (lh - rh) << ")"
					<< "���� LL ȸ�� ����]" << endl;
				p->leftChild = tp->rightChild;
				tp->rightChild = p;

				if (p == root)
					root = tp;
				else{
					if (pp->leftChild == p)
						pp->leftChild = tp;
					else if (pp->rightChild == p)
						pp->rightChild = tp;
				}
			}

			// LR �϶�
			else if (findT(tp->rightChild, newnode)){
				cout << "[" << p->data.key << "(" << (lh - rh) << ")"
					<< "���� LR ȸ�� ����]" << endl;
				ttp = tp->rightChild;
				p->leftChild = ttp->rightChild;
				tp->rightChild = ttp->leftChild;
				ttp->rightChild = p;
				ttp->leftChild = tp;
				if (p == root)
					root = ttp;
				else{
					if (pp->leftChild == p)
						pp->leftChild = ttp;
					else if (pp->rightChild == p)
						pp->rightChild = ttp;
				}

			}
		}
	}

	// BF ���� �Ҿ��� 
	else if ((lh - rh) <= -2){
		// R- �϶�
		if (AVL(p, p->rightChild, newnode) == true){
			tp = p->rightChild;
			//RR�϶�
			if (findT(tp->rightChild, newnode)){
				cout << "[" << p->data.key << "(" << (lh - rh) << ")"
					<< "���� RR ȸ�� ����]" << endl;

				p->rightChild = tp->leftChild;
				tp->leftChild = p;
				if (p == root)
					root = tp;
				else{
					if (pp->leftChild == p)
						pp->leftChild = tp;
					else if (pp->rightChild == p)
						pp->rightChild = tp;
				}
			}

			//RL�϶�
			else if (findT(tp->leftChild, newnode)){
				cout << "[" << p->data.key << "(" << (lh - rh) << ")"
					<< "���� RL ȸ�� ����]" << endl;
				ttp = tp->leftChild;
				p->rightChild = ttp->leftChild;
				tp->leftChild = ttp->rightChild;
				ttp->leftChild = p;
				ttp->rightChild = tp;

				if (p == root)
					root = ttp;
				else{
					if (pp->leftChild == p)
						pp->leftChild = ttp;
					else if (pp->rightChild == p)
						pp->rightChild = ttp;
				}
			}
		}
	}
	else
		return true;	// BF���� �������̴�

	return false;		// BF�� +2�̻� �Ǵ� -2���ϸ� false BF�� �Ҿ���
}
// -------------------------------------------------

// ��� �ؿ� �����Էµ� ��尡 �ִ��� �˻��Ѵ�
template <class T>
bool BST<T> ::findT(TreeNode<T> *p, TreeNode<T> *n){
	while (p){
		// ���ο� key�� ���� key���� �۴ٸ� left child �˻�
		if (n->data.key < p->data.key)
			p = p->leftChild;
		// ���ο� key�� ���� key���� ũ�ٸ� right child �˻�
		else if (n->data.key > p->data.key)
			p = p->rightChild;
		// ���ο� key�� ���� key�� ���ٸ� �̹� �����ϹǷ� ����!
		else if (p->data.key == n->data.key)
			return true;
	}
	return false;
}
// ���̸� ���ϴ� �Լ���. �� �Լ��� Ʈ���� �ϴ����� ������������ 1�� ������
// child�� ���� �ܸ� ��忡�� ���������� ������ ���� ���Ͽ� �ְ� ����(h)�� ���Ѵ�.
template <class T>
int  BST<T> ::Height(TreeNode<T> *p, int cnt){

	// recursive�ϰ� ȣ���Ͽ� ���̸� ���Ѵ�
	if (p){
		cnt++;
		Height(p->leftChild, cnt);
		Height(p->rightChild, cnt);
	}

	// �ܸ���忡 �������� ��� ���̸� �ְ� ���̿� ���Ͽ� ���Ѵ�.
	else{
		if (h < cnt)
			h = cnt;
	}
	return h;
}