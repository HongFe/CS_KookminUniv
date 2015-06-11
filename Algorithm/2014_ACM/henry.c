/***************************************************************************************
*
*           ���� : ACM_problemD(henry)
*
*                ���δ��б� ����������Ŵ��� ��ǻ�Ͱ��к� 3 �г�
*                �輺�� 20093267, �輼�� 20093268, ��ȫö 20093284
*
*****************************************************************************************
      
Ǯ�̹� : 
1. Xn�� ���ٸ� ������� ���ϱ����� ���� �м��� ������ ���ѵ� ���콺�Լ��� ���ϰ� 1�� ���Ͽ��ش�
2. ��� �����Ͽ� ���ڰ� 1�̵Ǵ¼��� ���
*/
#include <stdio.h>

int gcd(int a, int b){
   return (b==0) ? a : gcd(b,a%b);
}
int main()
{
   int numTestCases; // �׽�Ʈ ���̽��� ����
   int gcdTemp; // gcd�� �ӽ������� ����
   unsigned int Xn,top,down; //top ����, down �и� �������� 
   FILE* in;
   
   in = fopen("input.txt","r");

   fscanf(in,"%d",&numTestCases);

   while(numTestCases--)
   {
      fscanf(in,"%d %d",&top,&down);

      while(1)
      {
         gcdTemp = 1;
         if(top == 1) // ���ڰ� 1�̵Ǹ� �����Ͽ��ش�
            break;
         Xn = down/top + 1;   //Xn �� �������ؼ� 1�����ش�
         top = top * Xn - down; // a/b - c/d �� ad - bc
         down = down*Xn;
         gcdTemp = gcd(top,down); // gcd�� ���Ͽ��ش�
         if(gcdTemp > 0){ // �ʹ� ū���ڰ� ���ų� ������ ���ҽ� ������ �������°�쵵�ֱ⶧���� ������ �ٿ��ش�
            top = top/gcdTemp;
            down = down/gcdTemp;
         }
      }
      printf("%d\n",down);
      

      
   }


   return 0;
}