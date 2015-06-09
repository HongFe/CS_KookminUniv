#include <stdio.h>
#include <malloc.h>
#include <string.h>

#define DEGREE 3	/*  B-트리의 차수 3일때 */
//#define DEGREE 4	/*  B-트리의 차수 4일때 */
#define MAX_ELEMENTS 1000	 /* 입력되는 key의 최대수  */
#define DELETE_COUNT 10  /* 삭제시 제거할 원소수*/

/* global variables */
FILE* pInputStream;	/* input 화일의 핸들 */

struct NODE {
	/* B-트리 노드구조  */
	int bRoot;
	char nElm[DEGREE - 1];
	void* pChild[DEGREE];
};
struct OVERFLOW_NODE    {
	/* B-트리의 오버플로 노드 구조 */
	int bRoot;
	char nElm[DEGREE];
	void* pChild[DEGREE + 1];
};

struct STACK_NODE{
	/* stack의 노드 구조 */
	struct NODE* pCurr;
	void* pPrev;
};

struct NODE* CreateNode();
	/* 노드 생성  */
	
struct OVERFLOW_NODE* CreateOverflowNode();
	/* 오버플로 노드 생성 */

struct STACK_NODE* CreateStackNode();
	/* Stack Node 생 성   */

struct STACK_NODE* PushlntoStack(struct  STACK_NODE*  pStackTop, struct NODE*  pNode);
	/*	Stack에 push  */	

struct STACK_NODE*  PopFromStack(struct   STACK_NODE* pStackTop);
	/* Stack에 서 pop */

struct NODE* PeepStackTop(struct   STACK_NODE*  pStackTop);
	/* Stack의  top을 peep */
	
int CheckElementExist(struct  NODE* pNode, char key);
	/* 노드에 key가 있는지 검사 */
	
int GetElementCount(struct  NODE*  pNode);
	/*  노드의 원소수 반환 */

void PrintNodeElement(struct NODE* pNode);
	/* 노드의 key 값 출력  */