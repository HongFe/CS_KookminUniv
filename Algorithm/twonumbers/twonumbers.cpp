
/*//////////////////////Two Numbers///////////////////////



국민대학교 컴퓨터공학부 3학년 나홍철


//////////////////////////////////////////////////////////*/
#include <stdio.h>
#include <vector>
#include <algorithm>

using namespace std;

#define SET_MAX_SIZE 1000000
#define TRUE 1
#define FALSE 0

int main(void){
	FILE *in;
	int numTestCase;
	int size, numK, numTemp, i, decSize;
	vector<int> vecSet;

	
	in = fopen("input.txt", "r");
	fscanf(in, "%d", &numTestCase);

	while (numTestCase--){
		// ~ init 
		vecSet.clear();
		fscanf(in, "%d %d", &size, &numK);

		// ~ scanf
		for (i = 0; i < size; i++){
			fscanf(in, "%d", &numTemp);
			vecSet.push_back(numTemp);
		}

		// ~ sorting
		sort(vecSet.begin(), vecSet.end());

		// ~ search
		decSize = vecSet.size() - 1;
		int interval = 0x7FFFFFFF;
		int numOfPairs = 0;

		while (size--){
			int key = vecSet[decSize];
			int destination = numK - key;

			// ~  Binary Search
			int beginPoint = 0, endPoint = decSize-1;
			int midPoint;

			while (beginPoint <= endPoint){

				midPoint = (beginPoint + endPoint) / 2;

				int tempInterval = vecSet[decSize] + vecSet[midPoint] - numK; //
				tempInterval = tempInterval < 0 ? -tempInterval : tempInterval;
				


				if (vecSet[midPoint] == destination){
					if (interval == tempInterval){
						numOfPairs++;
					}
					else{
						interval = tempInterval;
						numOfPairs = 1;
					}
					break; // ~ ???
				}
				else if (vecSet[midPoint] < destination){
					if (interval > tempInterval){
						interval = tempInterval;
						numOfPairs = 1;
					}
					else if (interval == tempInterval){
						numOfPairs++;
					}
					else{

					}
					beginPoint = midPoint+1;
					
					
				}
				else{
					if (interval > tempInterval){
						interval = tempInterval;
						numOfPairs = 1;
					}
					else if (interval == tempInterval){
						numOfPairs++;
					}
					else{

					}
					endPoint = midPoint-1;
				}
			}

			decSize--;
		}

		printf("%d\n", numOfPairs);

	}

	return 0;
}