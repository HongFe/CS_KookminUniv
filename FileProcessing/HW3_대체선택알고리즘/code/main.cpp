#include <stdio.h>
#include "Substitution.h"

using namespace std;

int main(){
	Substitution hw1,hw2(5),hw3(6);

	printf("m=4�϶�\n");
	hw1.replacementSelection();		// m���� 4�϶� �����Ǵ� ��
	printf("\n\nm=5�϶�\n");
	hw2.replacementSelection();		// m���� 5�϶� �����Ǵ� ��
	printf("\n\n\nm=6�϶�\n");
	hw3.replacementSelection();		// m���� 6�϶� �����Ǵ� ��
	return 0;
}
