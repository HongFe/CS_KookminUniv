#include<iostream>
#include<string.h>
using namespace std;

// Element Class
// this is the key-value pair
// We are going to put this pair into each node of the BST
struct Element {
	int key;
	char value;
	Element(int k, int v = 0) { key = k; value = v; };
	Element() {};
};

// forward declaration
template <class T> class BST;

// ---------- TreeNode class -------------

template <class T>
class TreeNode
{
	friend class BST<T>;

private:
	T data;	// storage for data : T will be Element
	TreeNode<T> *leftChild;	// link to the left Child
	TreeNode<T> *rightChild;	// link to the right Child

public:
	// constructors come here
	TreeNode(T d);
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
	BST();	// constructor
	~BST(); // destructor

	bool  Insert(T keyval);
	T  Get(T keyval);
	bool  Delete(T keyval);

	void  PreOrder(TreeNode<T> *ptr);
	void  InOrder(TreeNode<T> *ptr);
	void  PostOrder(TreeNode<T> *ptr);
	int  Count(TreeNode<T> *ptr);


	void  Show();	// Show the nodes in the Binary Search Tree  
private:
	TreeNode<T>* root; // pointer to the root node
};

template<class T>
BST<T>::BST()
{ // BST constructor. Initialize root variable
	root = NULL; // root is NULL, an empty binary search tree
}


template<class T>
BST<T>::~BST()
{ // BST destructor. Delete all nodes 
	// in the binary search tree

	// ignore this in HW 3

}


template <class T>
void  BST<T>::Show()
{
	cout << "Pre  Order : ";
	PreOrder(root);
	cout << endl << "In   Order : ";
	InOrder(root);
	cout << endl << "Post Order : ";
	PostOrder(root);
	cout << endl << "Count      : ";
	cout << Count(root) << endl;
}

#include "Tree_body.cpp"

int main()
{
	BST<Element> bst;
	// a binary search tree of type Element

	while (1)
	{
		cerr << "BST > ";
		char command[256];
		cin >> command;

		//	cout << command << " ";
		if (strcmp(command, "quit") == 0)
		{
			break;
		}
		else if (strcmp(command, "ins") == 0)
		{
			int key;
			char val;
			cin >> key >> val;
			if (bst.Insert(Element(key, val)) == false)
				cout << "Key Updated" << endl;
		}
		else if (strcmp(command, "del") == 0)
		{
			int key;
			cin >> key;
			if (bst.Delete(Element(key)) == false)
				cout << "Cannot Delete, Non Existing Key" << endl;
		}
		else if (strcmp(command, "get") == 0)
		{
			int key;
			cin >> key;
			Element p = bst.Get(Element(key));
			if (p.key == -1)
				cout << "Non Existing Key" << endl;
			else
				cout << "Item " << p.key << " " << p.value << endl;
		}

		// show the current binary search tree
		bst.Show();
	}
	cerr << endl;

	return 1;
}
