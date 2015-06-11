/***************************************************************************************
*
*	문제 : Problem A - Ducci Sequence
*
*	국민대학교 전자정보통신대학 컴퓨터공학부 3 학년
*	김성근 20093267, 김세훈 20093268, 나홍철 20093284
*
*	해당 알고리즘은 문제에 나온 방법을 그대로 적용시켜서 해결하였습니다.
*	일단 Loop 가 최대 1000개를 생성한다고 문제에 나와있기 때문에 전역변수로
*	해당 Loop 가 수행될 때마다 한라인씩 저장하여 기록하도록 하였습니다.
*
*	그리고 마지막에 0이나오는 작업은 sequence를 수행하는 함수에서 바로바로
*	그 값을 저장하여 확인하도록 하여 그 합이 0이나오면 바로 끝나도록
*	알고리즘을 구성하였습니다
*
*****************************************************************************************/
#include <stdio.h>
#include <string.h>

// ~ 상수 정의
#define TRUE 1	// c++ 이 아닌 C로 작성했기 때문에 TRUE, FALSE 상수로 지정
#define FALSE 0

#define MIN_SIZE 3 // 각 Test Case가 입력받는 Size의 크기의 최소값과 최대값
#define MAX_SIZE 15
#define MAX_LOOP_SIZE 1000 // 각 Test Case가 수행될 시에 최대로 돌아가는 LOOP 횟수

int loop[MAX_LOOP_SIZE][MAX_SIZE] = {0}; // n-tuple을 기억해두기 위한 장소

/**
*	가장 초기에 input.txt로 부터 입력을 받을 함수.
*	@param *in
*		FILE
*	@param *ducci
*		file로 부터 데이터를 받아갈 행렬 
*	@param *size
*		입력 받은 n의 크기
*/
void input_scan(FILE *in, int *ducci, int *size){
	int i, temp;
	fscanf(in, "%d", size);	
	for(i = 0; i< *size; i++){
		fscanf(in, "%d", &temp);
		ducci[i] = temp;
	}
}
/**
*	Debug을 위하여 배열을 출력하는 함수
*
*	@param *arr
*		print를 시키기 위한 배열
*	@param *size
*		배열의 크기
*/
void print(int *arr, int *size){
	int i;
	for(i=0; i<*size; i++){
		printf("%d ", arr[i]);
	}
	printf("\n");
}

/**
*	절대값을 리턴해주는 함수
*
*	@param value
*		정수 ( 음, 양 상관없음)
*	@return
*		절대값 리턴
*/
int abs(int value){
	return value < 0 ? -value : value;
}

/**
*	Ducci Sequence를 수행할 함수
*
*	@param *ducci
*		sequecnce를 수행할 배열
*	@param *next
*		sequence를 수행뒤 받아갈 배열
*	@param *size
*		배열의 크기
*	@return
*		sequece를 모두 수행한 합을 리턴
*/
int sequence(int *ducci, int *next, int *size){
	int i, sum = 0;
	for(i=0; i<*size; i++){
		if(i != *size-1){
			next[i] = abs(ducci[i] - ducci[i+1]);
			sum += next[i];
		}else{
			next[i] = abs(ducci[i] - ducci[0]);
			sum += next[i];
		}
	}
	//print(next, size); // Debug code
	return sum;
}

int main(void){
	FILE *in;
	int iTestCase, i, j, size, sum = 0, result = TRUE;
	int count = 0, same;
	int ducci[MAX_SIZE] = {0};
	int next[MAX_SIZE] = {0};

	in = fopen("input.txt", "r");	
	fscanf(in, "%d", &iTestCase);
	while(iTestCase--){
		// ~ init variable
		result = TRUE;
		memset(ducci, 0, sizeof(int)*MAX_SIZE);
		memset(next, 0, sizeof(int)*MAX_SIZE);
		input_scan(in, ducci, &size);
		memcpy(*(loop+count), ducci, sizeof(int)*size); 
		// print(ducci, &size); // Debug code
		
		// ~ algorithm
		count = 1;
		while(TRUE){ // sum 0이 나오거나, 반복되는 경우가 나올때까지 무한히 loop
			if(sequence(ducci, next, &size) == 0)// sum == 0이면 break
				break;
			// ~ compare
			for(i=0; i<count; i++){
				same = 1;
				for(j=0; j<size; j++){
					if(loop[i][j] != next[j]){
						break;
					}else{
						same++;
					}

					if(same == size){
						result = FALSE;
					}
				}
			}

			if(!result) // 동일한 loop가 나왔음으로 break 수행
				break;

			// ~ copy array
			memcpy(*(loop+(count++)), next, sizeof(int)*size); // 메모리 카피를 통하여 배열을 저장
			memcpy(ducci, next, sizeof(int)*size); // 다음 sequence를 수행하기 위해 next에 있던것을 ducci로 이동
		}

		if(result)
			printf("ZERO\n");
		else
			printf("LOOP\n");
	}

	return 0;
}