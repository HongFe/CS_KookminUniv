#include "btree.h"

int CheckElementExist(struct  NODE* pNode, char key)   {
	/* 노드에 key가 있는지 검사 */
	int i;
	int bExist;            /* key가 존재하는지 여부  flag*/
	if (pNode == NULL)    return 0;
	bExist = 0;
	for (i = 0; i < DEGREE - 1; ++i)    {
		if (key == pNode->nElm[i]) {
			bExist = 1;
			break;
		}
	}
	return bExist;
}

int GetElementCount(struct  NODE*  pNode)  {
	/*  노드의 원소수 반환 */
	int i;
	int count;
	if (pNode == NULL) return 0;
	count = 0;
	for (i = 0; i<DEGREE - 1; ++i)     {
		if (pNode->nElm[i] >0) count++;
		else   break;
	}
	return count;
}

void PrintNodeElement(struct NODE* pNode)  {
	/* 노드의  key 값 출력  */
	int i;
	struct NODE* pChild;

	/* In-order로 출력 */
	for (i = 0; i < DEGREE; i++){
		if (pNode->pChild[i] != NULL){
			PrintNodeElement(pNode->pChild[i]);
		}
		if (pNode->nElm[i] != NULL){
			fprintf(pOutputStream, "%c ", pNode->nElm[i]);
		}
	}
	return;
}
int btree()   {/*  main */
	int i;
	int nPos;                 /* 삽입  위치를 저장하기 위한 임시 변수 */
	char nKey;                 /* 삽입 및 삭제를 해줄 key값  */
	int bFinished;          /* 삽입, 삭제가 일어났는지의 flag */
	int bFound;             /* B-트리에서  해당 key가 발견되었는지의 flag */
	int bOverflow;         /* 해당 node에 서  오버플로가  발생했는지의 flag  */
	int nCount;                     /* 해당 node의  key 수  */
	struct NODE* pRoot;                   /* 루트  node */
	struct NODE* pCurr;             /* 현재 작업수행 중인 node */
	struct NODE* pChild;            /* 현재 node의 child node  */
	struct NODE* pNewNode;     /* 생성할 새 node */
	struct NODE* plnkey;            /* in-key의 pointer */
	struct STACK_NODE* pStackTop;       /* stack 의 top pointer  */
	struct OVERFLOW_NODE*  pOverflow;  /*오버 플로의 경우를 처리 할 임시 node*/
	char nlnput[MAX_ELEMENTS];   /* 화일 접근을 줄이기 위해 배열에 저장 */
	int nElementCnt;
	int height = 0;				/* 트리의 높이 카운팅 */

	/*  초 기 화  */
	nKey = NULL; bFinished = 0; bFound = 0; bOverflow = 0; nCount = 0;
	for (i = 0; i < MAX_ELEMENTS; ++i)
		nlnput[i] = NULL;
	nElementCnt = 0;
	/* 공백 B-트리 생성 */
	pRoot = CreateNode();
	pRoot->bRoot = 1;
	height++;
	pCurr = pRoot;
	/* input.txt  화임 열기 */

	if ((pInputStream = fopen("input1.txt", "r")) == NULL)  {
		/* input 화일을 열수  없음 */
		printf("input.txt  file cannot be openedl\n");
		return -1;
	}
	/* output.txt  화일 열기 */
	if ((pOutputStream = fopen("output.txt", "w")) == NULL)  {
		/*  output 화일을 열수 없음  */
		printf("output.txt  file cannot be openedl\n");
		return -1;
	}
	/*  b-트리로  삽입  */
	printf("[B-Tree] Insertion Process started\n");
	pStackTop = NULL;
	while (1)  {
		nKey = NULL;
		if (fscanf(pInputStream, "%c", &nKey) == EOF){
			/* EOF에 도달 */
			break;
		}
		if (nKey == 32){
			fscanf(pInputStream, "%c", &nKey);
		}

		/* key값을 배열에 저장 */
		nlnput[nElementCnt] = nKey;
		nElementCnt++;
		pCurr = pRoot;
		/* stack 초 기화 */
		while (pStackTop && pStackTop->pCurr){
			pStackTop = PopFromStack(pStackTop);
		}
		pStackTop = CreateStackNode();

		pChild = NULL;
		printf("Key (%c) inserted \n", nKey);
		fprintf(pOutputStream, "Key (%c) inserted \n", nKey);
		do {
			nCount = GetElementCount(pCurr);
			if (nCount == 0){
				/*  root node는 초기애 key값이  0개 일 수  있음 */
				pChild = pCurr->pChild[0];
			}
			else if (CheckElementExist(pCurr, nKey) == 1){
				bFound = 1;
			}
			else if (nKey < pCurr->nElm[0])  {
				pChild = pCurr->pChild[0];
			}
			else if (nKey>pCurr->nElm[(nCount - 1 > 0) ? nCount - 1 : 0])  {
				pChild = pCurr->pChild[nCount];
			}
			else {
				/*  현재  node의 key의 중간에  위치해 야  함 */
				for (i = 0; i<nCount - 1; ++i)  {
					if (nKey  > pCurr->nElm[i]
						&& nKey < pCurr->nElm[i + 1]) {
						pChild = pCurr->pChild[i + 1];
						break;
					}
				}
			}
			if (pChild != NULL){
				pStackTop = PushlntoStack(pStackTop, pCurr);
				pCurr = pChild;
			}
		} while (!bFound && pChild != NULL);
		if (bFound == 1)  {
			printf("[lnsertion] Key (%c)  already exists in the B-Tree\n", nKey);
		}
		else {
			/*  key를 B-트리에 삽입 만약 plnkey가 존재하면
			In-key와 같이 취급 즉, 1 개의 원소 소유 */
			pChild = NULL;
			plnkey = NULL;
			bFinished = 0;

			do {
				nCount = GetElementCount(pCurr);
				if (plnkey)    nKey = plnkey->nElm[0];
				if (nCount  < DEGREE - 1)   /*  만원이 아닌 경 우*/   {
					/*  해당 위치에 바로 삽입 */
					if (nCount == 0) {
						nPos = 0;
					}
					else if (nKey < pCurr->nElm[0])  {
						nPos = 0;
					}
					else if (nKey  > pCurr->nElm[(nCount - 1>0) ? nCount - 1 : 0]) {
						nPos = nCount;
					}

					else {
						for (i = 0; i<nCount - 1; ++i){
							if (nKey > pCurr->nElm[i] &&
								nKey < pCurr->nElm[i + 1]) {
								nPos = i + 1;
								break;
							}
						}
					}
					/*  기존 key들을 shift */
					for (i = DEGREE - 2; i>nPos; --i){
						pCurr->nElm[i] = pCurr->nElm[i - 1];
						pCurr->pChild[i + 1] = pCurr->pChild[i];
					}
					/* 삽입 nKey가 삽입되는 경우는 리프뿐임 */
					pCurr->nElm[nPos] = nKey;
					pCurr->pChild[nPos + 1] = NULL;
					if (plnkey)  {
						pCurr->pChild[nPos] = plnkey->pChild[0];
						pCurr->pChild[nPos + 1] = plnkey->pChild[1];
					}
					bFinished = 1;
				}
				else {
					/* OVERFLOW_NODE에 복사 */
					pOverflow = CreateOverflowNode();
					for (i = 0; i < DEGREE - 1; ++i) {
						pOverflow->nElm[i] = pCurr->nElm[i];
						pOverflow->pChild[i] = pCurr->pChild[i];
					}
					pOverflow->pChild[DEGREE - 1] = pCurr->pChild[DEGREE - 1];
					/*  nKey 삽입  */

					nCount = DEGREE - 1;
					if (nKey  < pOverflow->nElm[0]){
						nPos = 0;
					}
					else if (nKey>pOverflow->nElm[(nCount - 1 > 0) ? nCount - 1 : 0]){
						nPos = nCount;
					}
					else {
						for (i = 0; i<nCount - 1; ++i){
							if (nKey>pOverflow->nElm[i]
								&& nKey < pOverflow->nElm[i + 1]){
								nPos = i + 1;
								break;
							}
						}
					}
					/*  기 존 key들을 shift */
					for (i = DEGREE - 1; i >= nPos; --i)  {
						pOverflow->nElm[i] = pOverflow->nElm[i - 1];
						pOverflow->pChild[i + 1] = pOverflow->pChild[i];
					}
					/* 삽입 */
					pOverflow->nElm[nPos] = nKey;
					pOverflow->pChild[nPos + 1] = NULL;
					if (plnkey)  {
						pOverflow->pChild[nPos] = plnkey->pChild[0];
						pOverflow->pChild[nPos + 1] = plnkey->pChild[1];
					}
					/*  중간값 구하기  */
					nPos = (DEGREE - 1) / 2;
					nKey = pOverflow->nElm[nPos];
					/*  오버 플로를  pCurr과 새  노드애   분할 */
					/* pCurr를 clear한 후  pOverflow의 1st half를 복사 */
					for (i = 0; i < DEGREE - 1; ++i)    {
						pCurr->nElm[i] = 0;
						pCurr->pChild[i] = NULL;
					}
					pCurr->pChild[DEGREE - 1] = NULL;


					for (i = 0; i < nPos; ++i)    {
						pCurr->nElm[i] = pOverflow->nElm[i];
						pCurr->pChild[i] = pOverflow->pChild[i];
					}
					pCurr->pChild[nPos] = pOverflow->pChild[nPos];
					/*  새노드에 2nd half를 복사 */
					pNewNode = CreateNode();
					for (i = nPos + 1; i < DEGREE; ++i)    {
						pNewNode->nElm[i - nPos - 1] = pOverflow->nElm[i];
						pNewNode->pChild[i - nPos - 1] = pOverflow->pChild[i];
					}
					pNewNode->pChild[DEGREE - nPos - 1] =
						pOverflow->pChild[DEGREE];
					/*  plnkey를 생성하여 key와 child를 삽입  */
					plnkey = CreateNode();
					plnkey->nElm[0] = nKey;
					plnkey->pChild[0] = pCurr;
					plnkey->pChild[1] = pNewNode;
					/*  위로 올라가며 삽입 반복 */
					if (pStackTop  && pStackTop->pCurr){
						/*  부모  node로 올려  보냄  */
						pCurr = PeepStackTop(pStackTop);
						pStackTop = PopFromStack(pStackTop);
					}
					else {
						/*  트리 레벨 증가 */
						/*  plnkey가 새 root가 됨  */
						pCurr->bRoot = 0;
						pRoot = plnkey;
						height++;
						plnkey->bRoot = 1;
						bFinished = 1;
					}
				}
			} while (bFinished == 0); //////////////
		}
		fprintf(pOutputStream, "[ Height of Tree : %d ] ", height);
		PrintNodeElement(pRoot);
		fprintf(pOutputStream, "  \n\n");

	}
	printf("[B-Tree] Insertion Process Ended\n");
	fclose(pInputStream);
}