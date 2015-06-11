#include <stdio.h>
#include "BST.cpp"

using namespace std;

int main(){

	ifstream inStream, inStream2;
	inStream.open("input.txt", std::ios::in);
	inStream2.open("input2.txt", std::ios::in);

	BST<Element> bst;

	char *tmp = new char[4];

	cout << "1. �����Խ� inorder ��ȸ����" << endl;
	cout << "[ó��Ʈ�� ����] ";
	bst.Show();
	cout << endl << "-------------------------Insert ����------------------------" << endl;

	// 1. Insert ����
	while (!inStream.eof())
	{
		inStream >> tmp;
		bst.Insert(Element(tmp));
		cout << tmp << " ���� : ";
		bst.Show();
	}
	cout << endl << "--------------------------�Ϸ�-------------------------------";


	cout << endl << endl << " 2. ���Լ����� ���� ��� ������ inorder ��ȸ����" << endl;
	cout << "[ó��Ʈ�� ����] ";
	bst.Show();
	cout << endl << "--------------------------Delete ����------------------------" << endl;

	inStream.seekg(0, ios_base::beg);	// ���� �����͸� ������ �̵�
	// 2. Delete ����
	while (!inStream.eof())
	{
		inStream >> tmp;
		bst.Delete(Element(tmp));
		cout << tmp << " ���� : ";
		bst.Show();
	}

	inStream.seekg(0, ios_base::beg);

	// Delete ������ ���� �⺻ Ʈ���� �籸��
	while (!inStream.eof())
	{
		inStream >> tmp;
		bst.Insert(Element(tmp));
	}
	cout << endl << "--------------------------�Ϸ�-------------------------------";

	cout << endl << endl << " 3. ���Լ����� ���� ��� ������ inorder ��ȸ����" << endl;
	cout << "[ó��Ʈ�� ����] ";
	bst.Show();
	cout << endl << "--------------------------Delete ����------------------------" << endl;

	// 3. Delete ����
	while (!inStream2.eof())
	{
		inStream2 >> tmp;	
		bst.Delete(Element(tmp));
		cout << tmp << " ���� : ";
		bst.Show();
	}

	cout << endl << "--------------------------�Ϸ�-------------------------------";


	inStream.close();
	inStream2.close();

	return 0;
}