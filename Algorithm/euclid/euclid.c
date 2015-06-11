
/*///////////////////유클리드///////////////////////////////////


국민대학교 컴퓨터공학부 3학년 나홍철


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
