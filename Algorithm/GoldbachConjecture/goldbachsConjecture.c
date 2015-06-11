/***************************************************************************************
*
*	문제 : Problem E - Goldbach's Conjecture
*
*	국민대학교 전자정보통신대학 컴퓨터공학부 3 학년
*	김성근 20093267, 김세훈 20093268, 나홍철 20093284
*
*	짝수가 들어오게 되면 그 짝수에 반 만큼만 for문을 돌면서
*	해당 수가 소수라면 그 소수를 짝수에서 뺀것이 소수이면서
*	최소값인지 확인하는 알고리즘으로 작성하였습니다.
*
*	for문이 반복됨에 따라서 시간복잡도 크긴하지만
*	n은 1000이하라는 제한 조건이 있음으로 크게 문제가
*	되지 않는 다고 판단하고 알고리즘을 작성하였습니다.
*
*****************************************************************************************/
#include <stdio.h>

#define TRUE 1
#define FALSE 0

/**
*	Parameter로 넘어온 value값이 소수인지 아닌지 판별하는 함수
*	@param
*		소수를 판단할 value
*	@return
*		소수라면 TRUE, 소수가 아니라면 FALSE
*/
int isPrime(int value){
	int i;

	if(value == 1 || value == 2 || value == 3){ // value가 1,2,3이라면 소수임으로 return TRUE
		return TRUE;
	}

	for(i=2; i<value;i++){ // 2 부터 해당 value값 까지 나누어 보면서 소수인지 판단하고
		if(value%i == 0) return FALSE; // 나누어진다면 소수가 아님으로 FALSE 리턴
	}
	return TRUE; // 나누어지는 수가 없었다면 소수가 아님으로 TRUE 리턴
}

/**
*	짝수가 소수의 덧셈으로 되는지 확인하는 함수
*	
*	@param
*		짝수 값
*/
void conjecture(int even){
	int i, sub;
	int first, second, abs, min = 0x7FFFFFFF;

	for(i=2; i<(even/2+1); i++){ // 해당수를 반으로 나눈 값보다 1을 더한만틈 for문을 돈다 : 짝수가 10이라면 2~5까지
		if(isPrime(i)){ // 그 수가 소수라면
			sub = even - i; // 짝수에서 소수 값을 뺀것이
			if(isPrime(sub) && min > (sub - i) ){ // 소수이면서 현재 최소값보다 작으면 해당수 적용
				first = i, second = sub;
				min = second - first;
			}
		}
	}

	printf("%d %d\n", first, second);
}

int main(void){
	FILE *in;
	int iTestCase;
	int even;

	in = fopen("input.txt", "r");
	fscanf(in, "%d", &iTestCase);

	while(iTestCase--){
		// ~ init variabel
		fscanf(in, "%d", &even);
		conjecture(even);
	}

	return 0;
}