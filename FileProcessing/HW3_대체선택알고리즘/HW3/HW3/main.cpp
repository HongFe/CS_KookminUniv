#include <stdio.h>
#include "Substitution.h"

using namespace std;

int main(){
	Substitution hw1,hw2(5),hw3(6);

	printf("m=4일때\n");
	hw1.replacementSelection();		// m값이 4일때 생성되는 런
	printf("\n\nm=5일때\n");
	hw2.replacementSelection();		// m값이 5일때 생성되는 런
	printf("\n\n\nm=6일때\n");
	hw3.replacementSelection();		// m값이 6일때 생성되는 런
	return 0;
}
