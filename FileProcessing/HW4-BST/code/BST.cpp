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

// Element����ü�� ��忡 
// �ԷµǴ� ���ڸ� ���� �� �ֵ��� ���ش�
struct Element {
	char *key = new char[4];
	Element(char *k) {
		for (int i = 0; i < 4; i++){
			key[i] = k[i];
		}
	};
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
	bool  Insert(T keyval);	// Ʈ���� ��带 �߰��ϴ� �Լ���.
	bool  Delete(T keyval); // Ʈ���� ��带 �����ϴ� �Լ���.
	void  Inorder(TreeNode<T> *ptr);	// Ʈ���� Inorder ������� �д´�.
	void  Show();	// Ʈ���� �����ش�. 
	int  Height(TreeNode<T> *p, int cnt); // Ʈ���� ���� ���� ���̸� �����Ѵ�
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
	if (ptr){
		Inorder(ptr->leftChild);
		cout << ptr->data.key << " ";
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
		if (strcmp(keyval.key, p->data.key)<0)		// ���ο� key�� ���� key���� �۴ٸ� left child �˻�
			p = p->leftChild;
		else if (strcmp(keyval.key, p->data.key)>0)	// ���ο� key�� ���� key���� ũ�ٸ� right child �˻�
			p = p->rightChild;
		else if (strcmp(keyval.key, p->data.key) == 0){	// ���ο� key�� ���� key�� ���ٸ� �̹� �����ϹǷ� ����!
			cout << "ERROR : already in tree" << endl;
			return false;
		}
	}
	p = new TreeNode<T>(keyval);	// ���ο� ��� ����

	// root�� ���� �ҽ�
	if (root){
		// �θ����� key���� �۴ٸ� left,
		// �θ����� key���� ũ�ٸ� right
		if (strcmp(keyval.key, pp->data.key) < 0)
			pp->leftChild = p;
		else
			pp->rightChild = p;
	}
	// root�� �������� �ʴ´ٸ� ���ο� ��尡 root
	else
		root = p;

	return true;
}

// �Էµ� ��带 Ʈ������ ����
template <class T>
bool BST<T>::Delete(T keyval)
{
	bool ret = true;
	TreeNode<T> *p = root, *pp = 0;
	TreeNode<T> *tp = 0, *tpp = 0;	//tp �� 2node delete���� �˻��� ���� parent���
	int rh, lh;

	if (root == NULL)
		return false;

	//���� p�� ���� �� �Ѵٸ�
	if (!p)
		return false;

	pp = p;
	while (p){
		// p���� ���� �μ� ���� ��� ���ʿ��� ã�´�
		if (strcmp(keyval.key, p->data.key)<0){
			pp = p;
			p = p->leftChild;
		}
		// p���� ū �μ� ���� ��� �����ʿ��� ã�´�
		else if (strcmp(keyval.key, p->data.key)>0){
			pp = p;
			p = p->rightChild;
		}
		//ã�� node �߽߰�
		else if (strcmp(keyval.key, p->data.key) == 0)
			break;	
	}
	// ã�� key�� �������� �ʴ´ٸ�
	if(!p) {
		cout << "ERROR : Node Not Exist" << endl;
		return false;
	}

	// �����Ǵ� ����� ������ ���̰� ���� ���� ����Ʈ���� ���Ѵ�.
	h = 0;
	rh = Height(p->rightChild, 0);	// ������ Ʈ�� ����
	h = 0;
	lh = Height(p->leftChild, 0);   // ���� Ʈ�� ����

	tpp = pp;

	// ���� ������� ��尡 �ܸ�����϶�
	if (!p->leftChild && !p->rightChild){
		// ������� ��尡 �θ����� left child�϶� �θ���� NULL�� ����Ų��
		if (pp->leftChild == p){
			pp->leftChild = NULL;
		}
		// ������� ��尡 �θ����� left child�϶� �θ���� NULL�� ����Ų��.
		else if (pp->rightChild == p){
			pp->rightChild = NULL;
		}
	}
	// ���� ������� ��尡 �ܸ���尡 �ƴҶ�
	else{
		// �������� ���̰� ������ ������ ����Ʈ���� ���� ���� ���� ��ü�� ������ ����	
		if (rh > lh){
			tp = p->rightChild;
			// ����Ʈ���� ���� ���� ���� ���Ѵ�
			while (tp->leftChild != NULL){
				tpp = tp;
				tp = tp->leftChild;
			}

			// ������ ��� p �� �θ� �����Ҷ� ��ü�� ��带 ����Ų��.
			if (pp->leftChild == p){
				pp->leftChild = tp;
			}
			else if (pp->rightChild == p){
				pp->rightChild = tp;
			}

			// ���� p�� child�� �ٷ� ��ü�� ���̶��
			if (p->rightChild == tp){
				if (p->leftChild)
				{
					tp->leftChild = p->leftChild;
				}
			}
			else{
				//���� ���� ������(��ü�� ��)�� right child�� �����ϸ� ��ü�Ǵ� ���� �θ��� left child�� �ȴ�
				if (tp->leftChild){
					tpp->leftChild = tp->rightChild;
				}
				else
					tpp->leftChild = NULL;

				// ������ ���� left child�� �����ϸ�
				if (p->leftChild){
					tp->leftChild = p->leftChild;
				}
				// ������ ���� right child�� �����ϸ�
				if (p->rightChild){
					tp->rightChild = p->rightChild;

				}
			}
		}
		// ������ ���̰� ������ ���� ����Ʈ���� ���� ū ���� ��ü�� ������ ����	
		else{
			tp = p->leftChild;

			// ����Ʈ���� ���� ū ���� ���Ѵ�
			while (tp->rightChild != NULL){
				tpp = tp;
				tp = tp->rightChild;
			}

			// ������ ��� p �� �θ� �����Ҷ� ��ü�� ��带 ����Ų��.
			if (pp->leftChild == p){
				pp->leftChild = tp;

			}
			else if (pp->rightChild == p){
				pp->rightChild = tp;
			}

			// ���� p�� child�� �ٷ� ��ü�� ���̶��
			if (p->leftChild == tp){
				if (p->rightChild)
				{
					tp->rightChild = p->rightChild;
				}
			}
			else{
				//���� ���� ū��(��ü�� ��)�� left child�� �����ϸ� ��ü�Ǵ� ���� �θ��� right child�� �ȴ�
				if (tp->leftChild){
					tpp->rightChild = tp->leftChild;
				}
				else
					tpp->rightChild = NULL;

				// ������ ���� left child�� �����ϸ�
				if (p->leftChild){
					tp->leftChild = p->leftChild;
				}
				// ������ ���� right child�� �����ϸ�
				if (p->rightChild){
					tp->rightChild = p->rightChild;
				}
			}
		}
	}
	// ���� �����Ǵ� ���� root�ϰ�� ��ü�� ���� root�� �缳���Ѵ�.
	if (p == root){
		root = tp;
	}
	delete p;
	return ret;	// return true
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

	// �ܸ���忡 �ٴ޾��� ��� ���̸� �ְ� ���̿� ���Ͽ� ���Ѵ�.
	else{
		if (h < cnt)
			h = cnt;
	}
	return h;
}

