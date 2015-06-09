#include <stdio.h>
#include <malloc.h>
#include <string.h>

#define DEGREE 3	/*  B-Ʈ���� ���� 3�϶� */
//#define DEGREE 4	/*  B-Ʈ���� ���� 4�϶� */
#define MAX_ELEMENTS 1000	 /* �ԷµǴ� key�� �ִ��  */
#define DELETE_COUNT 10  /* ������ ������ ���Ҽ�*/

/* global variables */
FILE* pInputStream;	/* input ȭ���� �ڵ� */

struct NODE {
	/* B-Ʈ�� ��屸��  */
	int bRoot;
	char nElm[DEGREE - 1];
	void* pChild[DEGREE];
};
struct OVERFLOW_NODE    {
	/* B-Ʈ���� �����÷� ��� ���� */
	int bRoot;
	char nElm[DEGREE];
	void* pChild[DEGREE + 1];
};

struct STACK_NODE{
	/* stack�� ��� ���� */
	struct NODE* pCurr;
	void* pPrev;
};

struct NODE* CreateNode();
	/* ��� ����  */
	
struct OVERFLOW_NODE* CreateOverflowNode();
	/* �����÷� ��� ���� */

struct STACK_NODE* CreateStackNode();
	/* Stack Node �� ��   */

struct STACK_NODE* PushlntoStack(struct  STACK_NODE*  pStackTop, struct NODE*  pNode);
	/*	Stack�� push  */	

struct STACK_NODE*  PopFromStack(struct   STACK_NODE* pStackTop);
	/* Stack�� �� pop */

struct NODE* PeepStackTop(struct   STACK_NODE*  pStackTop);
	/* Stack��  top�� peep */
	
int CheckElementExist(struct  NODE* pNode, char key);
	/* ��忡 key�� �ִ��� �˻� */
	
int GetElementCount(struct  NODE*  pNode);
	/*  ����� ���Ҽ� ��ȯ */

void PrintNodeElement(struct NODE* pNode);
	/* ����� key �� ���  */