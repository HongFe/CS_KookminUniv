/***************************************************************************************
*
*           문제 : ProblemF 순서(order)
*
*                국민대학교 전자정보통신대학 컴퓨터공학부 3 학년
*                김성근 20093267, 김세훈 20093268, 나홍철 20093284
*
*****************************************************************************************
* 만약 n개의 정수로 된 R의 n번째 숫자(마지막)가 7이라면 1~n까지의 나열된 숫자중 
* 8이 확실하다. 이는 S의 n번째 숫자를 확실하게 구할 수 있다는 것이다.
*
* 그 다음 n-1 또한 1~n까지의 숫자중 이미 선택된 8을 제외한 숫자들중 확실한
* 숫자를 찾을 수 있다. 

* 이처럼 R의 n, n-1, n-2,,,3,2,1 번째 숫자를 이용하여
* S의 n, n-1 ,n-2,,,3,2,1 번재 숫자를 차례대로 찾을 수 있다.
*
* 이 내용의 알고리즘을 나타내보자.
*
* 배열 number = [1~n] , R, S 가 존재 할 때,
*
* S[n-1] = number[R[n-1]] | number의 R[n-1]번째 이후 오른쪽 숫자를 왼쪽으로 한칸씩 이동 
* S[n-2] = number[R[n-2]] | number의 R[n-2]번째 이후 오른쪽 숫자를 왼쪽으로 한칸씩 이동 
* ....
* S[1] = number[R[1]] | number의 R[1]번째 이후 오른쪽 숫자를 왼쪽으로 한칸씩 이동 
* S[0] = number[R[0]] | number의 R[0]번째 이후 오른쪽 숫자를 왼쪽으로 한칸씩 이동
*                          (하지만 이후에 존재하지 않으므로 아무것도 하지않음)
****************************************************************************************/
#include <stdio.h>
#include <stdlib.h>

int Lineup(int *number, int begin, int end);
int OrderNum(int* rSet, int* sSet, int* number, int n);
int PrintNum(int *sSet, int n);

int main(void){
	FILE *in;
	int numTestCase;
	int n;  // R,S 공통의 갯수 n

	in = fopen("input.txt", "r");
	if (in == NULL) exit(1);

	fscanf(in, "%d", &numTestCase);
	
	while (numTestCase--){

		fscanf(in, "%d", &n);
		int *rSet;   // R 배열
		int *sSet;  // S 배열
		int *number;     // [1~n] 순서 배열, 이 배열에서 숫자를 선택하여 S배열에 채운다

		rSet = new int[n];
		sSet = new int[n];
		number = new int[n];

		// input.txt로부터 R배열을 입력받으며, S배열을 채우게 될 number를 나열한다.
		for (int i = 0; i < n; i++)
		{
			fscanf(in, "%d", &rSet[i]);
			number[i] = i + 1;	
		}

		/*
		  만약 잘못된 R배열일 경우 -1을 return하여
		  "IMPOSSIBLE"을 출력한다. 이외의 성공한다면 S배열을 출력한다.
		*/
		if (OrderNum(rSet, sSet, number, n) <= -1)
			printf("IMPOSSIBLE\n");		
		else
			PrintNum(sSet, n);
		
		delete[] rSet;
		delete[] sSet;
		delete[] number;
	}
	fclose(in);

	return 0;
}

/*
 number배열의 선택된 숫자 이후의 숫자들을
 왼쪽으로 한칸씩 이동 시켜 준다
 */
int Lineup(int *number, int begin, int end){

	// number배열의 선택된 숫자 이후 숫자들을 한칸씩 왼쪽으로 이동시키는 for문이다
	for (int i = begin; i < end - 1; i++){
		number[i] = number[i + 1];
	}
	return 0;
}

/*
 R의 n번째부터 차례대로 number의 배열 인덱스 값에 대입하여 S의 값을 찾는다
*/
int OrderNum(int* rSet, int* sSet, int* number, int n){

	/*
	  n-1번째 부터 1까지 차례대로 R(rSet)을 검사하여 S(sSet)의
	  n-1번째 부터 1까지의 순서를 찾는 for문 이다.
	*/
	for (int i = n - 1; i >= 0; i--){
		/*
		  R(rSet) 의 값이 검사하는 순서의 값보다 크거나 같다면 
		  남은 숫자가 그 순서보다 크다는 것으로 오류이므로 -1을 반환한다.
		*/
		if (rSet[i] >= i + 1){
			return -1;
		}

		sSet[i] = number[rSet[i]];  // S의 i번째는 숫자 배열 number의 
		                            // R의 i번째가 인덱스 값으로 오는 숫자가 된다.

		Lineup(number, rSet[i], n--);  // number에서 선택된 값을 제외한 자리를 채운다
	}
	return 0;
}

/*
  모두 찾은 S배열을 출력한다
*/
int PrintNum(int *sSet, int n){

	// S배열을 차례대로 출력한다
	for (int i = 0; i < n; i++)
	{
		printf("%d ", sSet[i]);
	}
	printf("\n");

	return 0;
}
