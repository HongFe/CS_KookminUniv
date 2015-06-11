
/*//////////////////////볼링///////////////////////



국민대학교 컴퓨터공학부 3학년 나홍철


//////////////////////////////////////////////////////////*/
#include <stdio.h>
#include <string.h>

#define MAX_SIZE 22
#define MAX_FRAME 10
#define STACK_SIZE 10

#define MISS 0
#define SPARE 1
#define STRIKE 2

#define TRUE 1
#define FALSE 0

int stack[STACK_SIZE] = {0};
int stack_top = 0;

void push(int score){
   stack[stack_top++] = score;
}

int pop(){
   return stack[--stack_top];
}

int top_value(){
   return stack[stack_top-1];
}

int main(void){
   FILE *in, *out;
   int iTestNum = 0;
   int count = 0, i, j, temp;
   int arrBowling[MAX_SIZE] = {-1};
   int scores[MAX_FRAME] = {0}, flags[MAX_FRAME] = {0};
   int score, before, sum, frame;
   int isError = FALSE;

   in = fopen("input.txt", "r");

   fscanf(in, "%d", &iTestNum);
   while(iTestNum--){
      // ~ init variable
      count = 0, frame = 0, isError = FALSE; 
      stack_top = 0;
      memset(arrBowling, -1, MAX_SIZE*sizeof(int));
      memset(scores, 0, MAX_FRAME*sizeof(int));
      memset(flags, 0, MAX_FRAME*sizeof(int));

      // ~ get value
      fscanf(in, "%d", &count);
      for(i=0; i<count; i++){
         fscanf(in, "%d", &temp);
         arrBowling[i] = temp;
      }

      for(i=0; i<count; i++){
         score = arrBowling[i];
         
         // ~ Calcuate strike, spare sum
         for(j=0; j<frame; j++){
            if(flags[j]){
               scores[j]+=score;
               flags[j]--;
            }
         }

         // ~ Last Frame
         if(frame == 10){
            continue;
         }

         // ~ Not have Miss Spare
         if(stack_top == 0 ){
            // ~ Not Strike
            if(score == 2 || score == 1 || score == 0){
               push(score);
               scores[frame]+=score;
            }
            // ~ Strike
            else{ 
               scores[frame]+=score;
               flags[frame] = STRIKE;
               frame++;
            }
         }// ~ Have Miss Spare
         else if(stack_top == 1){
            before = top_value();
            sum = score + before;
            if(sum > 3){
               // ~ Exception 
               isError = TRUE;
               break;
            }else if(sum == 3){
               // Spare
               scores[frame]+=score;
               flags[frame] = SPARE;
               frame++;
            }else{
               // Miss
               scores[frame]+=score;
               flags[frame] = MISS;
               frame++;
            }
            pop();
         }         

      }

      if(isError){
         printf("error\n");
      }else{
         sum = 0;
         for(i=0; i<MAX_FRAME;i++){
            sum+=scores[i];
         }
         printf("%d\n", sum);
      }
   }

   return 0;
}