/***************************************************************************************
*
*           문제 : ACM_problemD(henry)
*
*                국민대학교 전자정보통신대학 컴퓨터공학부 3 학년
*                김성근 20093267, 김세훈 20093268, 나홍철 20093284
*
*****************************************************************************************
      
풀이법 : 
1. Xn을 별다른 연산없이 구하기위해 원래 분수에 역수를 취한뒤 가우스함수를 취하고 1을 더하여준다
2. 계속 연산하여 분자가 1이되는순간 출력
*/
#include <stdio.h>

int gcd(int a, int b){
   return (b==0) ? a : gcd(b,a%b);
}
int main()
{
   int numTestCases; // 테스트 케이스의 갯수
   int gcdTemp; // gcd를 임시저장할 변수
   unsigned int Xn,top,down; //top 분자, down 분모 문제에서 
   FILE* in;
   
   in = fopen("input.txt","r");

   fscanf(in,"%d",&numTestCases);

   while(numTestCases--)
   {
      fscanf(in,"%d %d",&top,&down);

      while(1)
      {
         gcdTemp = 1;
         if(top == 1) // 분자가 1이되면 리턴하여준다
            break;
         Xn = down/top + 1;   //Xn 은 역수취해서 1더해준다
         top = top * Xn - down; // a/b - c/d 는 ad - bc
         down = down*Xn;
         gcdTemp = gcd(top,down); // gcd를 구하여준다
         if(gcdTemp > 0){ // 너무 큰숫자가 들어가거나 연산을 못할시 음수를 가져오는경우도있기때문에 조건을 붙여준다
            top = top/gcdTemp;
            down = down/gcdTemp;
         }
      }
      printf("%d\n",down);
      

      
   }


   return 0;
}