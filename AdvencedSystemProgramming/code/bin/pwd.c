/*------------------------------------------------------
고급시스템 프로그래밍 : Command [ pwd ]

							국민대학교 컴퓨터공학과
									20093284 나홍철
-------------------------------------------------------*/
/*
*	pwd명령어는 Current directory를 출력한다.
*/

#include "minishlib.h"

/*
*	PWD
*/
int main()
{
	char *curD;

	curD = getcwd(NULL, BUFSIZ);	/* Currnet Directory를 저장한다. */
	printf("Current Directory : %s\n", curD);

	return 0;
}
