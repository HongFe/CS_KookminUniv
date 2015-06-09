#include "main.h"

struct NODE* CreateNode(){
	/* 노드 생성  */
	struct NODE* pNode;
	pNode = (struct NODE*)malloc(sizeof(struct NODE));
	memset(pNode, 0, sizeof(struct  NODE));
	return pNode;
}
struct OVERFLOW_NODE* CreateOverflowNode()  {
	/* 오버플로 노드 생성 */
	struct OVERFLOW_NODE*  pNode;
	pNode = (struct OVERFLOW_NODE*)malloc(sizeof(struct OVERFLOW_NODE));
	memset(pNode, 0, sizeof(struct OVERFLOW_NODE));
	return pNode;
}

struct STACK_NODE* CreateStackNode(){
	/* Stack Node 생 성   */
	struct STACK_NODE*  pStackNode;
	pStackNode = (struct  STACK_NODE*)malloc(sizeof(struct  STACK_NODE));
	memset(pStackNode, 0, sizeof(struct STACK_NODE));
	return pStackNode;
}

struct STACK_NODE* PushlntoStack
	/*	Stack에 push  */
	(struct  STACK_NODE*  pStackTop, struct NODE*  pNode)   {
	struct STACK_NODE* pNewTop;
	if (pStackTop == NULL || pNode == NULL){
		return NULL;
	}

	if (pStackTop->pCurr == NULL){
		/* top이 비어 있는경우 */
		pStackTop->pCurr = pNode;
		pStackTop->pPrev = NULL;
		return pStackTop;
	}
	else {
		/* 새 top-블  생성   */
		pNewTop = CreateStackNode();
		pNewTop->pPrev = (void*)pStackTop;
		pNewTop->pCurr = pNode;
		return pNewTop;
	}
}

struct STACK_NODE*  PopFromStack(struct   STACK_NODE* pStackTop)  {
	/* Stack에 서 pop */
	struct STACK_NODE* pPrevTop;
	struct NODE* pNode;
	if (pStackTop == NULL) return NULL;
	if (pStackTop->pCurr == NULL) return NULL;
	else if (pStackTop->pPrev == NULL)   {
		/* 현재 top임 */
		pNode = pStackTop->pCurr;
		pStackTop->pCurr = NULL;
		return pStackTop;
	}
	else {
		/* pCurr， pPrev 모두 존재 */
		pNode = pStackTop->pCurr;
		pPrevTop = pStackTop;
		pStackTop = (struct STACK_NODE*)  pStackTop->pPrev;
		/* 이 전  top 삭제   */
		free((void*)pPrevTop);
		return pStackTop;
	}
}
struct NODE* PeepStackTop(struct   STACK_NODE*  pStackTop)  {
	/* Stack의  top을 peep */
	if (pStackTop == NULL)    return NULL;
	return pStackTop->pCurr;
}
int CheckElementExist(struct  NODE* pNode, char key)   {
	/* 노드에 key가 있는지 검사 */
	int i;
	int bExist;            /* key가 존재하는지 여부  flag*/
	if (pNode == NULL)    return 0; /* 노드가 존재 않으면 0 리턴 */
	bExist = 0;
	for (i = 0; i < DEGREE - 1; ++i)    {
		if (key == pNode->nElm[i]) {
			/* 노드에 찾는 key값이 존재한다면 */
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
		if (pNode->nElm[i] != 0) count++;
		else   break;
	}
	return count;
}

void PrintNodeElement(struct NODE* pNode)  {
	/* 노드의 key 값 출력  */
	int i;

	/* In-order로 출력 */
	for (i = 0; i < DEGREE; i++){
		/* 자식 노드가 존재한다면 재귀함수로 자식 호출 */
		if (pNode->pChild[i] != NULL){
			PrintNodeElement(pNode->pChild[i]);
		}
		if (pNode->nElm[i] != 0){
			/* 본인 노드에 key가 존재한다면 key 출력 */
			printf("%c ", pNode->nElm[i]);
		}
	}
	return;
}

int main(int argc, char* argv[])   {
	/*  main */
	int i;
	int nPos;		/* 삽입  위치를 저장하기 위한 임시 변수 */
	char nKey;		/* 삽입 및 삭제를 해줄 key값  */
	int bFinished;		/* 삽입, 삭제가 일어났는지의 flag */
	int bFound;		/* B-트리에서  해당 key가 발견되었는지의 flag */
	int bOverflow;		/* 해당 node에서 오버플로가 발생했는지의 flag  */
	int nCount;               /* 해당 node의 key 수 */
	struct NODE* pRoot;             /* 루트 node */
	struct NODE* pCurr;             /* 현재 작업수행 중인 node */
	struct NODE* pChild;            /* 현재 node의 child node  */
	struct NODE* pNewNode;	/* 생성할 새 node */
	struct NODE* plnkey;            /* in-key의 pointer */
	struct STACK_NODE* pStackTop;   /* stack 의 top pointer  */
	struct OVERFLOW_NODE*  pOverflow;
	/*오버 플로의 경우를 처리 할 임시 node*/
	int nElementCnt;			/* 노드의 총 갯수 */
	int height = 0;			/* 트리의 높이 카운팅 */

	/*  초 기 화  */
	nKey = 0;
	bFinished = 0;
	bFound = 0;
	bOverflow = 0;
	nCount = 0;
	nElementCnt = 0;
	/* 공백 B-트리 생성 */
	pRoot = CreateNode();
	pRoot->bRoot = 1;
	height++;
	pCurr = pRoot;

	/* input.txt  화일 열기 */
	if ((pInputStream = fopen("input.txt", "r")) == NULL)  {
		/* input 화일을 열수  없음 */
		printf("input.txt  file cannot be openedl\n");
		return -1;
	}


	/*  B-트리로  삽입  */
	printf("[B-Tree] Insertion Process started\n");
	pStackTop = NULL;
	while (1)  {
		nKey = 0;
		if (fscanf(pInputStream, "%c", &nKey) == EOF){
			/* EOF에 도달 */
			break;
		}
		if (nKey == 32){
			/* 공백일 경우 다음 것 읽기 */
			fscanf(pInputStream, "%c", &nKey);
		}

		/* key값을 배열에 저장 */
		nElementCnt++;
		pCurr = pRoot;
		/* stack 초 기화 */
		while (pStackTop && pStackTop->pCurr){
			pStackTop = PopFromStack(pStackTop);
		}
		pStackTop = CreateStackNode();

		pChild = NULL;
		printf("Key (%c) inserted ", nKey);
		do {
			nCount = GetElementCount(pCurr);
			if (nCount == 0){
				/* root node는 초기에 key값이 0개일 수 있음 */
				pChild = pCurr->pChild[0];
			}
			else if (CheckElementExist(pCurr, nKey) == 1){
				/* key값이 이미 tree에 존재할 때 */
				bFound = 1;
			}
			else if (nKey < pCurr->nElm[0])  {
				/* key값보다 큰 값의 처음 key를 갖은 노드라면
				처음 자식노드를 조사한다. */
				pChild = pCurr->pChild[0];
			}
			else if (nKey>pCurr->
				nElm[(nCount - 1 > 0) ? nCount - 1 : 0])  {
				/* 현재 노드의 끝 key값보다 큰 key라면
				노드의 끝 자식노드를 조사한다. */
				pChild = pCurr->pChild[nCount];
			}
			else {
				/* 현재 node를 조사해서 key값이 들어가기 마땅한
				중간위치를 찾는다 */
				for (i = 0; i<nCount - 1; ++i)  {
					if (nKey  > pCurr->nElm[i]
						&& nKey < pCurr->nElm[i + 1]) {
						pChild = pCurr->pChild[i + 1];
						break;
					}
				}
			}

			if (pChild != NULL){
				/* 만약 자식이 있다면 스택에 저장하여
				나중에 부모노드가 필요할 때 꺼내어 사용 */
				pStackTop = PushlntoStack(pStackTop, pCurr);
				pCurr = pChild;
			}
		} while (!bFound && pChild != NULL);
		/* 해당하는 key값이 있거나 자식노드가 없는 leaf노드에
		도달하면 멈춘다 */

		if (bFound == 1)  {
			/* B-트리에 존재하는 key값일 경우 */
			printf("[lnsertion] Key (%c)  already exists in the B-Tree\n", nKey);
		}
		else {
			/* B-트리에 존재하지 않는 key라면
			트리에 노드를 삽입 */
			pChild = NULL;
			plnkey = NULL;
			bFinished = 0;

			do {
				/* 입력된 key의 위치를 결정하는 반복문 */
				nCount = GetElementCount(pCurr);

				if (plnkey)    nKey = plnkey->nElm[0];
				/* 만약 child의 값이 부모 노드로 올라왔다면
				현재 key는 올라온 key값이 된다 */

				if (nCount < DEGREE - 1)
					/* 만원이 아닌 경우 */   {
					/* 해당 위치에 바로 삽입 */
					if (nCount == 0) {
						/* key값이 없는 빈 노드일 때
						삽입위치는 처음*/
						nPos = 0;
					}
					else if (nKey < pCurr->nElm[0])  {
						/* key값이 현재 노드의 첫 번째
						key값보다 작다면 삽입위치는 처음 */
						nPos = 0;
					}
					else if (nKey  >
						pCurr->nElm[(nCount - 1 > 0) ? nCount - 1 : 0]) {
						/*  key값이 노드의 끝 key값보다 크다면
						삽입위치는 노드의 끝 */
						nPos = nCount;
					}

					else {
						/* 현재 node를 조사해서 key값이 들어가기
						마땅한 중간위치를 찾는다 */
						for (i = 0; i<nCount - 1; ++i){
							if (nKey > pCurr->nElm[i]
								&& nKey < pCurr->nElm[i + 1]) {
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
						/* 만약 child의 값이 부모 노드로 올라왔다면                                              현재 key는 올라온 key값이 된다 */
						pCurr->pChild[nPos] = plnkey->pChild[0];
						pCurr->pChild[nPos + 1]
							= plnkey->pChild[1];
					}
					bFinished = 1;
				}
				else {
					/* 노드에 key값이 가득 차있다면 overflow
					OVERFLOW_NODE에 복사하여 작업하기 */
					pOverflow = CreateOverflowNode();
					for (i = 0; i < DEGREE - 1; ++i) {
						pOverflow->nElm[i] = pCurr->nElm[i];
						pOverflow->pChild[i] = pCurr->pChild[i];
					}
					pOverflow->pChild[DEGREE - 1] =
						pCurr->pChild[DEGREE - 1];
					/* nKey 삽입 */

					nCount = DEGREE - 1;
					if (nKey  < pOverflow->nElm[0]){
						/* key값이 overflow 노드의 첫 번째
						key값보다 작다면 삽입위치는 처음 */
						nPos = 0;
					}
					else if (nKey>pOverflow->
						nElm[(nCount - 1 > 0) ? nCount - 1 : 0]){
						/*  key값이 노드의 끝 key값보다 크다면
						삽입위치는 노드의 끝 */
						nPos = nCount;
					}
					else {
						/* 현재 node를 조사해서 key값이 들어가기
						마땅한 중간위치를 찾는다 */
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
						pOverflow->nElm[i] =
							pOverflow->nElm[i - 1];
						pOverflow->pChild[i + 1] =
							pOverflow->pChild[i];
					}
					/* 삽입 */
					pOverflow->nElm[nPos] = nKey;
					pOverflow->pChild[nPos + 1] = NULL;
					if (plnkey)  {
						pOverflow->pChild[nPos] =
							plnkey->pChild[0];
						pOverflow->pChild[nPos + 1] =
							plnkey->pChild[1];
					}
					/*  중간값 구하기  */
					nPos = (DEGREE - 1) / 2;
					nKey = pOverflow->nElm[nPos];

					/* 오버플로를 pCurr과 새 노드에 분할
					pCurr를 clear한 후 pOverflow의 1st half를 복사 */
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

					/* 새노드에 2nd half를 복사 */
					pNewNode = CreateNode();
					for (i = nPos + 1; i < DEGREE; ++i)    {
						pNewNode->nElm[i - nPos - 1] =
							pOverflow->nElm[i];
						pNewNode->pChild[i - nPos - 1] =
							pOverflow->pChild[i];
					}
					pNewNode->pChild[DEGREE - nPos - 1] =
						pOverflow->pChild[DEGREE];
					/* plnkey를 생성하여 key와 child를 삽입  */
					plnkey = CreateNode();
					plnkey->nElm[0] = nKey;
					plnkey->pChild[0] = pCurr;
					plnkey->pChild[1] = pNewNode;
					/* 위로 올라가며 삽입 반복 */
					if (pStackTop  && pStackTop->pCurr){
						/* 부모 node로 올려보냄 */
						pCurr = PeepStackTop(pStackTop);
						pStackTop = PopFromStack(pStackTop);
					}
					else {
						/* 트리레벨 증가 */
						/* plnkey가 새 root가 됨 */
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


	/* b-트리 에서  삭제:  삭제를  위한 초기화 */
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

	/* 삭제 시작 */
	printf("\n[B-Tree]  Deletion Process started\n");	

	/* inputD.txt  화일 열기 */
	if ((pInputStream = fopen("inputD.txt", "r")) == NULL)  {
		/* input 화일을 열수  없음 */
		printf("input.txt  file cannot be openedl\n");
		return -1;
	}
	//while (pInputStream != EOF)
	while (1){
		nKey = 0;
		if (fscanf(pInputStream, "%c", &nKey) == EOF){
			/* EOF에 도달 */	break;
		}
		if (nKey == 32){
			/* 공백일 경우 다음 것 읽기 */
			fscanf(pInputStream, "%c", &nKey);
		}

		/* key값 키 가져 오기 */
		printf("Key (%c) deleted \n", nKey);
		bFound = 0; // 해당 key를 찾았을때 쓰는 flag
		/* Stack에 부모 노드를 쌓으면서 key값 찾기 */
		while (pStackTop && pStackTop->pCurr){
			/* 스택을 비우기 */
			pStackTop = PopFromStack(pStackTop);
		}
		pStackTop = CreateStackNode(); // 새로운 스택을 생성
		pCurr = pRoot;	// root를 현재의 노드로 선택
		pChild = NULL;
		do {
			nCount = GetElementCount(pCurr);	// 현재 노드의 key수
			if (CheckElementExist(pCurr, nKey) == 1){
				/* 현재 노드에서 해당하는 key를 찾음*/
				bFound = 1;
				pChild = pCurr;
			}
			else if (nKey < pCurr->nElm[0]){
				/* 현재 노드의 처음 key값이 찾고자 하는 key보다 크면 처음 child 노드로 */
				pChild = pCurr->pChild[0];
			}
			else if (nKey > pCurr->nElm[(nCount - 1 > 0) ? nCount - 1 : 0]) {
				/* 현재 노드의 마지막 key값이 찾고자 하는 key보다 작다면 마지막 child 노드로 */
				pChild = pCurr->pChild[nCount];
			}
			else {
				/* 현재 node의 중간 key값중 작다면 그바로 오른쪽 child노드로 */
				for (i = 0; i<nCount - 1; ++i)  {
					if (nKey > pCurr->nElm[i] && nKey < pCurr->nElm[i + 1]){
						pChild = pCurr->pChild[i + 1];
						break;
					}
				}
			}
			if (pChild != NULL){
				/* child 노드가 있다면 현재의 노드를 스택에 저장하고 child노드가 현재 노드가된다. */
				pStackTop = PushlntoStack(pStackTop, pCurr);
				pCurr = pChild;
			}

		} while (!bFound);

		if (bFound != 1){
			/* key값이 Tree에 존재하지 않는다면 */
			printf("[Deletion] Key (%c) does not exist in the B-Tree\n", nKey);
		}

		///////
		else {
			/* 리프 노드에 있지않은 경우 */
			/* 이 경우，pCurr는 NULL이 아니고，
			최소한 pCurr의 leftmost child도 NULL이 아님 */

			if (pCurr && pCurr->pChild[0]) { /* 선행자 찾기 */
				pChild = NULL;
				nPos = 0;
				for (i = 0; i < DEGREE - 1; ++i){
					if (nKey == pCurr->nElm[i]){
						/* pCurr이 리프 노드가 아니 므로
						선행자는 pCurr내의 key값의 좌측 child의
						맨 우측 자손의 맨 우측 key값이 됨 */
						nPos = i;
						pChild = pCurr->pChild[i];
						break;
					}
				}

				if (pChild)
					pStackTop = PushlntoStack(pStackTop, pChild);
				/* 경로는 stack에 저장 */
				/* 경로는 pCurr까지 동일하므토, pStackTop에 첨가 */
				while (pChild) {
					bFound = 0;  /* 선행 자의 pos */
					/* pChild에서 가장 큰 key를 찾고，
					해당 노드의 우측child 따라 검색*/
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
					else {    /* 리프 노드 */
						/* key를 swap 하고, loop 탈출 */
						nTemp4 = pChild->nElm[bFound];
						pChild->nElm[bFound] = pCurr->nElm[nPos];
						pCurr->nElm[nPos] = nTemp4;
						pCurr = pChild;
						break;
					}
				}
			}
			/* key를 삭제하고 shift */
			nPos = 0;
			for (i = 0; i < DEGREE - 1; ++i){
				if (pCurr->nElm[i] == nKey){
					nPos = i;
					break;
				}
			}
			if (nPos == DEGREE - 2){
				/* 마지막 key값이라면 그냥 삭제*/
				pCurr->nElm[nPos] = 0;
			}
			else {
				////444
				for (i = nPos + 1; i < DEGREE - 1; ++i) {
					/* 이동시킨다 */
					pCurr->nElm[i-1] = pCurr->nElm[i];
				}
				pCurr->nElm[DEGREE - 2] = 0;
			}
			/* 트리  재조정 */
			bFinished = 0;
			do {
				if (pCurr->bRoot == 1 || GetElementCount(pCurr)
					>= ((DEGREE + 1) / 2 - 1)) {
					/* root node이거나 언더 플로가 발생하지 않으면 */
					bFinished = 1;
				}
				else {
					/*  재분배 가능여부 판별 */
					bFound = 0; /* 재분배 가능 여부 flag */
					nTemp = 0;         /* 0: impossible，1: left sibling，2: right sibling */
					pChild = NULL;
					/* root가 아니므로 parent 존재  */
					pNewNode = PeepStackTop(pStackTop);
					if (pCurr == pNewNode) {
						/* 현재 노드와 스택위에 쌓인 노드가 같을때 */
						pStackTop = PopFromStack(pStackTop);
						pNewNode = PeepStackTop(pStackTop);
					}
					if (pNewNode == NULL) 
						printf("Error  wrong Situation\n");
					nPos = -1;
					for (i = 0; i <= GetElementCount(pNewNode); ++i){
						if (pCurr == pNewNode->pChild[i])   {
							/* 부모노드를 찾는다 */
							nPos = i;
							break;
						}
					}

					if (nPos == -1)
						printf("Error, the Node doesnt match\n");
					/* Sibling을 찾아서 재분배가능 여부 결정 */
					if (nPos == 0){ /* 이 경우에는 무조건 우측 sibling 결정 */
						if (GetElementCount(pNewNode->pChild[nPos + 1])
				> ((DEGREE + 1) / 2 - 1)) {
							bFound = 1;
							nTemp = 2;
						}
					}
					else if (nPos == DEGREE - 1)  {
						/*  이 경우에는 무조건 좌측 sibling결정 */
						if (GetElementCount(pNewNode->pChild[nPos - 1])
			> ((DEGREE + 1) / 2 - 1)) {
							bFound = 1;
							nTemp = 1;
						}
					}
					else {  /* left sibling 결정 */
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
					if (bFound) { /* 재분배 가능 */
						/* TWOBNODE 사용하지않음  */
						if (nTemp == 1) {
							/* 좌측 sibling，parent와  합쳐 반으로 나누기
							- key값, child만 이동 */
							pChild = pNewNode->pChild[nPos - 1];
							nTemp3 = GetElementCount(pChild);
							nTemp2 = (nTemp3 - GetElementCount(pCurr)) / 2;
							/* 균형을 맞추기 위해 left sibling에서
							nTemp 수만큼 pCurr에 분배 */
							/* pCurr에서 nTemp2만큼 우측으로 shift */
							for (i = DEGREE - 2; i >= nTemp2; --i)  {
								pCurr->nElm[i] = pCurr->nElm[i - nTemp2];
								pCurr->pChild[i + 1] = pCurr->pChild[i + 1 - nTemp2];
							}
							pCurr->pChild[nTemp2] = pCurr->pChild[0];
							/*  부모 key값 내려 보내기 */
							pCurr->nElm[nTemp2 - 1] = pNewNode->nElm[nPos - 1];
							/* left sibling에서 nTemp2-1만큼 pCurr로 이 동 */
							for (i = 0; i<nTemp2 - 1; ++i)  {
								pCurr->nElm[nTemp2 - i - 2]
									= pChild->nElm[nTemp3 - i - 1];
								pCurr->pChild[nTemp2 - i - 1]
									= pChild->pChild[nTemp3 - i];
							}
							pCurr->pChild[0] = pChild->pChild[nTemp3 - nTemp2 + 1];
							/*  leftsibling에서 부모로 key 올려주고 제거 */
							pNewNode->nElm[nPos - 1]
								= pChild->nElm[nTemp3 - nTemp2];
							for (i = nTemp3 - 1; i>nTemp3 - nTemp2 - 1; --i){
								pChild->nElm[i] = 0;
								pChild->pChild[i + 1] = NULL;
							}
						}
						else {
							/*  우측 sibling, parent와 합쳐 반으로 나누기
							key값, child만 이동  */
							pChild = pNewNode->pChild[nPos + 1];
							nTemp3 = GetElementCount(pChild);
							nTemp2 = (nTemp3 - GetElementCount(pCurr)) / 2;
							/* 균형을 맞추기 위해 right sibling에서 nTemp
							수만큼 pCurr에 분배 */
							/* 부모 key값 내려 보내기 */
							pCurr->nElm[GetElementCount(pCurr)]
								= pNewNode->nElm[nPos];
							/* right sibling에서 nTemp2-1 만큼 pCurr로 이동 */
							for (i = 0; i < nTemp2 - 1; ++i) {
								pCurr->nElm[GetElementCount(pCurr) + i + 1]
									= pChild->nElm[i];
								pCurr->pChild[GetElementCount(pCurr) + i + 1]
									= pChild->pChild[i];
							}
							pCurr->pChild[GetElementCount(pCurr) + nTemp2 - 1]
								= pChild->pChild[nTemp2 - 1];
							/* right sibling에서 부모로 key 올려주고 left shift */
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
						if (nTemp == 1)  { /* left sibling과 합병 */
							pChild = pNewNode->pChild[nPos - 1];
							nTemp3 = GetElementCount(pChild);
							nTemp2 = GetElementCount(pCurr);
							/*  parent 합병  */
							pChild->nElm[nTemp3] = pNewNode->nElm[nPos - 1];
							pChild->pChild[nTemp3 + 1] = pCurr->pChild[0];
							/* pCurr의 key와 child를 left sibling에 합병 */
							for (i = 0; i < nTemp2; ++i) {
								pChild->nElm[i + nTemp3 + 1] = pCurr->nElm[i];
								pChild->pChild[i + nTemp3 + 2] = pCurr->pChild[i + 1];
								/* parent에서  key, child 삭제 */
							}
							for (i = nPos - 1; i < DEGREE - 1; ++i) {
								pNewNode->nElm[i] = pNewNode->nElm[i + 1];
								pNewNode->pChild[i + 1] = pNewNode->pChild[i + 2];
							}
							pNewNode->nElm[DEGREE - 2] = 0;
							pNewNode->pChild[DEGREE - 1] = NULL;
							pCurr = pNewNode;
						}
						else { /* right sibling고 합병 */
							pChild = pNewNode->pChild[nPos + 1];
							nTemp3 = GetElementCount(pChild);
							nTemp2 = GetElementCount(pCurr);
							/*  parent 합벙 */
							pCurr->nElm[nTemp2] = pNewNode->nElm[nPos];
							pCurr->pChild[nTemp2 + 1] = pChild->pChild[0];
							/*  right sibling의 key와 child를 pCurr에 합병*/
							for (i = 0; i < nTemp3; ++i) {
								pCurr->nElm[i + nTemp2 + 1] = pChild->nElm[i];
								pCurr->pChild[i + nTemp2 + 2] = pChild->pChild[i + 1];
							}
							/* parent에서 key, child 삭제 */
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
			/*  root에 key가 없으면 트리 레벨을 감소  */
			nTemp2 = GetElementCount(pRoot);
			if (nTemp2 == 0) {
				height--;
				if (pRoot->pChild[0]) {
					/*  정상인  경우*/
				}
					if (pRoot->pChild[1]){
						/*  비정상적인 경우 */
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

