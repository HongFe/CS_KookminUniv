#include <stdio.h>
#include <stdlib.h>

void main(void){

	FILE *wfp;
	wfp=fopne("input.txt", "w");
	for (int i = 0; i < MAX; i++)
		fscanf(wfp,"%d ", i);


}