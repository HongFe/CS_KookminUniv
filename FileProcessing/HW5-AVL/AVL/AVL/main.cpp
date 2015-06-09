#include "AVL.cpp"
using namespace std;

int main(){
	ifstream inStream;
	inStream.open("input.txt", std::ios::in);
	BST<Element> bst;

	char tmp = NULL;

	cout << "AVL Tree" << endl
		<< "-------------------------Insert ����------------------------" << endl;
	// Insert ���� 16�� ���� �Է¹޾� insert
	for (int i = 0; i < 16; i++)
	{
		inStream >> tmp;

		cout << tmp << " ���� - ";
		bst.Insert(Element(tmp));
		cout << "Inorder Key(BF): ";
		bst.Show();
		cout << endl;
	}
	cout << "--------------------------�Ϸ�-------------------------------";

	inStream.close();

	return 0;
}
