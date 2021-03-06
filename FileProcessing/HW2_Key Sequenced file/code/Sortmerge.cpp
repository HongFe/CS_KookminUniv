#include "Sortmerge.h"

Sortmerge::Sortmerge(){ max = 60; }

Sortmerge::Sortmerge(unsigned int num){ max = num; }

// Making transaction file
void Sortmerge::makeTrans()
{
	ofstream outStream;

	srand((int)time(NULL));
	char ran_code[3] = { 'i', 'd', 'm' };
	int rand1;		//time stamp
	char rand2;		//random code

	outStream.open("output1_trans.txt", std::ios::trunc);

	if (outStream.fail())
	{
		cerr << "output file opeing failed\n";
		exit(1);
	}

	for (unsigned int i = 1; i <= max; i++){

		rand1 = rand() % 20 + 1;
		rand2 = ran_code[rand() % 3];
		cout << i << ".����Ű " << rand1 << "/t"			// print screen
			<< i << " (" << rand2 << ")" << endl;

		outStream << rand1 << "/t"						// save real log
			<< i << "(" << rand2 << ") ";
	}
	outStream.close();
}

// Sorting transaction file
void Sortmerge::sortTrans()
{
	ifstream inStream;
	ofstream outStream;

	inStream.open("output1_trans.txt", std::ios::in);
	outStream.open("output2_sort.txt");

	int *arrKey = new int[max];
	int *arrTime = new int[max];

	int arrCount[20] = { 0 }; // initialize same Key array counting 
	int t, num;

	string *str = new string[max];

	int l1 = 0, l2 = 0; // location of intager

	// Key Sort
	for (unsigned int i = 0; i < max; i++)
	{
		inStream >> str[i];

		// Extraction key
		if (str[i].find_first_of("/", l1)){
			l2 = str[i].find("/", l1);
		}
		arrKey[i] = atoi(str[i].substr(l1, l2).c_str());		// KEY : char -> intager		
	}
	bubble_sort(str, arrKey, 0, max, 0);	// sorting Key(string)


	t = arrKey[0];
	num = 0;
	// Time Sort
	for (unsigned int j = 0; j < max; j++){

		// Counting same key means Timestamp range of sorting sector. 
		if (arrKey[j] == t){
			arrCount[num] = arrCount[num] + 1;
		}
		else {
			num++;
			t = arrKey[j];
		}

		// Extraction time stamp
		if (str[j].find_first_of("t", 0)){
			l1 = str[j].find("t", 0);
			if (str[j].find_first_of("(", l1)){
				l2 = str[j].find("(", l2);
			}
		}
		arrTime[j] = atoi(str[j].substr(l1, l2).c_str());		// Time Stamp : char -> intager
	}

	for (int k = 0; k < 20; k++){
		bubble_sort(str, arrTime, arrCount[k], arrCount[k], 0);	// Sorting Time(string)
	}

	for (unsigned int l = 0; l < max; l++){
		outStream << str[l] << " ";			//Output string in text
	}

	delete[] arrTime;
	delete[] arrKey;

	inStream.close();
	outStream.close();
}

// Bubble Sorting method. dir=0 ascending order, dir=1 descending order
void Sortmerge::bubble_sort(string *str, int *Ary, int First, int ArySize, int dir)
{
	int i, j;
	string temp;

	for (i = ArySize - 1; i > First; i--)
	for (j = 0; j<i; j++)
	if ((*(Ary + j) > *(Ary + j + 1))){
		swap(str + j, str + j + 1, Ary + j, Ary + j + 1);
	}
}

// Swap two variable method for sort method
void Sortmerge::swap(string *sx, string *sy, int *x, int *y)
{
	string stemp = *sx;
	*sx = *sy;
	*sy = stemp;
	int temp = *x;
	*x = *y;
	*y = temp;
}

// Transaction merge with oldmaster file  
void Sortmerge::Merge(){
	ifstream inStreamT, inStreamO;;
	ofstream outStream;

	inStreamT.open("output2_sort.txt", std::ios::in);
	inStreamO.open("oldMaster.txt", std::ios::in);
	outStream.open("output4_errormsg.txt", std::ios::trunc);	//Error msg file initialize

	outStream.close();

	string trans;	// String of Transation
	int maxold = 20;	//maximum oldmaster size
	int tmp[21] = { 0 };	//	1~20
	int t = 0;
	int tp = 1; // tp->Transaction pointer	
	int tkey;	//Transaction Key
	char type;	//Transaction type

	// old value in array
	while (!inStreamO.eof()){
		inStreamO >> t;
		tmp[t] = 1;
	}

	inStreamT >> trans;

	while (!inStreamT.eof()){
		type = CheckType(trans);
		tkey = CheckKey(trans);

		if (type == 'i'){			// if type 'i'
			if (tmp[tkey] == 0){	
				tmp[tkey] = 1;
			}
			else{
				ErrorM(type, tp);
			}
		}
		else if (type == 'd'){			// if type 'd'
			if (tmp[tkey] == 0){
				ErrorM(type, tp);
			}
			else{
				tmp[tkey] = 0;
			}
		}
		else if (type == 'm'){			// if type 'm'
			if (tmp[tkey] == 0){
				ErrorM(type, tp);
			}
			else{
				tmp[tkey] = 1;
			}
		}
		tp++;	// transaction pointer ++
		inStreamT >> trans;
	} // while
	PrintArr(tmp);

	inStreamT.close();
	inStreamO.close();
}

// Print temp array
void Sortmerge::PrintArr(int *tmp){

	ofstream outStream;
	outStream.open("output3_newMaster.txt");

	for (int i = 1; i <= 20; i++){
		if (tmp[i])
			outStream << i << " ";
	}
	outStream.close();
}

// Check key of transaction one string
int Sortmerge::CheckKey(const string str){
	char num[10] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };

	for (int i = 0; i < 10; i++){
		if (str.at(0) == num[i]){	// First number of Key
			for (int j = 0; j < 10; j++){
				if (str.at(1) == num[j])
					return i * 10 + j;
			}
			return i;
		}
	}
	return -1;
}

// Check type of transaction one string
char Sortmerge::CheckType(const string str){
	char type[3] = { 'm', 'i', 'd' };

	for (int i = 5; i <= 8; i++){
		for (int j = 0; j < 3; j++){
			if (str.at(i) == type[j])
				return type[j];
		}
	}
}

// Print error massage
void Sortmerge::ErrorM(const char type, const int line){
	ofstream outStream;

	//outStream.open("output4_errormsg.txt", std::ios::app);
	outStream.open("output4_errormsg.txt", std::ios::app);

	outStream << "<ERROR>  type : "
		<< type << " | Line number in Transaction : " << line << endl;

	outStream.close();
}
