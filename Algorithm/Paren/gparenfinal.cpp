/************************************************************************
* 
* 국민대학교 전자정보통신대학 컴퓨터공학부 3 학년
* 김성근 20093267, 김세훈 20093268, 나홍철 20093284
*
* '('가 전에 있는 지 여부를 기록하고 '('가 있으면 ')'을 만났을때
* '('를 기록을 제거하는 로직을 반복하면 해당 문제를 해결할 수 있습니다.
*
* '('이 존재 여부를 stack으로 기록하였는데 그 이유는 이문제를 확장하였을 경우
* '('뿐만 아니라 '{', '[' 에 대해서 제대로 쓰여져 있는지 여부를 판단하는 
* 문제를 생각하였습니다. 
*
* 따라서 '('를 만날 때마다 stack에 해당 '('을 push하여 두고 ')'을 만난경우
* pop을 하도록 구성하였고, 만약 '('존재하지 않고 pop을 할경우 error가 발생
* 하도록 algorithm을 작성하였습니다.
*
*************************************************************************/
#include <stdio.h>
#include <string.h>

#define TRUE 1   
#define FALSE 0

#define STACK_POP_ERROR -1   // POP ERROR 발생에 대한 상수
#define STACK_MAX_SIZE 52   // 문제에서 주어진 '(', ')'의 입력 크기가 50이기 때문에 52로주었습니다. 
                     // 절반의 크기만 주어질 수도 있지만 '('가 50번 반복 될 수도 있기 때문에 stack size 를 52로 하였습니다.

// ~ stack 전역 변수로 push, pop 기능이 사용가능하도록 구현
char stack[STACK_MAX_SIZE] = {0};
char top = 0;

/**
* @param parenthesis '(' 를 입력 받음
*/
void push(char parenthesis){
   if(top == STACK_MAX_SIZE){
      return;
   }
   stack[top++] = parenthesis;
}

/*
* @return stack 가장 높은 위치 값
*/
char pop(){
   if(top == 0){
      return STACK_POP_ERROR;
   }
   return stack[--top];
}

// ~ main

int main(void){
   FILE *in;
   int iTestCase;
   char chTemp; // input.txt에서 scan 받는 변수
   int flag; // 결과 값을 출력해줄 flag

   in = fopen("input.txt", "r");

   fscanf(in, "%d", &iTestCase);
   fscanf(in, "%c", &chTemp);
   while(iTestCase--){
      // ~ init variable
      flag = TRUE;
      top = 0;
      memset(stack, 0, sizeof(char) * STACK_MAX_SIZE);

      // ~ input scan and judge 
      while(TRUE){
         if(fscanf(in, "%c", &chTemp) == -1 || chTemp == 10){ // file에 \n을 만나거나 마지막인 경우 종료
            break;
         }

         if(chTemp == '('){ // '('을 만난 경우 push
            push(chTemp);
         }else{
            if(pop() == STACK_POP_ERROR){// ')'일 경우 pop() 과 동시에 error check
               flag = FALSE; // pop error 인경우 올바르지 않은 경우 ex) '())'
               // ~ scan
               while(TRUE){ // scan을 받으면서 동시에 체크하기 때문에 해당 줄을 모두 scan
                  if(fscanf(in, "%c", &chTemp) == -1 || chTemp == 10){
                     break;
                  }
               }
               break; // while 문 종료
            }
         }
      }

      if(top != 0){ // 모든 scan이 완료된후 stack에서 top은 0이어야함(아무것도 쌓여있지 않아야함.)
         flag = FALSE;//만약 쌓여있다면 올바르지 않은 경우 ex)'((())'
      }

      // ~ output print
      if(flag){
         printf("YES\n");
      }else{
         printf("NO\n");
      }
   }

   return 0;
}