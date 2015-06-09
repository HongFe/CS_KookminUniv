#include "Sortmerge.h"

Sortmerge::Sortmerge(){	max = 60;}

Sortmerge::Sortmerge(unsigned int num){	max = num;}

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
		cout << i << "»ý¼ºÅ° " << rand1 << "/t"			// print screen
			<< i << " (" << rand2 << ")" << endl;

		outStream << rand1 << "/t"						// save real log
			<< i << "(" << rand2 << ") ";
	}
	outStream.close();	
}

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

	int l1 = 0 ,l2 = 0; // location of intager
	
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
	bubble_sort(str, arrKey, 0, max,0);	// sorting Key(string)
	

	t = arrKey[0];
	num = 0;
	// Time Sort
	for (unsigned int j = 0; j < max; j++){	

		// Counting same key means Timestamp range of sorting sector. 
		if (arrKey[j] == t){
			arrCount[num]=arrCount[num]+1;
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

// Sorting Method
void Sortmerge::bubble_sort(string *str, int *Ary, int First, int ArySize, int dir)
{
	int i, j;
	string temp;

	for (i = ArySize - 1; i>First; i--)
		for (j = 0; j<i; j++)
		if ((*(Ary + j) > *(Ary + j + 1))){
			swap(str + j, str + j + 1,Ary + j, Ary + j + 1);
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

void Sortmerge::Merge(){
	ifstream inStreamT, inStreamO;;
	ofstream outStream,outStreamE;

	inStreamT.open("output2_sort.txt", std::ios::in);
	inStreamO.open("oldMaster.txt", std::ios::in);
	outStream.open("output3_newMaster.txt");
	outStreamE.open("output4_errormsg",std::ios::trunc);
	outStreamE.close();

	string trans;	// String of Transation
	string garbage;
	int okey = 0;			// Key of Oldmaster file
	int tkey = 0;		// key of Transation
	char type;		// Type of Transation	[i, m, d ]
	int t = 1, o = 1;		//	Line of File pointer
	int tmpK = 0;
	char tmpT = 0;
		
	while (1){
		type = NULL;
		// End of Transaction
		if (inStreamT.eof()){
			// End of Oldmaster
			if (inStreamO.eof()){ 
				inStreamT.close();
				inStreamO.close();
				outStream.close();
				exit(1); 
			}
				
			// Not End of Oldmaster
			else{
				while (!inStreamO.eof()){
					inStreamO >> okey;
					outStream << okey << " ";
				}
				inStreamT.close();
				inStreamO.close();
				outStream.close();
				exit(1);
			}
		}
		/////
		// Not End of Transaction
		else{
			inStreamT >> trans;
			type = CheckType(trans);
			tkey = CheckKey(trans);
			// Error Situation in Same key.	 ex)	2
			//		at transation					2
			if (tmpK == tkey){
				if (tmpT == 'i'){		
					if (type == 'i'){		//	i i
						ErrorM(type, t);
						t++;
						continue;
					}
				}
				else if (tmpT == 'd'){
					if (type == 'd'){		//	d d
						ErrorM(type, t);
						t++;
						continue;
					}
					else if (type == 'm'){	// d m
						ErrorM(type, t);
						t++;
						continue;
					}
				}
				else if (tmpT == 'm'){
					if (type == 'i'){		//	m i
						ErrorM(type, t); 
						t++;
						continue;
					}
				}			
			}

			// End of Oldmaster
			if (inStreamO.eof()){
				while (!inStreamT.eof()){					
					if (type == 'i'){
						outStream << tkey << " ";
						t++;
						inStreamT >> garbage;
					}
					else if ((type == 'm') || (type == 'd')){
						ErrorM(type, t);
						inStreamT >> garbage;
					}
				}
				inStreamT.close();
				inStreamO.close();
				outStream.close();
				exit(1);
			}

			// Not End of Oldmaster
			else{
				inStreamO >> okey;			

				// TRANS-KEY < OLD-MSTR-KEY
				if (tkey < okey){
					if (type == 'i'){
						outStream << tkey << " ";
						t++;
					}
					else {
						ErrorM(type, t); 
						t++;
					}
				}
				// TRANS-KEY = OLD-MSTR-KEY
				else if (tkey == okey){
					if (type == 'm'){	
						outStream.close();

						ifstream inStream;						
						inStream.open("output3_newMaster.txt");
						inStream.seekg(-2, std::ios_base::end);
						char *a = new char[1];

						inStream >> a;
						// If write key already
						//Something change in here
						if (tkey == atoi(a)){
							outStream << tkey << " ";
						}
						else{
							inStream.seekg(0, std::ios_base::end);
							outStream << tkey << " ";
						}
						t++;
						inStream.close();
						ofstream outStream;
						outStream.open("output3_newMaster.txt", std::ios::app);
					}
					else if (type == 'd'){
						t++;
						o++;
					}
					else {
						ErrorM(type, t);
						t++;
					}					
				}
				else if (tkey > okey){
					outStream << okey << " ";
					o++;
				}

			}
			tmpK = tkey;	// Save pre key
			tmpT = type;	// Save pre type
		}		
	} // while

}

int Sortmerge::CheckKey(const string str){
	char num[10] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
	
	for (int i = 0; i<10; i++){
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

char Sortmerge::CheckType(const string str){
	char type[3] = { 'm', 'i', 'd' };

	for (int i = 5; i <= 8; i++){
		for (int j = 0; j < 3; j++){
			if (str.at(i) == type[j])
				return type[j];				
		}
	}
}
void Sortmerge::ErrorM(const char type , const int line){
	ofstream outStream;

	//outStream.open("output4_errormsg.txt", std::ios::app);
	outStream.open("output4_errormsg.txt", std::ios::app);

	outStream << "<ERROR>  type : " 
		<< type << " | Line : " << line << endl;

	outStream.close();
}