/***************************************************************************************
*
*           문제 : ACM_problemA(hotel)
*
*                국민대학교 전자정보통신대학 컴퓨터공학부 3 학년
*                김성근 20093267, 김세훈 20093268, 나홍철 20093284
*
*****************************************************************************************
풀이법 : 
1. 층수는 손님이 들어오는 순서를 총 층수로 나눈 나머지가 층수이다.
1.1 만약 나머지가 0일때는 가장 꼭대기 층에 방을 배정한다. 
2. 호수는 순서를 층수로 나눈 몫이다.
*/

#include <stdio.h>

int main()
{
	int numTestCases; // 테스트 케이스의 갯수
	int height,width,order; // height 높이, width 방갯수, order 손님이 온 순서
	FILE* in;

	in = fopen("input.txt","r");

	fscanf(in,"%d",&numTestCases);

	while(numTestCases--)
	{
		fscanf(in,"%d %d %d",&height,&width,&order);
		/* 만약 들어오는 순서를 호텔의 높이로 나누었을때 
		나머지가 없다면 꼭대기 층에 배정이 된다. */
		if(order%height != 0)
			printf("%d%.2d\n",order%height,order/height+1); // N번째 손님에게 배정 되는 방번호를 출력한다
		/* 꼭대기 층에 배정이 되지 않는다. */
		else 
			printf("%d%.2d\n",height,order/height); // N번째 손님에게 배정 되는 방번호를 출력한다
	}
	return 0;
}