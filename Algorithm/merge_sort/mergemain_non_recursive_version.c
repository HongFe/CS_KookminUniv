/***************************************************************************************
*
*           문제 : Non-recursive Merge sort
*
*                국민대학교 전자정보통신대학 컴퓨터공학부 3 학년
*												나홍철 20093284
*
*****************************************************************************************
* 설명 :
*	 메모리 주소를 다루는 Merge Sorting이다. 2 Merge Set간 내부 데이터를 비교하면서 작은 값을
*	교체하여 준다. Merge Sorting이 한번 씩 진행될 때마다 정렬된 데이터는 2개, 4개, 8개,, 2*n 씩 된다.
*	임시 버퍼에 내용을 모두 저장하고, 두 Merge Set의 각 원소를 비교하여 작은 값을 실제 데이터 주소에 저장한다.
*	한쪽 Merge Set의 데이터들이 모두 비교되면 다른 한 쪽 Merge Set을 차례차례 실제 데이터 주소에
*	데이터를 카피한다.
*/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_LENGTH 120	/* 입력받는 최대 정수 갯수  */
#define MAX_BUF 1000	/* 임시로 저장받는 버퍼의 사이즈 */

/*
*	MERGE SORT
*	[변수 설명] 
*	base_ptr : 레지스터에 저장되는 변수로 void형으로 받은 것의 주소다 
*	bPtr : 실제 데이터에 저장될 Set의 처음 위치
*	range : 
*	baseEnd : 실제 데이터의 전체 사이즈이면서 시작주소에 더하면 마지막 위치가 된다.
*	movePtr : 실제 데이터에 저장할 위치
*	tmp : 실제 데이터를 비교하기위해 임시로 저장하는 버퍼
*	tmpLast : 임시 버퍼의 마지막 위치로 진행되는 Merge과정에서 상한선이 된다.
*	tmpLeft : 임시 버퍼의 처음 주소다
*	rightPtr : 임시 버퍼에서 Merge Set의 오른쪽 시작 주소다.
*	rightEnd : 임시 버퍼에서 Merge Set의 오른쪽 끝 주소다.
*	beginL, *beginR : 비교할 두 Merge Set의 각 시작주소다.
*/
int msort(void *base, size_t nel, size_t width, int(*compare)(const void *, const void *))
{
	register char *base_ptr = (char *)base;	/* 레지스터에 저장되는 변수로 void형으로 받은 것의 주소다 */
	char *bPtr;
	int range;
	int baseEnd = nel*width;
	char *movePtr;
	char tmp[MAX_BUF];
	char *tmpLast;
	char *tmpLeft, *rightPtr, *rightEnd;
	char *beginL, *beginR;

	/* sorting 할 것이 존재하지 않는다면 나간다. */
	if (nel == 0)
		return 0;
	else
	{
		/* Merge Set의 사이즈를 결정한다. ex) 1 , 2 ,4 ,8..이 된다. */
		for (range = width; range < baseEnd; range *= 2)
		{
			memcpy(tmp, base_ptr, baseEnd);	/* 실제 데이터를 임시 버퍼에 저장한다. */
			tmpLeft = tmp;
			tmpLast = tmpLeft + baseEnd;

			/* Merge 할 Set을 다음 set으로 옮긴다.  */
			for (bPtr = base_ptr; bPtr < base_ptr + baseEnd; bPtr += (range * 2), tmpLeft += (range * 2))
			{
				movePtr = bPtr;				
				rightPtr = tmpLeft + range;
				rightEnd = rightPtr + range;

				beginL = tmpLeft;
				beginR = rightPtr;

				/* 상한선을 넘지 않도록 한다. */
				if (rightEnd >  tmpLast)
					rightEnd = tmpLast;

				/* 두 Set의 범위는 각자의 상한선을 넘지 않는다. */
				while (beginL < rightPtr && beginR < rightEnd)
				{
					/* 각 Set의 원소를 비교한다. */
					if ((*compare) ((void *)beginL, (void *)beginR) < 0)
					{	
						memcpy(movePtr, beginL, width);
						movePtr += width;
						beginL += width;
					}
					else
					{
						memcpy(movePtr, beginR, width);
						movePtr += width;
						beginR += width;
					}
				}
				/* 임시버퍼의 잔여 원소들을 실제 데이터에 셋팅한다. */
				while (beginL < rightPtr)
				{
					memcpy(movePtr, beginL, width);
					movePtr += width;
					beginL += width;
				}
				/* 임시버퍼의 잔여 원소들을 실제 데이터에 셋팅한다. */
				while (beginR < rightEnd)
				{
					memcpy(movePtr, beginR, width);
					movePtr += width;
					beginR += width;
				}
			}

		}
	}
	return 0;
}

/*
*	변수끼리 비교하는 함수다.
*/
int icompare(const void *a, const void *b)
{
	return *(int*)a-*(int*)b;
}

int main()
{
	FILE *inputFile;
	int testCase,numCount;
	int ints[MAX_LENGTH];
	int i;

	inputFile = fopen("input.txt", "r");
	fscanf(inputFile, "%d", &testCase);
	while (testCase--)
	{
		fscanf(inputFile, "%d", &numCount);
		for (i = 0;i<numCount;i++)
			fscanf(inputFile, "%d", &ints[i]);
		msort(ints, numCount, sizeof(int), icompare);
		for (i = 0; i<numCount ; i++)
		{
			printf("%d ", ints[i]);
		}
		printf("\n");
	}
	return 0;
}

