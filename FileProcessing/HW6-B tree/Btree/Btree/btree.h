#include <stdio.h>
#include <malloc.h>
#include <string.h>

#define DEGREE 3	/*  B-Ʈ���� ���� */
//#define DEGREE 4	/*  B-Ʈ���� ���� */
#define MAX_ELEMENTS 1000	 /* �ԷµǴ� key�� �ִ��  */

/* global variables */
FILE* pInputStream;	/* input ȭ���� �ڵ� */
FILE* pOutputStream; /* output ȭ���� �ڵ�  */

struct NODE {    /* B-Ʈ�� ��屸��  */
	int bRoot;
	char nElm[DEGREE - 1];
	void* pChild[DEGREE];
};
struct OVERFLOW_NODE    {   /* B-Ʈ����   �����÷� ��� ���� */
	int bRoot;
	char nElm[DEGREE];
	void* pChild[DEGREE + 1];
};

struct STACK_NODE{ /* stack�� ��� ����  */
	struct NODE* pCurr;
	void* pPrev;
};

struct NODE* CreateNode(){
	/* ��� ����  */
	struct NODE* pNode;
	pNode = (struct NODE*)malloc(sizeof(struct  NODE));
	memset(pNode, 0, sizeof(struct  NODE));
	return pNode;
}
struct OVERFLOW_NODE*  CreateOverflowNode()  {
	/* �����÷� ��� ���� */
	struct OVERFLOW_NODE*  pNode;
	pNode = (struct OVERFLOW_NODE*)malloc(sizeof(struct OVERFLOW_NODE));
	memset(pNode, 0, sizeof(struct OVERFLOW_NODE));
	return pNode;
}
struct STACK_NODE*  CreateStackNode(){
	/* Stack Node �� ��   */
	struct STACK_NODE*  pStackNode;
	pStackNode = (struct  STACK_NODE*)malloc(sizeof(struct  STACK_NODE));
	memset(pStackNode, 0, sizeof(struct STACK_NODE));
	return pStackNode;
}

struct STACK_NODE*  PushlntoStack    /*	Stack�� push  */
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
int CheckElementExist(struct  NODE* pNode, char key);
int GetElementCount(struct  NODE*  pNode);
void PrintNodeElement(struct NODE* pNode);
int btree();