#include <iostream>
using namespace std;

#include "treeOrder.cpp"


int main(){

	char node[1025] = { 0 };		// Node value (Limit line is 10) 
	int b = 1;

	cout << "TREE ORDER PROGRAM" << endl << endl;
	cout << "Input your Node Value. Blank node is '0'    [Finish command '!']" << endl;
	cout << " ex) 1Line : A / 2Line : BC / 3Line : D0EF / 4Line : 00000GH0 / 5Line : ! " << endl << endl;

	for (int i = 1; i<50; i = i++){
		cout << i << "Line ";
		for (int j = b; j<b * 2; j++){
			cin >> node[j];			// Store value in Node array

			// Command '!' means excute order function.
			if (node[j] == '!')
			{
				node[j] = 0;	// Remove character '!' 

				cout << "Preorder : ";
				preOrder(node, 1);
				cout << endl;

				cout << "Inoreder : ";
				inOrder(node, 1);
				cout << endl;

				cout << "Postorder : ";
				postOrder(node, 1);
				cout << endl;

				exit(1);
			}

		}
		b = b * 2;		// Number of Left node
	}
	return 0;
}
