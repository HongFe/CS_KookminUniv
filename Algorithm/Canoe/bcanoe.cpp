/************************************************************************
* 
* 국민대학교 전자정보통신대학 컴퓨터공학부 3 학년
* 김성근 20093267, 김세훈 20093268, 나홍철 20093284
*
* 이 문제는 Binary Search 를 이용해서 algorithm을 작성하였습니다.
* 
* Binary Search 를 사용하기 위해서 4개의 Class A, B, C, D를 2개의 Class로
* 변환 시키게 됩니다. 예를 들어 A+B, C+D 를 한뒤 조합을 하게 되는데 
* 이 계산이 가능한 이유는 입력받는 n의 크기가 1000 이기 때문에 A+B 조합을 한다고
* 하면 중복이 없다고 가정할 시 최대 1,000,000 개 임으로 계산이 가능한 정도의 
* 수 입니다.
*
* 그리고 이렇게 합을 계산한것을 E, F 라고 할 때 E를 기준으로 F에 대해서 근사 
* 값이 0에 가까워지도록 하는 value를 Binary Search 를 하여 값을 찾게되고
* 모든 값에 대해 검사하도록 합니다.
*
* 이 방법에 대한 착안은 만약에 Class가 2반만 있는 경우 Binary Search를 하여
* 검색하는 알고리즘 시간을 n log n 번만 하게하는 것을 이용하기 위해서 
* 시간이 오래걸리더라도 모든 항목에 대하여 덧셈을 수행하도록 한것 입니다.
*
**************************************************************************/
#include <stdio.h>
#include <vector>
#include <algorithm>

using namespace std;

#define MAXCLASS 4

/**
*   중복제거 함수
*
*   @param vec
*      몸무게를 더한 리스트
*   @return 중복을 제거한 새로운 vector
*/
vector<int> removeDuplication(vector<int> vec){
   vector<int> temp;
   int n = 0;
   temp.push_back(vec[0]);

   int size = vec.size();

   for (int i = 0; i < size; i++){
      if (vec[n] != vec[i]){
         temp.push_back(vec[i]);
         n = i;
      }
   }
   return temp;
}

/**
*   Binary Search를 이용하여 최상의 무게를 계산하는 함수
*
*   @param abVec
*      classA, classB를 더한 리스트
*   @param cdvec
*      classC, classD를 더한 리스트
*   @param k
*      specific value of a boat
*   @param size
*      the number of students in each class
*   @return 
*      최상의 무게
*/
int extractBestWeight(const vector<int>& abVec, const  vector<int>& cdVec, int numK){
   int interval = 0x7FFFFFFF;
   int bestWeight;
   int numTemp;
   int size = cdVec.size();
   
   bestWeight = cdVec[0] + abVec[0];
   
   for(int i=0; i<size; i++){
      // ~  Binary Search
      int beginPoint = 0, endPoint = abVec.size() - 1; // 시작점과 끝점 지정
      int midPoint; // 중간점

      while (beginPoint <= endPoint){ // 시작점이 끝점 보다 클경우 종료
         midPoint = (beginPoint + endPoint) / 2; // 중간 지점 계산
         numTemp = cdVec[i] + abVec[midPoint]; // 중간점에 대한 값 계산

         if (numTemp == numK) return numTemp; // 두 가지가 같은 값이라면 종료
         else if (numTemp > numK){ // 계산한 값이 더 크다면 
            // 절대값을 통해서 값을 비교하고 해당 값이 같다면 음수가 더 우선
            if ((abs(interval) > abs(numK - numTemp)) || ((abs(interval) == abs(numK - numTemp)) && (interval < 0))){
               interval = numK - numTemp;
               bestWeight = numTemp;
            }
            endPoint = midPoint - 1; // endPoint 이동
         }else { // 계산한 값이 더 작다면
            // 절대값을 통해서 값을 비교하고 해당 값이 같다면 음수가 더 우선
            if ((abs(interval) > abs(numK - numTemp)) || ((abs(interval) == abs(numK - numTemp)) && (interval < 0))){
               interval = numK - numTemp;
               bestWeight = numTemp;
            }
            beginPoint = midPoint + 1; // beginPoint 이동
         }
      }
   }
   return bestWeight;
}

/**
*   input.txt로 부터 Data를 입력받는 함수
*
*   @param in
*      열려있는 file
*   @param vec
*      vector
*   @param size
*      number of students
*/
void scanf(FILE *in, vector<int>& vec, int size){
   int numTemp;
   vector<int> tempVec;
   
   vec.clear(); // vector를 main 에서 선언하기 때문에 입력받을 때 항상 clear
   for(int i=0; i<2; i++){ // 스캔을 받으면서 두개의 라인에 대한 합을 계산
      for(int j=0; j<size; j++){ 
         fscanf(in, "%d", &numTemp);
         if(i == 0)
            // 처음 들어오는 값에 대해서 임시 저장
            tempVec.push_back(numTemp);
         else{
            // 스캔을 받으면서 동시에 합을 하기 위해 for문을 돌리면서 저장
            for (int k = 0; k < size; k++){
               vec.push_back(numTemp+tempVec[k]);
            }
         }
      }
   }
}

int main(void){
   FILE *in;
   int numTestCase, numK, size;
   vector<int> abVec, cdVec;

   in = fopen("input.txt", "r");
   if (in == NULL) exit(1);

   fscanf(in, "%d", &numTestCase);
   while (numTestCase--){
      // ~ init variable
      fscanf(in, "%d %d", &numK, &size);
      scanf(in, abVec, size);
      scanf(in, cdVec, size);

      // ~ sorting
      sort(abVec.begin(), abVec.end());
      sort(cdVec.begin(), cdVec.end());

      // ~ 중복제거
      abVec=removeDuplication(abVec);
      cdVec=removeDuplication(cdVec);
      
      // ~ print
      printf("%d\n", extractBestWeight(abVec, cdVec, numK));      
   }
   fclose(in);
   return 0;
}