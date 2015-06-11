/***************************************************************************************
*
*	���� : Problem E - Goldbach's Conjecture
*
*	���δ��б� ����������Ŵ��� ��ǻ�Ͱ��к� 3 �г�
*	�輺�� 20093267, �輼�� 20093268, ��ȫö 20093284
*
*	¦���� ������ �Ǹ� �� ¦���� �� ��ŭ�� for���� ���鼭
*	�ش� ���� �Ҽ���� �� �Ҽ��� ¦������ ������ �Ҽ��̸鼭
*	�ּҰ����� Ȯ���ϴ� �˰������� �ۼ��Ͽ����ϴ�.
*
*	for���� �ݺ��ʿ� ���� �ð����⵵ ũ��������
*	n�� 1000���϶�� ���� ������ �������� ũ�� ������
*	���� �ʴ� �ٰ� �Ǵ��ϰ� �˰����� �ۼ��Ͽ����ϴ�.
*
*****************************************************************************************/
#include <stdio.h>

#define TRUE 1
#define FALSE 0

/**
*	Parameter�� �Ѿ�� value���� �Ҽ����� �ƴ��� �Ǻ��ϴ� �Լ�
*	@param
*		�Ҽ��� �Ǵ��� value
*	@return
*		�Ҽ���� TRUE, �Ҽ��� �ƴ϶�� FALSE
*/
int isPrime(int value){
	int i;

	if(value == 1 || value == 2 || value == 3){ // value�� 1,2,3�̶�� �Ҽ������� return TRUE
		return TRUE;
	}

	for(i=2; i<value;i++){ // 2 ���� �ش� value�� ���� ������ ���鼭 �Ҽ����� �Ǵ��ϰ�
		if(value%i == 0) return FALSE; // ���������ٸ� �Ҽ��� �ƴ����� FALSE ����
	}
	return TRUE; // ���������� ���� �����ٸ� �Ҽ��� �ƴ����� TRUE ����
}

/**
*	¦���� �Ҽ��� �������� �Ǵ��� Ȯ���ϴ� �Լ�
*	
*	@param
*		¦�� ��
*/
void conjecture(int even){
	int i, sub;
	int first, second, abs, min = 0x7FFFFFFF;

	for(i=2; i<(even/2+1); i++){ // �ش���� ������ ���� ������ 1�� ���Ѹ�ƴ for���� ���� : ¦���� 10�̶�� 2~5����
		if(isPrime(i)){ // �� ���� �Ҽ����
			sub = even - i; // ¦������ �Ҽ� ���� ������
			if(isPrime(sub) && min > (sub - i) ){ // �Ҽ��̸鼭 ���� �ּҰ����� ������ �ش�� ����
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