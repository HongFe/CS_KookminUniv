/***************************************************************************************
*
*           ���� : Non-recursive Merge sort
*
*                ���δ��б� ����������Ŵ��� ��ǻ�Ͱ��к� 3 �г�
*												��ȫö 20093284
*
*****************************************************************************************
* ���� :
*	 �޸� �ּҸ� �ٷ�� Merge Sorting�̴�. 2 Merge Set�� ���� �����͸� ���ϸ鼭 ���� ����
*	��ü�Ͽ� �ش�. Merge Sorting�� �ѹ� �� ����� ������ ���ĵ� �����ʹ� 2��, 4��, 8��,, 2*n �� �ȴ�.
*	�ӽ� ���ۿ� ������ ��� �����ϰ�, �� Merge Set�� �� ���Ҹ� ���Ͽ� ���� ���� ���� ������ �ּҿ� �����Ѵ�.
*	���� Merge Set�� �����͵��� ��� �񱳵Ǹ� �ٸ� �� �� Merge Set�� �������� ���� ������ �ּҿ�
*	�����͸� ī���Ѵ�.
*/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_LENGTH 120	/* �Է¹޴� �ִ� ���� ����  */
#define MAX_BUF 1000	/* �ӽ÷� ����޴� ������ ������ */

/*
*	MERGE SORT
*	[���� ����] 
*	base_ptr : �������Ϳ� ����Ǵ� ������ void������ ���� ���� �ּҴ� 
*	bPtr : ���� �����Ϳ� ����� Set�� ó�� ��ġ
*	range : 
*	baseEnd : ���� �������� ��ü �������̸鼭 �����ּҿ� ���ϸ� ������ ��ġ�� �ȴ�.
*	movePtr : ���� �����Ϳ� ������ ��ġ
*	tmp : ���� �����͸� ���ϱ����� �ӽ÷� �����ϴ� ����
*	tmpLast : �ӽ� ������ ������ ��ġ�� ����Ǵ� Merge�������� ���Ѽ��� �ȴ�.
*	tmpLeft : �ӽ� ������ ó�� �ּҴ�
*	rightPtr : �ӽ� ���ۿ��� Merge Set�� ������ ���� �ּҴ�.
*	rightEnd : �ӽ� ���ۿ��� Merge Set�� ������ �� �ּҴ�.
*	beginL, *beginR : ���� �� Merge Set�� �� �����ּҴ�.
*/
int msort(void *base, size_t nel, size_t width, int(*compare)(const void *, const void *))
{
	register char *base_ptr = (char *)base;	/* �������Ϳ� ����Ǵ� ������ void������ ���� ���� �ּҴ� */
	char *bPtr;
	int range;
	int baseEnd = nel*width;
	char *movePtr;
	char tmp[MAX_BUF];
	char *tmpLast;
	char *tmpLeft, *rightPtr, *rightEnd;
	char *beginL, *beginR;

	/* sorting �� ���� �������� �ʴ´ٸ� ������. */
	if (nel == 0)
		return 0;
	else
	{
		/* Merge Set�� ����� �����Ѵ�. ex) 1 , 2 ,4 ,8..�� �ȴ�. */
		for (range = width; range < baseEnd; range *= 2)
		{
			memcpy(tmp, base_ptr, baseEnd);	/* ���� �����͸� �ӽ� ���ۿ� �����Ѵ�. */
			tmpLeft = tmp;
			tmpLast = tmpLeft + baseEnd;

			/* Merge �� Set�� ���� set���� �ű��.  */
			for (bPtr = base_ptr; bPtr < base_ptr + baseEnd; bPtr += (range * 2), tmpLeft += (range * 2))
			{
				movePtr = bPtr;				
				rightPtr = tmpLeft + range;
				rightEnd = rightPtr + range;

				beginL = tmpLeft;
				beginR = rightPtr;

				/* ���Ѽ��� ���� �ʵ��� �Ѵ�. */
				if (rightEnd >  tmpLast)
					rightEnd = tmpLast;

				/* �� Set�� ������ ������ ���Ѽ��� ���� �ʴ´�. */
				while (beginL < rightPtr && beginR < rightEnd)
				{
					/* �� Set�� ���Ҹ� ���Ѵ�. */
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
				/* �ӽù����� �ܿ� ���ҵ��� ���� �����Ϳ� �����Ѵ�. */
				while (beginL < rightPtr)
				{
					memcpy(movePtr, beginL, width);
					movePtr += width;
					beginL += width;
				}
				/* �ӽù����� �ܿ� ���ҵ��� ���� �����Ϳ� �����Ѵ�. */
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
*	�������� ���ϴ� �Լ���.
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

