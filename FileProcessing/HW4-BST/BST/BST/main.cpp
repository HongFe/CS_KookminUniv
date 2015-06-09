#include <stdio.h>
#include "BST.cpp"

using namespace std;

int main(){

	ifstream inStream, inStream2;
	inStream.open("input.txt", std::ios::in);
	inStream2.open("input2.txt", std::ios::in);

	BST<Element> bst;

	char *tmp = new char[4];

	cout << "1. 노드삽입시 inorder 순회순서" << endl;
	cout << "[처음트리 상태] ";
	bst.Show();
	cout << endl << "-------------------------Insert 수행------------------------" << endl;

	// 1. Insert 수행
	while (!inStream.eof())
	{
		inStream >> tmp;
		bst.Insert(Element(tmp));
		cout << tmp << " 삽입 : ";
		bst.Show();
	}
	cout << endl << "--------------------------완료-------------------------------";


	cout << endl << endl << " 2. 삽입순서와 동일 노드 삭제시 inorder 순회순서" << endl;
	cout << "[처음트리 상태] ";
	bst.Show();
	cout << endl << "--------------------------Delete 수행------------------------" << endl;

	inStream.seekg(0, ios_base::beg);	// 파일 포인터를 앞으로 이동
	// 2. Delete 수행
	while (!inStream.eof())
	{
		inStream >> tmp;
		bst.Delete(Element(tmp));
		cout << tmp << " 삭제 : ";
		bst.Show();
	}

	inStream.seekg(0, ios_base::beg);

	// Delete 수행을 위해 기본 트리로 재구성
	while (!inStream.eof())
	{
		inStream >> tmp;
		bst.Insert(Element(tmp));
	}
	cout << endl << "--------------------------완료-------------------------------";

	cout << endl << endl << " 3. 삽입순서와 역순 노드 삭제시 inorder 순회순서" << endl;
	cout << "[처음트리 상태] ";
	bst.Show();
	cout << endl << "--------------------------Delete 수행------------------------" << endl;

	// 3. Delete 수행
	while (!inStream2.eof())
	{
		inStream2 >> tmp;	
		bst.Delete(Element(tmp));
		cout << tmp << " 삭제 : ";
		bst.Show();
	}

	cout << endl << "--------------------------완료-------------------------------";


	inStream.close();
	inStream2.close();

	return 0;
}