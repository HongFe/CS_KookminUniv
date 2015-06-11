#include "btree.h"

int CheckElementExist(struct  NODE* pNode, char key)   {
	/* ��忡 key�� �ִ��� �˻� */
	int i;
	int bExist;            /* key�� �����ϴ��� ����  flag*/
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
	/*  ����� ���Ҽ� ��ȯ */
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
	/* �����  key �� ���  */
	int i;
	struct NODE* pChild;

	/* In-order�� ��� */
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
	int nPos;                 /* ����  ��ġ�� �����ϱ� ���� �ӽ� ���� */
	char nKey;                 /* ���� �� ������ ���� key��  */
	int bFinished;          /* ����, ������ �Ͼ������ flag */
	int bFound;             /* B-Ʈ������  �ش� key�� �߰ߵǾ������� flag */
	int bOverflow;         /* �ش� node�� ��  �����÷ΰ�  �߻��ߴ����� flag  */
	int nCount;                     /* �ش� node��  key ��  */
	struct NODE* pRoot;                   /* ��Ʈ  node */
	struct NODE* pCurr;             /* ���� �۾����� ���� node */
	struct NODE* pChild;            /* ���� node�� child node  */
	struct NODE* pNewNode;     /* ������ �� node */
	struct NODE* plnkey;            /* in-key�� pointer */
	struct STACK_NODE* pStackTop;       /* stack �� top pointer  */
	struct OVERFLOW_NODE*  pOverflow;  /*���� �÷��� ��츦 ó�� �� �ӽ� node*/
	char nlnput[MAX_ELEMENTS];   /* ȭ�� ������ ���̱� ���� �迭�� ���� */
	int nElementCnt;
	int height = 0;				/* Ʈ���� ���� ī���� */

	/*  �� �� ȭ  */
	nKey = NULL; bFinished = 0; bFound = 0; bOverflow = 0; nCount = 0;
	for (i = 0; i < MAX_ELEMENTS; ++i)
		nlnput[i] = NULL;
	nElementCnt = 0;
	/* ���� B-Ʈ�� ���� */
	pRoot = CreateNode();
	pRoot->bRoot = 1;
	height++;
	pCurr = pRoot;
	/* input.txt  ȭ�� ���� */

	if ((pInputStream = fopen("input1.txt", "r")) == NULL)  {
		/* input ȭ���� ����  ���� */
		printf("input.txt  file cannot be openedl\n");
		return -1;
	}
	/* output.txt  ȭ�� ���� */
	if ((pOutputStream = fopen("output.txt", "w")) == NULL)  {
		/*  output ȭ���� ���� ����  */
		printf("output.txt  file cannot be openedl\n");
		return -1;
	}
	/*  b-Ʈ����  ����  */
	printf("[B-Tree] Insertion Process started\n");
	pStackTop = NULL;
	while (1)  {
		nKey = NULL;
		if (fscanf(pInputStream, "%c", &nKey) == EOF){
			/* EOF�� ���� */
			break;
		}
		if (nKey == 32){
			fscanf(pInputStream, "%c", &nKey);
		}

		/* key���� �迭�� ���� */
		nlnput[nElementCnt] = nKey;
		nElementCnt++;
		pCurr = pRoot;
		/* stack �� ��ȭ */
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
				/*  root node�� �ʱ�� key����  0�� �� ��  ���� */
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
				/*  ����  node�� key�� �߰���  ��ġ�� ��  �� */
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
			/*  key�� B-Ʈ���� ���� ���� plnkey�� �����ϸ�
			In-key�� ���� ��� ��, 1 ���� ���� ���� */
			pChild = NULL;
			plnkey = NULL;
			bFinished = 0;

			do {
				nCount = GetElementCount(pCurr);
				if (plnkey)    nKey = plnkey->nElm[0];
				if (nCount  < DEGREE - 1)   /*  ������ �ƴ� �� ��*/   {
					/*  �ش� ��ġ�� �ٷ� ���� */
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
					/*  ���� key���� shift */
					for (i = DEGREE - 2; i>nPos; --i){
						pCurr->nElm[i] = pCurr->nElm[i - 1];
						pCurr->pChild[i + 1] = pCurr->pChild[i];
					}
					/* ���� nKey�� ���ԵǴ� ���� �������� */
					pCurr->nElm[nPos] = nKey;
					pCurr->pChild[nPos + 1] = NULL;
					if (plnkey)  {
						pCurr->pChild[nPos] = plnkey->pChild[0];
						pCurr->pChild[nPos + 1] = plnkey->pChild[1];
					}
					bFinished = 1;
				}
				else {
					/* OVERFLOW_NODE�� ���� */
					pOverflow = CreateOverflowNode();
					for (i = 0; i < DEGREE - 1; ++i) {
						pOverflow->nElm[i] = pCurr->nElm[i];
						pOverflow->pChild[i] = pCurr->pChild[i];
					}
					pOverflow->pChild[DEGREE - 1] = pCurr->pChild[DEGREE - 1];
					/*  nKey ����  */

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
					/*  �� �� key���� shift */
					for (i = DEGREE - 1; i >= nPos; --i)  {
						pOverflow->nElm[i] = pOverflow->nElm[i - 1];
						pOverflow->pChild[i + 1] = pOverflow->pChild[i];
					}
					/* ���� */
					pOverflow->nElm[nPos] = nKey;
					pOverflow->pChild[nPos + 1] = NULL;
					if (plnkey)  {
						pOverflow->pChild[nPos] = plnkey->pChild[0];
						pOverflow->pChild[nPos + 1] = plnkey->pChild[1];
					}
					/*  �߰��� ���ϱ�  */
					nPos = (DEGREE - 1) / 2;
					nKey = pOverflow->nElm[nPos];
					/*  ���� �÷θ�  pCurr�� ��  ����   ���� */
					/* pCurr�� clear�� ��  pOverflow�� 1st half�� ���� */
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
					/*  ����忡 2nd half�� ���� */
					pNewNode = CreateNode();
					for (i = nPos + 1; i < DEGREE; ++i)    {
						pNewNode->nElm[i - nPos - 1] = pOverflow->nElm[i];
						pNewNode->pChild[i - nPos - 1] = pOverflow->pChild[i];
					}
					pNewNode->pChild[DEGREE - nPos - 1] =
						pOverflow->pChild[DEGREE];
					/*  plnkey�� �����Ͽ� key�� child�� ����  */
					plnkey = CreateNode();
					plnkey->nElm[0] = nKey;
					plnkey->pChild[0] = pCurr;
					plnkey->pChild[1] = pNewNode;
					/*  ���� �ö󰡸� ���� �ݺ� */
					if (pStackTop  && pStackTop->pCurr){
						/*  �θ�  node�� �÷�  ����  */
						pCurr = PeepStackTop(pStackTop);
						pStackTop = PopFromStack(pStackTop);
					}
					else {
						/*  Ʈ�� ���� ���� */
						/*  plnkey�� �� root�� ��  */
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