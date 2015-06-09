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

// Element 구조체는 노드에 
// 입력되는 문자를 담을 수 있도록 해준다
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
	BST();	// 생성자
	bool Insert(T keyval);	// 트리에 노드를 추가하는 함수다.
	void Inorder(TreeNode<T> *ptr);	// 트리를 Inorder 방식으로 읽는다.
	void Show();	// 트리를 보여준다. 
	int  Height(TreeNode<T> *p, int cnt); // 트리의 가장 높은 높이를 측정한다

	// NON-AVL을 AVL로 만들어준다
	bool AVL(TreeNode<T> *pp, TreeNode<T> *p, TreeNode<T> *newnode);
	// 해당 node(p)아래로 찾는노드(n)의 존재 유무확인
	bool findT(TreeNode<T> *p, TreeNode<T> *n);
private:
	TreeNode<T>* root; // pointer to the root node
	int h = 0;	// 가장 높은 높이를 갖는 변수
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

// 트리를 Inorder 방식으로 표현한다.
template <class T>
void  BST<T>::Inorder(TreeNode<T> *ptr)
{
	int rh, lh;
	if (ptr){
		Inorder(ptr->leftChild);
		h = 0;
		rh = Height(ptr->rightChild, 0);	// 오른쪽 트리 높이
		h = 0;
		lh = Height(ptr->leftChild, 0);   // 왼쪽 트리 높이
		cout << ptr->data.key << "(" << lh - rh << ") "; // BF출력
		Inorder(ptr->rightChild);
	}
}

// 트리에 노드를 삽입한다.
template <class T>
bool  BST<T>::Insert(T keyval)
{
	TreeNode<T> *p = root, *pp = 0;

	// 삽입하려는 노드의 key 위치를 배정
	while (p){
		pp = p;
		// 새로운 key가 현재 key보다 작다면 left child 검색
		if (keyval.key < p->data.key)
			p = p->leftChild;
		// 새로운 key가 현재 key보다 크다면 right child 검색
		else if (keyval.key > p->data.key)
			p = p->rightChild;
		// 새로운 key가 현재 key와 같다면 이미 존재하므로 에러!
		else if (keyval.key == p->data.key){
			cout << "ERROR : already in tree" << endl;
			return false;
		}
	}
	p = new TreeNode<T>(keyval);	// 새로운 노드 생성

	// root가 존재 할시
	if (root){
		// 부모노드의 key보다 작다면 left,
		// 부모노드의 key보다 크다면 right
		if (keyval.key< pp->data.key)
			pp->leftChild = p;
		else
			pp->rightChild = p;
	}
	// root가 존재하지 않는다면 새로운 노드가 root
	else
		root = p;

	// 트리의 깊이 출력
	cout << "Height of tree : " << Height(root, 0) << endl;
	AVL(NULL, root, p);	// 새로 삽입된 노드 p는 
	// 포함되어 있는 서브트리를 검사할 때 사용
	return true;
}

// ---------- AVL 수행 함수 -------------
template <class T>
bool BST<T> ::AVL(TreeNode<T> *pp, TreeNode<T> *p, TreeNode<T> *newnode){

	TreeNode<T> *tp = 0, *ttp = 0;
	int rh, lh;

	if (!p){
		return false;	// p가 존재하지 않는다면 연산 할 필요 없음 
	}
	AVL(p, p->rightChild, newnode);
	AVL(p, p->leftChild, newnode);

	h = 0;
	rh = Height(p->rightChild, 0);	// 오른쪽 트리 높이
	h = 0;
	lh = Height(p->leftChild, 0);   // 왼쪽 트리 높이


	// BF 값이 불안정
	if ((lh - rh) >= 2){
		// L- 일때
		if (AVL(p, p->leftChild, newnode) == true){
			tp = p->leftChild;
			// LL 일때
			if (findT(tp->leftChild, newnode)){
				cout << "[" << p->data.key << "(" << (lh - rh) << ")"
					<< "에서 LL 회전 수행]" << endl;
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

			// LR 일때
			else if (findT(tp->rightChild, newnode)){
				cout << "[" << p->data.key << "(" << (lh - rh) << ")"
					<< "에서 LR 회전 수행]" << endl;
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

	// BF 값이 불안정 
	else if ((lh - rh) <= -2){
		// R- 일때
		if (AVL(p, p->rightChild, newnode) == true){
			tp = p->rightChild;
			//RR일때
			if (findT(tp->rightChild, newnode)){
				cout << "[" << p->data.key << "(" << (lh - rh) << ")"
					<< "에서 RR 회전 수행]" << endl;

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

			//RL일때
			else if (findT(tp->leftChild, newnode)){
				cout << "[" << p->data.key << "(" << (lh - rh) << ")"
					<< "에서 RL 회전 수행]" << endl;
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
		return true;	// BF값이 안정적이다

	return false;		// BF가 +2이상 또는 -2이하면 false BF값 불안정
}
// -------------------------------------------------

// 노드 밑에 새로입력된 노드가 있는지 검색한다
template <class T>
bool BST<T> ::findT(TreeNode<T> *p, TreeNode<T> *n){
	while (p){
		// 새로운 key가 현재 key보다 작다면 left child 검색
		if (n->data.key < p->data.key)
			p = p->leftChild;
		// 새로운 key가 현재 key보다 크다면 right child 검색
		else if (n->data.key > p->data.key)
			p = p->rightChild;
		// 새로운 key가 현재 key와 같다면 이미 존재하므로 에러!
		else if (p->data.key == n->data.key)
			return true;
	}
	return false;
}
// 높이를 구하는 함수다. 이 함수는 트리가 하단으로 내려갈때마다 1씩 더해져
// child가 없는 단말 노드에서 최종적으로 더해진 값을 비교하여 최고 높이(h)를 구한다.
template <class T>
int  BST<T> ::Height(TreeNode<T> *p, int cnt){

	// recursive하게 호출하여 높이를 구한다
	if (p){
		cnt++;
		Height(p->leftChild, cnt);
		Height(p->rightChild, cnt);
	}

	// 단말노드에 도달했을 경우 높이를 최고 높이와 비교하여 정한다.
	else{
		if (h < cnt)
			h = cnt;
	}
	return h;
}