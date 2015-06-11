/***************************************************************************************
*
*	���� : Problem A - Ducci Sequence
*
*	���δ��б� ����������Ŵ��� ��ǻ�Ͱ��к� 3 �г�
*	�輺�� 20093267, �輼�� 20093268, ��ȫö 20093284
*
*	�ش� �˰����� ������ ���� ����� �״�� ������Ѽ� �ذ��Ͽ����ϴ�.
*	�ϴ� Loop �� �ִ� 1000���� �����Ѵٰ� ������ �����ֱ� ������ ����������
*	�ش� Loop �� ����� ������ �Ѷ��ξ� �����Ͽ� ����ϵ��� �Ͽ����ϴ�.
*
*	�׸��� �������� 0�̳����� �۾��� sequence�� �����ϴ� �Լ����� �ٷιٷ�
*	�� ���� �����Ͽ� Ȯ���ϵ��� �Ͽ� �� ���� 0�̳����� �ٷ� ��������
*	�˰����� �����Ͽ����ϴ�
*
*****************************************************************************************/
#include <stdio.h>
#include <string.h>

// ~ ��� ����
#define TRUE 1	// c++ �� �ƴ� C�� �ۼ��߱� ������ TRUE, FALSE ����� ����
#define FALSE 0

#define MIN_SIZE 3 // �� Test Case�� �Է¹޴� Size�� ũ���� �ּҰ��� �ִ밪
#define MAX_SIZE 15
#define MAX_LOOP_SIZE 1000 // �� Test Case�� ����� �ÿ� �ִ�� ���ư��� LOOP Ƚ��

int loop[MAX_LOOP_SIZE][MAX_SIZE] = {0}; // n-tuple�� ����صα� ���� ���

/**
*	���� �ʱ⿡ input.txt�� ���� �Է��� ���� �Լ�.
*	@param *in
*		FILE
*	@param *ducci
*		file�� ���� �����͸� �޾ư� ��� 
*	@param *size
*		�Է� ���� n�� ũ��
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
*	Debug�� ���Ͽ� �迭�� ����ϴ� �Լ�
*
*	@param *arr
*		print�� ��Ű�� ���� �迭
*	@param *size
*		�迭�� ũ��
*/
void print(int *arr, int *size){
	int i;
	for(i=0; i<*size; i++){
		printf("%d ", arr[i]);
	}
	printf("\n");
}

/**
*	���밪�� �������ִ� �Լ�
*
*	@param value
*		���� ( ��, �� �������)
*	@return
*		���밪 ����
*/
int abs(int value){
	return value < 0 ? -value : value;
}

/**
*	Ducci Sequence�� ������ �Լ�
*
*	@param *ducci
*		sequecnce�� ������ �迭
*	@param *next
*		sequence�� ����� �޾ư� �迭
*	@param *size
*		�迭�� ũ��
*	@return
*		sequece�� ��� ������ ���� ����
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
		while(TRUE){ // sum 0�� �����ų�, �ݺ��Ǵ� ��찡 ���ö����� ������ loop
			if(sequence(ducci, next, &size) == 0)// sum == 0�̸� break
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

			if(!result) // ������ loop�� ���������� break ����
				break;

			// ~ copy array
			memcpy(*(loop+(count++)), next, sizeof(int)*size); // �޸� ī�Ǹ� ���Ͽ� �迭�� ����
			memcpy(ducci, next, sizeof(int)*size); // ���� sequence�� �����ϱ� ���� next�� �ִ����� ducci�� �̵�
		}

		if(result)
			printf("ZERO\n");
		else
			printf("LOOP\n");
	}

	return 0;
}