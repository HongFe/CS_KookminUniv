/***************************************************************************************
*
*           ���� : ACM_problemC(����̵� �׸���)
* 
*                ���δ��б� ����������Ŵ��� ��ǻ�Ͱ��к� 3 �г�
*                �輺�� 20093267, �輼�� 20093268, ��ȫö 20093284
*
*****************************************************************************************

1. �Ұ����� ���� ����.
2. ��� �Է°��� 4���� case�� ���� �� �ִ�.
  -Ȧ��XȦ��(m X n)
   (0,0)���� �����Ͽ� (0,1)->(1,1)->(1,2)->(0,2)->(0,3)...(0,n-1)���� �̵��ѵ�
   (m-1,n-1)�� �̵��Ͽ� (m-1,n-2)->(m-1,n-3)...(m-1,0)���� �̵� ��
   (m-2,0)���� �̵��Ͽ� (m-2,1)->(m-2,2)...(m-2,n-1)�̷������� �ݺ��Ͽ�
   (1,0)�� �����Ѵ�.
 
  -Ȧ��X¦��
   (0,0)���� �����Ͽ� (0,n-1)���� �̵� ��
   (0,n-1)���� (m-1,n-1)���� �̵� ��
   (m-1,n-2)���� (1,n-2)���� �̵� ��
   (1,n-3)���� (m-1,n-3)���� �̵� ������ �ݺ��Ͽ� 
   (1,0)�� �����Ѵ�.
   
  -¦��XȦ��
   (0,0)���� �����Ͽ� (0,n-1)���� �̵� ��
   (m-1,n-1)���� �����Ͽ� (1,n-1)���� �̵� ��
   (1,n-2)���� �����Ͽ� (m-1,n-2)���� �̵��� �ݺ��Ͽ� 
   (1,0)�� �����Ѵ�.
  -¦��X¦��
   Ȧ��X¦���� Ǯ�̹��� �����ϴ�.
   
   */

#include <stdio.h>

int main(int argc, const char * argv[])
{    
    FILE *in;
    int numTestCases;
    int m,n;	// ����̵� �׸����� ��(m)�� ��(n) �̴�.
    int x,y;	// �˻��ϴ� ��(x)�� ��(n) �̴�.
    int i,j;
    in = fopen("input.txt","r");
    fscanf(in, "%d" ,&numTestCases);
    
    
    while (numTestCases--) {
        fscanf(in,"%d %d",&m,&n);
        x = y = 0;	// �˻� �� ��� ���� ���� �ʱ�ȭ ���ش�.

        printf("1\n");	// �Ұ����� ���� �����Ƿ� 
						//������ ������ �˷��ִ� '1'�� ����Ѵ�.
   
		// CASE1 : Ȧ��XȦ�� Toroidal Grid
        if(m%2 == 1 && n%2 == 1) 
        {
			/* (0,0)���� �����Ͽ� (0,1)->(1,1)->(1,2)->(0,2)->(0,3)...(0,n-1) ����
			�̵������ ����Ѵ�. */
            while (1)  { 
                printf("(%d,%d)\n",x,y);
             
                if (y % 2 == 0 && x % 2 == 0) y++;
                else if (y % 2 == 1 && x % 2 == 0) x++;
                else if (y % 2 == 1 && x % 2 == 1) y++;
                else if (y % 2 == 0 && x % 2 == 1) x--;
				
				/* (0,n-1)�� ������ �Ѵٸ� ��� ���� ���� ����ϰ� 
				�ݺ����� ���� (m-1,n-1) �̵��Ѵ�. */
                if (x == 0 && y == n-1)
                {
                    printf("(%d,%d)\n",x,y);
                    break;
                }
            }
            x = m-1; // ������ �Ʒ� 
            y = n-1; // �𼭸����� Ž���� �����Ѵ�
            
			/* (m-1,n-1)�� �̵��Ͽ� (m-1,n-2)->(m-1,n-3)...(m-1,0)���� �̵���θ� ��� ��
			 (m-2,0)���� �̵��Ͽ� (m-2,1)->(m-2,2)...(m-2,n-1)������ �̵���θ� ����Ѵ�.
			 �̸� �ݺ��Ͽ� (1,0)�� �����Ѵ�. */
            while(1)
            {
				printf("(%d,%d)\n",x,y);
				if(x == 1 && y == 0) break; // (1,0)�� ���� �ϸ� �����Ѵ�.
                if(x % 2 == 0 && y != 0) y--;
                else if(x % 2 == 1 && y != n-1) y++;
                else if(y == 0 || y == n-1) x--;
            }
        }
		
		// CASE2 : Ȧ��X¦�� Toroidal Grid
        else if(m%2 == 1 && n%2 == 0) 
        {
			/* (0,0)���� �����Ͽ� (0,n-1)���� �̵���θ� ����Ѵ�.  */
            while (1) { 
                printf("(%d,%d)\n",x,y);
                y++;
                if(y == n-1) break;
            }
			/* (0,n-1)���� (m-1,n-1)���� �̵���θ� ����Ѵ�. */
            while (1) { 
                printf("(%d,%d)\n",x,y);
                x++;
                if(x == m-1) break;
            }
			/* (m-1,n-2)���� (1,n-2)���� �̵���θ� ��� ��
			(1,n-3)���� (m-1,n-3)���� �̵���� ����� �ݺ��Ͽ� 
			(1,0)�� ���� �Ѵ�.*/
            while (1) { \
                printf("(%d,%d)\n",x,y);
                if (x == 1 && y ==0) break;
                if (x == m-1){
                    y--;
                    printf("(%d,%d)\n",x,y);
                    x--;
                }
                else if(x == 1)
                {
                    y--;
                    printf("(%d,%d)\n",x,y);
                    x++;
                }
                else if(y % 2 == 0) x--;
                else if(y % 2 == 1) x++;
            }

        }
		// CASE3 : ¦��XȦ�� Toroidal Grid
        else if(m%2 == 0 && n%2 == 1)
        {
			/* (0,0)���� �����Ͽ� (0,n-1)���� �̵���θ� ����Ѵ�. */   
            while (1) { 
                
                printf("(%d,%d)\n",x,y);
                if(y == n-1) break;
                y++;
            }
            x = m-1; // ������ �Ʒ� 
            y = n-1; // �𼭸��� �̵��Ѵ�.
			
            /* (m-1,n-1)���� �����Ͽ� (1,n-1)���� �̵���θ� ��� ��
			(1,n-2)���� �����Ͽ� (m-1,n-2)���� �̵���θ� ����ϴ�.
			���� ������ �ݺ��Ͽ� (1,0)�� �����Ѵ�. */
            while (1) {
                printf("(%d,%d)\n",x,y);
                if (x == 1 && y ==0) break;
                if(x == m-1 && y == n-1) x--;
                else if(y % 2 == 0 && x != 1) x--;
                else if(x == 1){
                    y--;
                    printf("(%d,%d)\n",x,y);
                    x++;
                }
                else if(y % 2 == 1 && x != m-1)x++;
                else{
                    y--;
                    printf("(%d,%d)\n",x,y);
                    x--;
                }
            }
        }
		// CASE4 : ¦�� X ¦�� Grid (= Ȧ�� X ¦��)
        else if(m%2 == 0 && n%2 == 0)
        {
			/* (0,0)���� �����Ͽ� (0,n-1)���� �̵���θ� ����Ѵ�.  */
            while (1) { 
                printf("(%d,%d)\n",x,y);
                y++;
                if(y == n-1) break;
            }
			/* (0,n-1)���� (m-1,n-1)���� �̵���θ� ����Ѵ�. */
            while (1) { 
                printf("(%d,%d)\n",x,y);
                x++;
                if(x == m-1) break;
            }
            
			/* (m-1,n-2)���� (1,n-2)���� �̵���θ� ��� ��
			(1,n-3)���� (m-1,n-3)���� �̵���� ����� �ݺ��Ͽ� 
			(1,0)�� ���� �Ѵ�.*/
            while (1) {
                printf("(%d,%d)\n",x,y);
                if (x == 1 && y ==0) break;
                if (x == m-1){
                    y--;
                    printf("(%d,%d)\n",x,y);
                    x--;
                }
                else if(x == 1)
                {
                    y--;
                    printf("(%d,%d)\n",x,y);
                    x++;
                }
                else if(y % 2 == 0) x--;
                else if(y % 2 == 1) x++;     
            }
      }
    }
    return 0;
}