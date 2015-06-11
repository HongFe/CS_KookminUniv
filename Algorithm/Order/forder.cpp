/***************************************************************************************
*
*           ���� : ProblemF ����(order)
*
*                ���δ��б� ����������Ŵ��� ��ǻ�Ͱ��к� 3 �г�
*                �輺�� 20093267, �輼�� 20093268, ��ȫö 20093284
*
*****************************************************************************************
* ���� n���� ������ �� R�� n��° ����(������)�� 7�̶�� 1~n������ ������ ������ 
* 8�� Ȯ���ϴ�. �̴� S�� n��° ���ڸ� Ȯ���ϰ� ���� �� �ִٴ� ���̴�.
*
* �� ���� n-1 ���� 1~n������ ������ �̹� ���õ� 8�� ������ ���ڵ��� Ȯ����
* ���ڸ� ã�� �� �ִ�. 

* ��ó�� R�� n, n-1, n-2,,,3,2,1 ��° ���ڸ� �̿��Ͽ�
* S�� n, n-1 ,n-2,,,3,2,1 ���� ���ڸ� ���ʴ�� ã�� �� �ִ�.
*
* �� ������ �˰����� ��Ÿ������.
*
* �迭 number = [1~n] , R, S �� ���� �� ��,
*
* S[n-1] = number[R[n-1]] | number�� R[n-1]��° ���� ������ ���ڸ� �������� ��ĭ�� �̵� 
* S[n-2] = number[R[n-2]] | number�� R[n-2]��° ���� ������ ���ڸ� �������� ��ĭ�� �̵� 
* ....
* S[1] = number[R[1]] | number�� R[1]��° ���� ������ ���ڸ� �������� ��ĭ�� �̵� 
* S[0] = number[R[0]] | number�� R[0]��° ���� ������ ���ڸ� �������� ��ĭ�� �̵�
*                          (������ ���Ŀ� �������� �����Ƿ� �ƹ��͵� ��������)
****************************************************************************************/
#include <stdio.h>
#include <stdlib.h>

int Lineup(int *number, int begin, int end);
int OrderNum(int* rSet, int* sSet, int* number, int n);
int PrintNum(int *sSet, int n);

int main(void){
	FILE *in;
	int numTestCase;
	int n;  // R,S ������ ���� n

	in = fopen("input.txt", "r");
	if (in == NULL) exit(1);

	fscanf(in, "%d", &numTestCase);
	
	while (numTestCase--){

		fscanf(in, "%d", &n);
		int *rSet;   // R �迭
		int *sSet;  // S �迭
		int *number;     // [1~n] ���� �迭, �� �迭���� ���ڸ� �����Ͽ� S�迭�� ä���

		rSet = new int[n];
		sSet = new int[n];
		number = new int[n];

		// input.txt�κ��� R�迭�� �Է¹�����, S�迭�� ä��� �� number�� �����Ѵ�.
		for (int i = 0; i < n; i++)
		{
			fscanf(in, "%d", &rSet[i]);
			number[i] = i + 1;	
		}

		/*
		  ���� �߸��� R�迭�� ��� -1�� return�Ͽ�
		  "IMPOSSIBLE"�� ����Ѵ�. �̿��� �����Ѵٸ� S�迭�� ����Ѵ�.
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
 number�迭�� ���õ� ���� ������ ���ڵ���
 �������� ��ĭ�� �̵� ���� �ش�
 */
int Lineup(int *number, int begin, int end){

	// number�迭�� ���õ� ���� ���� ���ڵ��� ��ĭ�� �������� �̵���Ű�� for���̴�
	for (int i = begin; i < end - 1; i++){
		number[i] = number[i + 1];
	}
	return 0;
}

/*
 R�� n��°���� ���ʴ�� number�� �迭 �ε��� ���� �����Ͽ� S�� ���� ã�´�
*/
int OrderNum(int* rSet, int* sSet, int* number, int n){

	/*
	  n-1��° ���� 1���� ���ʴ�� R(rSet)�� �˻��Ͽ� S(sSet)��
	  n-1��° ���� 1������ ������ ã�� for�� �̴�.
	*/
	for (int i = n - 1; i >= 0; i--){
		/*
		  R(rSet) �� ���� �˻��ϴ� ������ ������ ũ�ų� ���ٸ� 
		  ���� ���ڰ� �� �������� ũ�ٴ� ������ �����̹Ƿ� -1�� ��ȯ�Ѵ�.
		*/
		if (rSet[i] >= i + 1){
			return -1;
		}

		sSet[i] = number[rSet[i]];  // S�� i��°�� ���� �迭 number�� 
		                            // R�� i��°�� �ε��� ������ ���� ���ڰ� �ȴ�.

		Lineup(number, rSet[i], n--);  // number���� ���õ� ���� ������ �ڸ��� ä���
	}
	return 0;
}

/*
  ��� ã�� S�迭�� ����Ѵ�
*/
int PrintNum(int *sSet, int n){

	// S�迭�� ���ʴ�� ����Ѵ�
	for (int i = 0; i < n; i++)
	{
		printf("%d ", sSet[i]);
	}
	printf("\n");

	return 0;
}
