#include <stdio.h>
#include <malloc.h>
#include <string.h>

#define DEGREE 3	/*  B-트리의 차수 */
//#define DEGREE 4	/*  B-트리의 차수 */
#define MAX_ELEMENTS 1000	 /* 입력되는 key의 최대수  */

/* global variables */
FILE* pInputStream;	/* input 화일의 핸들 */
FILE* pOutputStream; /* output 화일의 핸들  */

struct NODE {    /* B-트리 노드구조  */
	int bRoot;
	char nElm[DEGREE - 1];
	void* pChild[DEGREE];
};
struct OVERFLOW_NODE    {   /* B-트리의   오버플로 노드 구조 */
	int bRoot;
	char nElm[DEGREE];
	void* pChild[DEGREE + 1];
};

struct STACK_NODE{ /* stack의 노드 구조  */
	struct NODE* pCurr;
	void* pPrev;
};

struct NODE* CreateNode(){
	/* 노드 생성  */
	struct NODE* pNode;
	pNode = (struct NODE*)malloc(sizeof(struct  NODE));
	memset(pNode, 0, sizeof(struct  NODE));
	return pNode;
}
struct OVERFLOW_NODE*  CreateOverflowNode()  {
	/* 오버플로 노드 생성 */
	struct OVERFLOW_NODE*  pNode;
	pNode = (struct OVERFLOW_NODE*)malloc(sizeof(struct OVERFLOW_NODE));
	memset(pNode, 0, sizeof(struct OVERFLOW_NODE));
	return pNode;
}
struct STACK_NODE*  CreateStackNode(){
	/* Stack Node 생 성   */
	struct STACK_NODE*  pStackNode;
	pStackNode = (struct  STACK_NODE*)malloc(sizeof(struct  STACK_NODE));
	memset(pStackNode, 0, sizeof(struct STACK_NODE));
	return pStackNode;
}

struct STACK_NODE*  PushlntoStack    /*	Stack에 push  */
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
int CheckElementExist(struct  NODE* pNode, char key);
int GetElementCount(struct  NODE*  pNode);
void PrintNodeElement(struct NODE* pNode);
int btree();