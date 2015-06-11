#include <iostream>
#include <fstream>

using std::ifstream;
using std::ofstream;
using std::endl;

using namespace std;

#define MAX_NUM 100
int main(){
	ifstream inStream;
	ofstream outStream;

	int last = 0;

	for (int i = 0; i <= MAX_NUM; i++){
		inStream.open("input.txt");
		outStream.open("input.txt", std::ios::app);

		if (inStream.fail())
		{
			cerr << "Input file opeing failed\n";
			exit(1);
		}

		if (outStream.fail())
		{
			cerr << "output file opeing failed\n";
			exit(1);
		}

		while (!inStream.eof())
			inStream >> last;

		outStream << "\n" << last + 1;
		outStream.close();
		inStream.close();
	}
	return 0;
}