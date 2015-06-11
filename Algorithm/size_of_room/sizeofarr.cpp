
/*///////////////////방의 크기 구하기//////////////////////////



국민대학교 컴퓨터공학부 3학년 나홍철


//////////////////////////////////////////////////////////*/
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <algorithm>

using namespace std;

void find(char **arr, unsigned int rowSize, unsigned int colSize, unsigned int row, unsigned int col, unsigned int num, vector<int> &countArr);

int main(void){
	FILE *in;
	int numTestCase;
	unsigned int rowSize, colSize;
	vector<int> countArr;
	int num;

	in = fopen("input.txt", "r");
	fscanf(in, "%d", &numTestCase);

	


	while (numTestCase--){
		// ~ init 				
		fscanf(in, "%d %d", &colSize, &rowSize);
		num = 0;
		countArr.clear();
		
		char** pData = new char*[rowSize];

		for (int i = 0; i < rowSize; i++)
		{
			pData[i] = new char[colSize];
			for (int j = 0; j < colSize; j++){
				fscanf(in, "%c", &pData[i][j]);
				if (pData[i][j] == '\n')
					fscanf(in, "%c", &pData[i][j]);

			}
		}


		for (int k = 1; k < rowSize - 1; k++){
			for (int m = 1; m < colSize - 1; m++){
				if (pData[k][m] == '.'){
					pData[k][m] = '0';
					countArr.push_back(1);
					find(pData, rowSize, colSize, k, m, num, countArr);
					num++;				
				}
			}
		}

		sort(countArr.begin(), countArr.end());

		printf("%d\n", countArr.size());
		for (int n = countArr.size()-1; n >= 0; n--)
				printf("%d ", countArr[n]);

		printf("\n");

		for (int z = 0; z < rowSize; z++)
		{
			delete[] pData[z];
		}
		delete[] pData;
	}

	return 0;
}


void find(char **arr, unsigned int rowSize, unsigned int colSize, unsigned int row, unsigned int col, unsigned int num, vector<int> &countArr){



	if (row - 1 > 0){
		if (arr[row - 1][col] == '.'){
			arr[row - 1][col] = 'O'; //num;
			countArr[num]++;
			find(arr, rowSize, colSize, row - 1, col, num, countArr);
		}
	}

	if (row + 1 < rowSize -1){
		if (arr[row + 1][col] == '.'){
			arr[row + 1][col] = 'O'; //num;
			countArr[num]++; 
			find(arr, rowSize, colSize, row + 1, col, num, countArr);

		}
	}

	if (col - 1 > 0){
		if (arr[row][col - 1] == '.'){
			arr[row][col - 1] = 'O'; //num;
			countArr[num]++;
			find(arr, rowSize, colSize, row , col-1 , num, countArr);
		}
	}

	if (col +1  < colSize-1){
		if (arr[row][col + 1] == '.'){
			arr[row][col + 1] = 'O'; //num;
			countArr[num]++;
			find(arr, rowSize, colSize, row, col+1, num, countArr);
		}
	}
}
