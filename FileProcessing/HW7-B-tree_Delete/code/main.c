#include "main.h"

struct NODE* CreateNode(){
	/* ��� ����  */
	struct NODE* pNode;
	pNode = (struct NODE*)malloc(sizeof(struct NODE));
	memset(pNode, 0, sizeof(struct  NODE));
	return pNode;
}
struct OVERFLOW_NODE* CreateOverflowNode()  {
	/* �����÷� ��� ���� */
	struct OVERFLOW_NODE*  pNode;
	pNode = (struct OVERFLOW_NODE*)malloc(sizeof(struct OVERFLOW_NODE));
	memset(pNode, 0, sizeof(struct OVERFLOW_NODE));
	return pNode;
}

struct STACK_NODE* CreateStackNode(){
	/* Stack Node �� ��   */
	struct STACK_NODE*  pStackNode;
	pStackNode = (struct  STACK_NODE*)malloc(sizeof(struct  STACK_NODE));
	memset(pStackNode, 0, sizeof(struct STACK_NODE));
	return pStackNode;
}

struct STACK_NODE* PushlntoStack
	/*	Stack�� push  */
	(struct  STACK_NODE*  pStackTop, struct NODE*  pNode)   {
	struct STACK_NODE* pNewTop;
	if (pStackTop == NULL || pNode == NULL){
		return NULL;
	}

	if (pStackTop->pCurr == NULL){
		/* top�� ��� �ִ°�� */
		pStackTop->pCurr = pNode;
		pStackTop->pPrev = NULL;
		return pStackTop;
	}
	else {
		/* �� top-��  ����   */
		pNewTop = CreateStackNode();
		pNewTop->pPrev = (void*)pStackTop;
		pNewTop->pCurr = pNode;
		return pNewTop;
	}
}

struct STACK_NODE*  PopFromStack(struct   STACK_NODE* pStackTop)  {
	/* Stack�� �� pop */
	struct STACK_NODE* pPrevTop;
	struct NODE* pNode;
	if (pStackTop == NULL) return NULL;
	if (pStackTop->pCurr == NULL) return NULL;
	else if (pStackTop->pPrev == NULL)   {
		/* ���� top�� */
		pNode = pStackTop->pCurr;
		pStackTop->pCurr = NULL;
		return pStackTop;
	}
	else {
		/* pCurr�� pPrev ��� ���� */
		pNode = pStackTop->pCurr;
		pPrevTop = pStackTop;
		pStackTop = (struct STACK_NODE*)  pStackTop->pPrev;
		/* �� ��  top ����   */
		free((void*)pPrevTop);
		return pStackTop;
	}
}
struct NODE* PeepStackTop(struct   STACK_NODE*  pStackTop)  {
	/* Stack��  top�� peep */
	if (pStackTop == NULL)    return NULL;
	return pStackTop->pCurr;
}
int CheckElementExist(struct  NODE* pNode, char key)   {
	/* ��忡 key�� �ִ��� �˻� */
	int i;
	int bExist;            /* key�� �����ϴ��� ����  flag*/
	if (pNode == NULL)    return 0; /* ��尡 ���� ������ 0 ���� */
	bExist = 0;
	for (i = 0; i < DEGREE - 1; ++i)    {
		if (key == pNode->nElm[i]) {
			/* ��忡 ã�� key���� �����Ѵٸ� */
			bExist = 1;
			break;
		}
	}
	return bExist;
}

int GetElementCount(struct  NODE*  pNode)  {
	/*  ����� ���Ҽ� ��ȯ */
	int i;
	int count;
	if (pNode == NULL) return 0;
	count = 0;
	for (i = 0; i<DEGREE - 1; ++i)     {
		if (pNode->nElm[i] != 0) count++;
		else   break;
	}
	return count;
}

void PrintNodeElement(struct NODE* pNode)  {
	/* ����� key �� ���  */
	int i;

	/* In-order�� ��� */
	for (i = 0; i < DEGREE; i++){
		/* �ڽ� ��尡 �����Ѵٸ� ����Լ��� �ڽ� ȣ�� */
		if (pNode->pChild[i] != NULL){
			PrintNodeElement(pNode->pChild[i]);
		}
		if (pNode->nElm[i] != 0){
			/* ���� ��忡 key�� �����Ѵٸ� key ��� */
			printf("%c ", pNode->nElm[i]);
		}
	}
	return;
}

int main(int argc, char* argv[])   {
	/*  main */
	int i;
	int nPos;		/* ����  ��ġ�� �����ϱ� ���� �ӽ� ���� */
	char nKey;		/* ���� �� ������ ���� key��  */
	int bFinished;		/* ����, ������ �Ͼ������ flag */
	int bFound;		/* B-Ʈ������  �ش� key�� �߰ߵǾ������� flag */
	int bOverflow;		/* �ش� node���� �����÷ΰ� �߻��ߴ����� flag  */
	int nCount;               /* �ش� node�� key �� */
	struct NODE* pRoot;             /* ��Ʈ node */
	struct NODE* pCurr;             /* ���� �۾����� ���� node */
	struct NODE* pChild;            /* ���� node�� child node  */
	struct NODE* pNewNode;	/* ������ �� node */
	struct NODE* plnkey;            /* in-key�� pointer */
	struct STACK_NODE* pStackTop;   /* stack �� top pointer  */
	struct OVERFLOW_NODE*  pOverflow;
	/*���� �÷��� ��츦 ó�� �� �ӽ� node*/
	int nElementCnt;			/* ����� �� ���� */
	int height = 0;			/* Ʈ���� ���� ī���� */

	/*  �� �� ȭ  */
	nKey = 0;
	bFinished = 0;
	bFound = 0;
	bOverflow = 0;
	nCount = 0;
	nElementCnt = 0;
	/* ���� B-Ʈ�� ���� */
	pRoot = CreateNode();
	pRoot->bRoot = 1;
	height++;
	pCurr = pRoot;

	/* input.txt  ȭ�� ���� */
	if ((pInputStream = fopen("input.txt", "r")) == NULL)  {
		/* input ȭ���� ����  ���� */
		printf("input.txt  file cannot be openedl\n");
		return -1;
	}


	/*  B-Ʈ����  ����  */
	printf("[B-Tree] Insertion Process started\n");
	pStackTop = NULL;
	while (1)  {
		nKey = 0;
		if (fscanf(pInputStream, "%c", &nKey) == EOF){
			/* EOF�� ���� */
			break;
		}
		if (nKey == 32){
			/* ������ ��� ���� �� �б� */
			fscanf(pInputStream, "%c", &nKey);
		}

		/* key���� �迭�� ���� */
		nElementCnt++;
		pCurr = pRoot;
		/* stack �� ��ȭ */
		while (pStackTop && pStackTop->pCurr){
			pStackTop = PopFromStack(pStackTop);
		}
		pStackTop = CreateStackNode();

		pChild = NULL;
		printf("Key (%c) inserted ", nKey);
		do {
			nCount = GetElementCount(pCurr);
			if (nCount == 0){
				/* root node�� �ʱ⿡ key���� 0���� �� ���� */
				pChild = pCurr->pChild[0];
			}
			else if (CheckElementExist(pCurr, nKey) == 1){
				/* key���� �̹� tree�� ������ �� */
				bFound = 1;
			}
			else if (nKey < pCurr->nElm[0])  {
				/* key������ ū ���� ó�� key�� ���� �����
				ó�� �ڽĳ�带 �����Ѵ�. */
				pChild = pCurr->pChild[0];
			}
			else if (nKey>pCurr->
				nElm[(nCount - 1 > 0) ? nCount - 1 : 0])  {
				/* ���� ����� �� key������ ū key���
				����� �� �ڽĳ�带 �����Ѵ�. */
				pChild = pCurr->pChild[nCount];
			}
			else {
				/* ���� node�� �����ؼ� key���� ���� ������
				�߰���ġ�� ã�´� */
				for (i = 0; i<nCount - 1; ++i)  {
					if (nKey  > pCurr->nElm[i]
						&& nKey < pCurr->nElm[i + 1]) {
						pChild = pCurr->pChild[i + 1];
						break;
					}
				}
			}

			if (pChild != NULL){
				/* ���� �ڽ��� �ִٸ� ���ÿ� �����Ͽ�
				���߿� �θ��尡 �ʿ��� �� ������ ��� */
				pStackTop = PushlntoStack(pStackTop, pCurr);
				pCurr = pChild;
			}
		} while (!bFound && pChild != NULL);
		/* �ش��ϴ� key���� �ְų� �ڽĳ�尡 ���� leaf��忡
		�����ϸ� ����� */

		if (bFound == 1)  {
			/* B-Ʈ���� �����ϴ� key���� ��� */
			printf("[lnsertion] Key (%c)  already exists in the B-Tree\n", nKey);
		}
		else {
			/* B-Ʈ���� �������� �ʴ� key���
			Ʈ���� ��带 ���� */
			pChild = NULL;
			plnkey = NULL;
			bFinished = 0;

			do {
				/* �Էµ� key�� ��ġ�� �����ϴ� �ݺ��� */
				nCount = GetElementCount(pCurr);

				if (plnkey)    nKey = plnkey->nElm[0];
				/* ���� child�� ���� �θ� ���� �ö�Դٸ�
				���� key�� �ö�� key���� �ȴ� */

				if (nCount < DEGREE - 1)
					/* ������ �ƴ� ��� */   {
					/* �ش� ��ġ�� �ٷ� ���� */
					if (nCount == 0) {
						/* key���� ���� �� ����� ��
						������ġ�� ó��*/
						nPos = 0;
					}
					else if (nKey < pCurr->nElm[0])  {
						/* key���� ���� ����� ù ��°
						key������ �۴ٸ� ������ġ�� ó�� */
						nPos = 0;
					}
					else if (nKey  >
						pCurr->nElm[(nCount - 1 > 0) ? nCount - 1 : 0]) {
						/*  key���� ����� �� key������ ũ�ٸ�
						������ġ�� ����� �� */
						nPos = nCount;
					}

					else {
						/* ���� node�� �����ؼ� key���� ����
						������ �߰���ġ�� ã�´� */
						for (i = 0; i<nCount - 1; ++i){
							if (nKey > pCurr->nElm[i]
								&& nKey < pCurr->nElm[i + 1]) {
								nPos = i + 1;
								break;
							}
						}
					}

					/*  ���� key���� shift */
					for (i = DEGREE - 2; i>nPos; --i){
						pCurr->nElm[i] = pCurr->nElm[i - 1];
						pCurr->pChild[i + 1] = pCurr->pChild[i];
					}
					/* ���� nKey�� ���ԵǴ� ���� �������� */
					pCurr->nElm[nPos] = nKey;
					pCurr->pChild[nPos + 1] = NULL;

					if (plnkey)  {
						/* ���� child�� ���� �θ� ���� �ö�Դٸ�                                              ���� key�� �ö�� key���� �ȴ� */
						pCurr->pChild[nPos] = plnkey->pChild[0];
						pCurr->pChild[nPos + 1]
							= plnkey->pChild[1];
					}
					bFinished = 1;
				}
				else {
					/* ��忡 key���� ���� ���ִٸ� overflow
					OVERFLOW_NODE�� �����Ͽ� �۾��ϱ� */
					pOverflow = CreateOverflowNode();
					for (i = 0; i < DEGREE - 1; ++i) {
						pOverflow->nElm[i] = pCurr->nElm[i];
						pOverflow->pChild[i] = pCurr->pChild[i];
					}
					pOverflow->pChild[DEGREE - 1] =
						pCurr->pChild[DEGREE - 1];
					/* nKey ���� */

					nCount = DEGREE - 1;
					if (nKey  < pOverflow->nElm[0]){
						/* key���� overflow ����� ù ��°
						key������ �۴ٸ� ������ġ�� ó�� */
						nPos = 0;
					}
					else if (nKey>pOverflow->
						nElm[(nCount - 1 > 0) ? nCount - 1 : 0]){
						/*  key���� ����� �� key������ ũ�ٸ�
						������ġ�� ����� �� */
						nPos = nCount;
					}
					else {
						/* ���� node�� �����ؼ� key���� ����
						������ �߰���ġ�� ã�´� */
						for (i = 0; i<nCount - 1; ++i){
							if (nKey>pOverflow->nElm[i]
								&& nKey < pOverflow->nElm[i + 1]){
								nPos = i + 1;
								break;
							}
						}
					}
					/*  �� �� key���� shift */
					for (i = DEGREE - 1; i >= nPos; --i)  {
						pOverflow->nElm[i] =
							pOverflow->nElm[i - 1];
						pOverflow->pChild[i + 1] =
							pOverflow->pChild[i];
					}
					/* ���� */
					pOverflow->nElm[nPos] = nKey;
					pOverflow->pChild[nPos + 1] = NULL;
					if (plnkey)  {
						pOverflow->pChild[nPos] =
							plnkey->pChild[0];
						pOverflow->pChild[nPos + 1] =
							plnkey->pChild[1];
					}
					/*  �߰��� ���ϱ�  */
					nPos = (DEGREE - 1) / 2;
					nKey = pOverflow->nElm[nPos];

					/* �����÷θ� pCurr�� �� ��忡 ����
					pCurr�� clear�� �� pOverflow�� 1st half�� ���� */
					for (i = 0; i < DEGREE - 1; ++i)    {
						pCurr->nElm[i] = 0;
						pCurr->pChild[i] = NULL;
					}
					pCurr->pChild[DEGREE - 1] = NULL;


					for (i = 0; i < nPos; ++i)    {
						pCurr->nElm[i] = pOverflow->nElm[i];
						pCurr->pChild[i] = pOverflow->pChild[i];
					}
					pCurr->pChild[nPos] =
						pOverflow->pChild[nPos];

					/* ����忡 2nd half�� ���� */
					pNewNode = CreateNode();
					for (i = nPos + 1; i < DEGREE; ++i)    {
						pNewNode->nElm[i - nPos - 1] =
							pOverflow->nElm[i];
						pNewNode->pChild[i - nPos - 1] =
							pOverflow->pChild[i];
					}
					pNewNode->pChild[DEGREE - nPos - 1] =
						pOverflow->pChild[DEGREE];
					/* plnkey�� �����Ͽ� key�� child�� ����  */
					plnkey = CreateNode();
					plnkey->nElm[0] = nKey;
					plnkey->pChild[0] = pCurr;
					plnkey->pChild[1] = pNewNode;
					/* ���� �ö󰡸� ���� �ݺ� */
					if (pStackTop  && pStackTop->pCurr){
						/* �θ� node�� �÷����� */
						pCurr = PeepStackTop(pStackTop);
						pStackTop = PopFromStack(pStackTop);
					}
					else {
						/* Ʈ������ ���� */
						/* plnkey�� �� root�� �� */
						pCurr->bRoot = 0;
						pRoot = plnkey;
						height++;
						plnkey->bRoot = 1;
						bFinished = 1;
					}
				}
			} while (bFinished == 0);
		}
		printf("[Depth of Tree : %d] \n", height);
		PrintNodeElement(pRoot);
		printf("\n\n");

	}
	printf("[B-Tree] Insertion Process Ended\n");
	fclose(pInputStream);

	///////////////////////////////////////////////////////////////////////////////////


	/* b-Ʈ�� ����  ����:  ������  ���� �ʱ�ȭ */
	pCurr = NULL;
	pChild = NULL;
	pNewNode = NULL;
	plnkey = NULL;
	pStackTop = NULL;
	pOverflow = NULL;
	nKey = 0;
	bFinished = 0;
	bFound = 0;
	bOverflow = 0;
	nCount = 0;
	int nTemp, nTemp2, nTemp3;
	char nTemp4;

	/* ���� ���� */
	printf("\n[B-Tree]  Deletion Process started\n");	

	/* inputD.txt  ȭ�� ���� */
	if ((pInputStream = fopen("inputD.txt", "r")) == NULL)  {
		/* input ȭ���� ����  ���� */
		printf("input.txt  file cannot be openedl\n");
		return -1;
	}
	//while (pInputStream != EOF)
	while (1){
		nKey = 0;
		if (fscanf(pInputStream, "%c", &nKey) == EOF){
			/* EOF�� ���� */	break;
		}
		if (nKey == 32){
			/* ������ ��� ���� �� �б� */
			fscanf(pInputStream, "%c", &nKey);
		}

		/* key�� Ű ���� ���� */
		printf("Key (%c) deleted \n", nKey);
		bFound = 0; // �ش� key�� ã������ ���� flag
		/* Stack�� �θ� ��带 �����鼭 key�� ã�� */
		while (pStackTop && pStackTop->pCurr){
			/* ������ ���� */
			pStackTop = PopFromStack(pStackTop);
		}
		pStackTop = CreateStackNode(); // ���ο� ������ ����
		pCurr = pRoot;	// root�� ������ ���� ����
		pChild = NULL;
		do {
			nCount = GetElementCount(pCurr);	// ���� ����� key��
			if (CheckElementExist(pCurr, nKey) == 1){
				/* ���� ��忡�� �ش��ϴ� key�� ã��*/
				bFound = 1;
				pChild = pCurr;
			}
			else if (nKey < pCurr->nElm[0]){
				/* ���� ����� ó�� key���� ã���� �ϴ� key���� ũ�� ó�� child ���� */
				pChild = pCurr->pChild[0];
			}
			else if (nKey > pCurr->nElm[(nCount - 1 > 0) ? nCount - 1 : 0]) {
				/* ���� ����� ������ key���� ã���� �ϴ� key���� �۴ٸ� ������ child ���� */
				pChild = pCurr->pChild[nCount];
			}
			else {
				/* ���� node�� �߰� key���� �۴ٸ� �׹ٷ� ������ child���� */
				for (i = 0; i<nCount - 1; ++i)  {
					if (nKey > pCurr->nElm[i] && nKey < pCurr->nElm[i + 1]){
						pChild = pCurr->pChild[i + 1];
						break;
					}
				}
			}
			if (pChild != NULL){
				/* child ��尡 �ִٸ� ������ ��带 ���ÿ� �����ϰ� child��尡 ���� ��尡�ȴ�. */
				pStackTop = PushlntoStack(pStackTop, pCurr);
				pCurr = pChild;
			}

		} while (!bFound);

		if (bFound != 1){
			/* key���� Tree�� �������� �ʴ´ٸ� */
			printf("[Deletion] Key (%c) does not exist in the B-Tree\n", nKey);
		}

		///////
		else {
			/* ���� ��忡 �������� ��� */
			/* �� ��죬pCurr�� NULL�� �ƴϰ�
			�ּ��� pCurr�� leftmost child�� NULL�� �ƴ� */

			if (pCurr && pCurr->pChild[0]) { /* ������ ã�� */
				pChild = NULL;
				nPos = 0;
				for (i = 0; i < DEGREE - 1; ++i){
					if (nKey == pCurr->nElm[i]){
						/* pCurr�� ���� ��尡 �ƴ� �Ƿ�
						�����ڴ� pCurr���� key���� ���� child��
						�� ���� �ڼ��� �� ���� key���� �� */
						nPos = i;
						pChild = pCurr->pChild[i];
						break;
					}
				}

				if (pChild)
					pStackTop = PushlntoStack(pStackTop, pChild);
				/* ��δ� stack�� ���� */
				/* ��δ� pCurr���� �����Ϲ���, pStackTop�� ÷�� */
				while (pChild) {
					bFound = 0;  /* ���� ���� pos */
					/* pChild���� ���� ū key�� ã��
					�ش� ����� ����child ���� �˻�*/
					for (i = DEGREE - 2; i >= 0; --i) {
						if (pChild->nElm[i] > 0)   {
							bFound = i;
							break;
						}
					}
					if (pChild->pChild[bFound + 1]){
						pStackTop = PushlntoStack(pStackTop, pChild);
						pChild = pChild->pChild[bFound + 1];
					}
					else {    /* ���� ��� */
						/* key�� swap �ϰ�, loop Ż�� */
						nTemp4 = pChild->nElm[bFound];
						pChild->nElm[bFound] = pCurr->nElm[nPos];
						pCurr->nElm[nPos] = nTemp4;
						pCurr = pChild;
						break;
					}
				}
			}
			/* key�� �����ϰ� shift */
			nPos = 0;
			for (i = 0; i < DEGREE - 1; ++i){
				if (pCurr->nElm[i] == nKey){
					nPos = i;
					break;
				}
			}
			if (nPos == DEGREE - 2){
				/* ������ key���̶�� �׳� ����*/
				pCurr->nElm[nPos] = 0;
			}
			else {
				////444
				for (i = nPos + 1; i < DEGREE - 1; ++i) {
					/* �̵���Ų�� */
					pCurr->nElm[i-1] = pCurr->nElm[i];
				}
				pCurr->nElm[DEGREE - 2] = 0;
			}
			/* Ʈ��  ������ */
			bFinished = 0;
			do {
				if (pCurr->bRoot == 1 || GetElementCount(pCurr)
					>= ((DEGREE + 1) / 2 - 1)) {
					/* root node�̰ų� ��� �÷ΰ� �߻����� ������ */
					bFinished = 1;
				}
				else {
					/*  ��й� ���ɿ��� �Ǻ� */
					bFound = 0; /* ��й� ���� ���� flag */
					nTemp = 0;         /* 0: impossible��1: left sibling��2: right sibling */
					pChild = NULL;
					/* root�� �ƴϹǷ� parent ����  */
					pNewNode = PeepStackTop(pStackTop);
					if (pCurr == pNewNode) {
						/* ���� ���� �������� ���� ��尡 ������ */
						pStackTop = PopFromStack(pStackTop);
						pNewNode = PeepStackTop(pStackTop);
					}
					if (pNewNode == NULL) 
						printf("Error  wrong Situation\n");
					nPos = -1;
					for (i = 0; i <= GetElementCount(pNewNode); ++i){
						if (pCurr == pNewNode->pChild[i])   {
							/* �θ��带 ã�´� */
							nPos = i;
							break;
						}
					}

					if (nPos == -1)
						printf("Error, the Node doesnt match\n");
					/* Sibling�� ã�Ƽ� ��й谡�� ���� ���� */
					if (nPos == 0){ /* �� ��쿡�� ������ ���� sibling ���� */
						if (GetElementCount(pNewNode->pChild[nPos + 1])
				> ((DEGREE + 1) / 2 - 1)) {
							bFound = 1;
							nTemp = 2;
						}
					}
					else if (nPos == DEGREE - 1)  {
						/*  �� ��쿡�� ������ ���� sibling���� */
						if (GetElementCount(pNewNode->pChild[nPos - 1])
			> ((DEGREE + 1) / 2 - 1)) {
							bFound = 1;
							nTemp = 1;
						}
					}
					else {  /* left sibling ���� */
						if (GetElementCount(pNewNode->pChild[nPos - 1])
					> ((DEGREE + 1) / 2 - 1)) {
							bFound = 1;
							nTemp = 1;
						}

						else if (GetElementCount(pNewNode->pChild[nPos + 1])
					> ((DEGREE + 1) / 2 - 1))  {
							bFound = 1;
							nTemp = 2;
						}
					}
					if (bFound) { /* ��й� ���� */
						/* TWOBNODE �����������  */
						if (nTemp == 1) {
							/* ���� sibling��parent��  ���� ������ ������
							- key��, child�� �̵� */
							pChild = pNewNode->pChild[nPos - 1];
							nTemp3 = GetElementCount(pChild);
							nTemp2 = (nTemp3 - GetElementCount(pCurr)) / 2;
							/* ������ ���߱� ���� left sibling����
							nTemp ����ŭ pCurr�� �й� */
							/* pCurr���� nTemp2��ŭ �������� shift */
							for (i = DEGREE - 2; i >= nTemp2; --i)  {
								pCurr->nElm[i] = pCurr->nElm[i - nTemp2];
								pCurr->pChild[i + 1] = pCurr->pChild[i + 1 - nTemp2];
							}
							pCurr->pChild[nTemp2] = pCurr->pChild[0];
							/*  �θ� key�� ���� ������ */
							pCurr->nElm[nTemp2 - 1] = pNewNode->nElm[nPos - 1];
							/* left sibling���� nTemp2-1��ŭ pCurr�� �� �� */
							for (i = 0; i<nTemp2 - 1; ++i)  {
								pCurr->nElm[nTemp2 - i - 2]
									= pChild->nElm[nTemp3 - i - 1];
								pCurr->pChild[nTemp2 - i - 1]
									= pChild->pChild[nTemp3 - i];
							}
							pCurr->pChild[0] = pChild->pChild[nTemp3 - nTemp2 + 1];
							/*  leftsibling���� �θ�� key �÷��ְ� ���� */
							pNewNode->nElm[nPos - 1]
								= pChild->nElm[nTemp3 - nTemp2];
							for (i = nTemp3 - 1; i>nTemp3 - nTemp2 - 1; --i){
								pChild->nElm[i] = 0;
								pChild->pChild[i + 1] = NULL;
							}
						}
						else {
							/*  ���� sibling, parent�� ���� ������ ������
							key��, child�� �̵�  */
							pChild = pNewNode->pChild[nPos + 1];
							nTemp3 = GetElementCount(pChild);
							nTemp2 = (nTemp3 - GetElementCount(pCurr)) / 2;
							/* ������ ���߱� ���� right sibling���� nTemp
							����ŭ pCurr�� �й� */
							/* �θ� key�� ���� ������ */
							pCurr->nElm[GetElementCount(pCurr)]
								= pNewNode->nElm[nPos];
							/* right sibling���� nTemp2-1 ��ŭ pCurr�� �̵� */
							for (i = 0; i < nTemp2 - 1; ++i) {
								pCurr->nElm[GetElementCount(pCurr) + i + 1]
									= pChild->nElm[i];
								pCurr->pChild[GetElementCount(pCurr) + i + 1]
									= pChild->pChild[i];
							}
							pCurr->pChild[GetElementCount(pCurr) + nTemp2 - 1]
								= pChild->pChild[nTemp2 - 1];
							/* right sibling���� �θ�� key �÷��ְ� left shift */
							pNewNode->nElm[nPos] = pChild->nElm[nTemp2 - 1];
							for (i = 0; i<DEGREE - 1 - nTemp2; ++i)  {
								pChild->nElm[i] = pChild->nElm[i + nTemp2];
								pChild->pChild[i] = pChild->pChild[i + nTemp2];
							}
							pChild->pChild[DEGREE - 1 - nTemp2]
								= pChild->pChild[DEGREE - 1];
							for (i = nTemp3; i>nTemp3 - nTemp2; --i){
								pChild->nElm[i - 1] = 0;
								pChild->pChild[i] = NULL;
							}
						}
						bFinished = 1;
					}
					else { /* cancatenation  */
						nTemp = (nPos == 0) ? 2 : 1;
						if (nTemp == 1)  { /* left sibling�� �պ� */
							pChild = pNewNode->pChild[nPos - 1];
							nTemp3 = GetElementCount(pChild);
							nTemp2 = GetElementCount(pCurr);
							/*  parent �պ�  */
							pChild->nElm[nTemp3] = pNewNode->nElm[nPos - 1];
							pChild->pChild[nTemp3 + 1] = pCurr->pChild[0];
							/* pCurr�� key�� child�� left sibling�� �պ� */
							for (i = 0; i < nTemp2; ++i) {
								pChild->nElm[i + nTemp3 + 1] = pCurr->nElm[i];
								pChild->pChild[i + nTemp3 + 2] = pCurr->pChild[i + 1];
								/* parent����  key, child ���� */
							}
							for (i = nPos - 1; i < DEGREE - 1; ++i) {
								pNewNode->nElm[i] = pNewNode->nElm[i + 1];
								pNewNode->pChild[i + 1] = pNewNode->pChild[i + 2];
							}
							pNewNode->nElm[DEGREE - 2] = 0;
							pNewNode->pChild[DEGREE - 1] = NULL;
							pCurr = pNewNode;
						}
						else { /* right sibling�� �պ� */
							pChild = pNewNode->pChild[nPos + 1];
							nTemp3 = GetElementCount(pChild);
							nTemp2 = GetElementCount(pCurr);
							/*  parent �պ� */
							pCurr->nElm[nTemp2] = pNewNode->nElm[nPos];
							pCurr->pChild[nTemp2 + 1] = pChild->pChild[0];
							/*  right sibling�� key�� child�� pCurr�� �պ�*/
							for (i = 0; i < nTemp3; ++i) {
								pCurr->nElm[i + nTemp2 + 1] = pChild->nElm[i];
								pCurr->pChild[i + nTemp2 + 2] = pChild->pChild[i + 1];
							}
							/* parent���� key, child ���� */
							for (i = nPos; i < DEGREE - 1; ++i)  {
								pNewNode->nElm[i] = pNewNode->nElm[i + 1];
								pNewNode->pChild[i + 1] = pNewNode->pChild[i + 2];
							}
							pNewNode->nElm[DEGREE - 2] = 0;
							pNewNode->pChild[DEGREE - 1] = NULL;
							pCurr = pNewNode;
						}
					}
				}
			} 
			while (!bFinished);
			/*  root�� key�� ������ Ʈ�� ������ ����  */
			nTemp2 = GetElementCount(pRoot);
			if (nTemp2 == 0) {
				height--;
				if (pRoot->pChild[0]) {
					/*  ������  ���*/
				}
					if (pRoot->pChild[1]){
						/*  ���������� ��� */
						printf("wrong  situation\n");
					}
					if (pRoot->pChild[0])  {
						pRoot = pRoot->pChild[0];
						pRoot->bRoot = 1;
					}
				}
			}
			if (GetElementCount(pRoot)) {
				PrintNodeElement(pRoot);
			}
			else {
				printf("There's no keys in the B-Tree\n");
			}
			printf("\nDepth of Tree : %d\n",height);
		}
		printf("[B-Tree] Deletion Process Ended\n");

		fclose(pInputStream);
		return 0;
	}

