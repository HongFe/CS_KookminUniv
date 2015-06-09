/*------------------------------------------------------
고급시스템 프로그래밍 : Command [ who ]

								국민대학교 컴퓨터공학과
								20093284 나홍철
-------------------------------------------------------*/
/*
*	 who 명령어는 현재 접속되어 있는 user들의 계정명, 터미널정보,
*	접속시간을 utmp파일을 참조하여 알 수 있다.
*/


#include "minishlib.h"

/* Time ouput standard */
char *outpT = "%F %H:%M";

/*
*	who 명령어
*/
int main(int argc, char **argv)
{
	struct utmpx *utx; /* utmp 파일 구조체이다 */
	struct tm *tm; /* time 구조체 */
	char tBuf[20]; /* 포멧에 맞추어진 시간을 저장한다. */
	time_t time; /* 시간 변수 */

	/* utmp 파일에서 읽어온 엔트리를 하나하나 처리한다. */
	while ((utx = getutxent()) != NULL)
	{
		/* user process만 처리한다. */
		if(utx->ut_type != USER_PROCESS)
			continue;
		time = (time_t)utx->ut_tv.tv_sec;	/* 시간을 처리한다. */
		tm = localtime(&time);				/* 시간정보를 가져온다. */
		strftime(tBuf, sizeof(tBuf), outpT, tm);	/* 포맷에 맞추어 시간을 정의 한다. */

		printf("%s     %-8s      %s (:%d)\n"
		,utx->ut_user,utx->ut_line,tBuf,utx->ut_session);
	}
	return 0;
}
