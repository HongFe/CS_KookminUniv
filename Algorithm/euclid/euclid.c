
/*///////////////////��Ŭ����///////////////////////////////////


���δ��б� ��ǻ�Ͱ��к� 3�г� ��ȫö


//////////////////////////////////////////////////////////*/
#include <stdio.h>
#include <stdlib.h>

int gcd(int a, int b){
	if (b == 0)
		return a;
	return gcd(b, a%b);
}

void main(void){
	FILE *in;
	int numTestCase;
	int a, b;
	int tmp;

	in = fopen("input.txt", "r");
	if (in == NULL) exit(1);

	fscanf(in, "%d", &numTestCase);

	while (numTestCase--){

		fscanf(in, "%d %d", &a, &b);

		printf("%d\n", gcd(a, b));
	}
	fclose(in);
}
