#include "AVL.cpp"
using namespace std;

int main(){
	ifstream inStream;
	inStream.open("input.txt", std::ios::in);
	BST<Element> bst;

	char tmp = NULL;

	cout << "AVL Tree" << endl
		<< "-------------------------Insert 수행------------------------" << endl;
	// Insert 수행 16개 문자 입력받아 insert
	for (int i = 0; i < 16; i++)
	{
		inStream >> tmp;

		cout << tmp << " 삽입 - ";
		bst.Insert(Element(tmp));
		cout << "Inorder Key(BF): ";
		bst.Show();
		cout << endl;
	}
	cout << "--------------------------완료-------------------------------";

	inStream.close();

	return 0;
}
