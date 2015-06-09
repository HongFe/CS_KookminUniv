/*------------------------------------------------------
��޽ý��� ���α׷��� : Command [ who ]

								���δ��б� ��ǻ�Ͱ��а�
								20093284 ��ȫö
-------------------------------------------------------*/
/*
*	 who ��ɾ�� ���� ���ӵǾ� �ִ� user���� ������, �͹̳�����,
*	���ӽð��� utmp������ �����Ͽ� �� �� �ִ�.
*/


#include "minishlib.h"

/* Time ouput standard */
char *outpT = "%F %H:%M";

/*
*	who ��ɾ�
*/
int main(int argc, char **argv)
{
	struct utmpx *utx; /* utmp ���� ����ü�̴� */
	struct tm *tm; /* time ����ü */
	char tBuf[20]; /* ���信 ���߾��� �ð��� �����Ѵ�. */
	time_t time; /* �ð� ���� */

	/* utmp ���Ͽ��� �о�� ��Ʈ���� �ϳ��ϳ� ó���Ѵ�. */
	while ((utx = getutxent()) != NULL)
	{
		/* user process�� ó���Ѵ�. */
		if(utx->ut_type != USER_PROCESS)
			continue;
		time = (time_t)utx->ut_tv.tv_sec;	/* �ð��� ó���Ѵ�. */
		tm = localtime(&time);				/* �ð������� �����´�. */
		strftime(tBuf, sizeof(tBuf), outpT, tm);	/* ���˿� ���߾� �ð��� ���� �Ѵ�. */

		printf("%s     %-8s      %s (:%d)\n"
		,utx->ut_user,utx->ut_line,tBuf,utx->ut_session);
	}
	return 0;
}
