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

// Element구조체는 노드에 
// 입력되는 문자를 담을 수 있도록 해준다
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
	BST();	// 생성자
	bool  Insert(T keyval);	// 트리에 노드를 추가하는 함수다.
	bool  Delete(T keyval); // 트리의 노드를 제거하는 함수다.
	void  Inorder(TreeNode<T> *ptr);	// 트리를 Inorder 방식으로 읽는다.
	void  Show();	// 트리를 보여준다. 
	int  Height(TreeNode<T> *p, int cnt); // 트리의 가장 높은 높이를 측정한다
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
	if (ptr){
		Inorder(ptr->leftChild);
		cout << ptr->data.key << " ";
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
		if (strcmp(keyval.key, p->data.key)<0)		// 새로운 key가 현재 key보다 작다면 left child 검색
			p = p->leftChild;
		else if (strcmp(keyval.key, p->data.key)>0)	// 새로운 key가 현재 key보다 크다면 right child 검색
			p = p->rightChild;
		else if (strcmp(keyval.key, p->data.key) == 0){	// 새로운 key가 현재 key와 같다면 이미 존재하므로 에러!
			cout << "ERROR : already in tree" << endl;
			return false;
		}
	}
	p = new TreeNode<T>(keyval);	// 새로운 노드 생성

	// root가 존재 할시
	if (root){
		// 부모노드의 key보다 작다면 left,
		// 부모노드의 key보다 크다면 right
		if (strcmp(keyval.key, pp->data.key) < 0)
			pp->leftChild = p;
		else
			pp->rightChild = p;
	}
	// root가 존재하지 않는다면 새로운 노드가 root
	else
		root = p;

	return true;
}

// 입력된 노드를 트리에서 제거
template <class T>
bool BST<T>::Delete(T keyval)
{
	bool ret = true;
	TreeNode<T> *p = root, *pp = 0;
	TreeNode<T> *tp = 0, *tpp = 0;	//tp 는 2node delete에서 검사할 노드와 parent노드
	int rh, lh;

	if (root == NULL)
		return false;

	//만약 p가 존재 않 한다면
	if (!p)
		return false;

	pp = p;
	while (p){
		// p보다 작은 인수 값의 경우 왼쪽에서 찾는다
		if (strcmp(keyval.key, p->data.key)<0){
			pp = p;
			p = p->leftChild;
		}
		// p보다 큰 인수 값의 경우 오른쪽에서 찾는다
		else if (strcmp(keyval.key, p->data.key)>0){
			pp = p;
			p = p->rightChild;
		}
		//찾는 node 발견시
		else if (strcmp(keyval.key, p->data.key) == 0)
			break;	
	}
	// 찾는 key가 존재하지 않는다면
	if(!p) {
		cout << "ERROR : Node Not Exist" << endl;
		return false;
	}

	// 삭제되는 노드의 서브중 높이가 가장 높은 서브트리를 구한다.
	h = 0;
	rh = Height(p->rightChild, 0);	// 오른쪽 트리 높이
	h = 0;
	lh = Height(p->leftChild, 0);   // 왼쪽 트리 높이

	tpp = pp;

	// 만약 지우려는 노드가 단말노드일때
	if (!p->leftChild && !p->rightChild){
		// 지우려는 노드가 부모노드의 left child일때 부모노드는 NULL을 가르킨다
		if (pp->leftChild == p){
			pp->leftChild = NULL;
		}
		// 지우려는 노드가 부모노드의 left child일때 부모노드는 NULL을 가르킨다.
		else if (pp->rightChild == p){
			pp->rightChild = NULL;
		}
	}
	// 만약 지우려는 노드가 단말노드가 아닐때
	else{
		// 오른쪽의 높이가 높을때 오른쪽 서브트리의 가장 작은 값을 대체할 값으로 지정	
		if (rh > lh){
			tp = p->rightChild;
			// 서브트리의 가장 작은 값을 구한다
			while (tp->leftChild != NULL){
				tpp = tp;
				tp = tp->leftChild;
			}

			// 삭제할 노드 p 의 부모가 존재할때 대체할 노드를 가르킨다.
			if (pp->leftChild == p){
				pp->leftChild = tp;
			}
			else if (pp->rightChild == p){
				pp->rightChild = tp;
			}

			// 만약 p의 child가 바로 대체할 값이라면
			if (p->rightChild == tp){
				if (p->leftChild)
				{
					tp->leftChild = p->leftChild;
				}
			}
			else{
				//만약 가장 작은값(대체할 값)의 right child가 존재하면 대체되는 값의 부모의 left child가 된다
				if (tp->leftChild){
					tpp->leftChild = tp->rightChild;
				}
				else
					tpp->leftChild = NULL;

				// 삭제할 값의 left child가 존재하면
				if (p->leftChild){
					tp->leftChild = p->leftChild;
				}
				// 삭제할 값의 right child가 존재하면
				if (p->rightChild){
					tp->rightChild = p->rightChild;

				}
			}
		}
		// 왼쪽의 높이가 높을때 왼쪽 서브트리의 가장 큰 값을 대체할 값으로 지정	
		else{
			tp = p->leftChild;

			// 서브트리의 가장 큰 값을 구한다
			while (tp->rightChild != NULL){
				tpp = tp;
				tp = tp->rightChild;
			}

			// 삭제할 노드 p 의 부모가 존재할때 대체할 노드를 가르킨다.
			if (pp->leftChild == p){
				pp->leftChild = tp;

			}
			else if (pp->rightChild == p){
				pp->rightChild = tp;
			}

			// 만약 p의 child가 바로 대체할 값이라면
			if (p->leftChild == tp){
				if (p->rightChild)
				{
					tp->rightChild = p->rightChild;
				}
			}
			else{
				//만약 가장 큰값(대체할 값)의 left child가 존재하면 대체되는 값의 부모의 right child가 된다
				if (tp->leftChild){
					tpp->rightChild = tp->leftChild;
				}
				else
					tpp->rightChild = NULL;

				// 삭제할 값의 left child가 존재하면
				if (p->leftChild){
					tp->leftChild = p->leftChild;
				}
				// 삭제할 값의 right child가 존재하면
				if (p->rightChild){
					tp->rightChild = p->rightChild;
				}
			}
		}
	}
	// 만약 삭제되는 값이 root일경우 대체된 값을 root로 재설정한다.
	if (p == root){
		root = tp;
	}
	delete p;
	return ret;	// return true
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

	// 단말노드에 다달았을 경우 높이를 최고 높이와 비교하여 정한다.
	else{
		if (h < cnt)
			h = cnt;
	}
	return h;
}

