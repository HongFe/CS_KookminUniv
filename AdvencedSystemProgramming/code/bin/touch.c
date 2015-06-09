/*------------------------------------------------------
고급시스템 프로그래밍 : Command [ touch ]

								국민대학교 컴퓨터공학과
								20093284 나홍철
-------------------------------------------------------*/
/*
*	 touch명령어는 파일이 존재하지 않는다면 생성하고, 
*	존재한다면 기존파일의 변경시간을 변경하는 명령어다. 
*	t옵션이 주어진다면 주어진 시간으로 시간을 변경한다.
*/
#include "minishlib.h"

/*
*	TOUCH 명령어
*/
int main(int argc, char **argv)
{
	char *optionTmp = argv[1];	/* 임시로 옵션을 받는다. */
	char *ptr;			/* strtol시 변환되자 않는 스트링위치를 받는 곳 */
	int tFlag = FALSE;	/* time Flag로 touch시 입력받은 시간으로 변경할 때 사용한다. */
	int filenum = 1;	/* 받은 인자들중 만들려는 파일의 시작위치 인덱스다. */
	struct utimbuf ubuf;	/* 시간을 변경하기위한 시간 구조체다. */
	int i;	
	int t;	/* 입력받은 인자중 변경할 시간스트링을 정수형으로 변환 */
	int fd;	/* 파일의 존재여부를 open으로 확인한다. */

	/* 옵션을 체크한다. */
	if( argc > 1 )
	{
		if(optionTmp[0] == '-')
		{
			/* 't' Option */	
			if(strchr(optionTmp,T))
			{
				tFlag=TRUE;
				filenum = 3;	/* 받은 인자중 파일의 시작위치 인덱스다. */
				t = strtol(argv[2], &ptr, 10);	/* 입력받은 시간 스트링을 정수형으로 변경해준다.*/
				ubuf.actime = (time_t)t;	/* 접근시간을 변경해준다.*/
				ubuf.modtime = (time_t)t;	/* 변경시간을 변경해준다. */
			}
			
			/* 잘못된 옵션이라면 에러처리 */
			else
			{
				printf("touch: Wrong Option\n");
				return 1;
			}
		}

		/* 입력받은 파일들이 존재하지않는다면 생성하고 시간을 변경해준다.*/
		for(i = filenum; i<=argc; i++)
		{			
			fd = open(argv[i], O_RDONLY | O_CREAT | O_EXCL, 0644);	/* 없는 파일이라면 생성하고 있는 파일이라면 그냥 무시한다. */
				
			/* 시간을 변경하는 옵션이 주어지면 변경한 ubuf시간 값으로 파일의 inode시간정보를 수정한다. */
			if(tFlag)
				utime(argv[i], &ubuf);
			/* 시간을 변경하는 옵션이 주어지지 않으면 현재의 시간으로 변경한다. */
			else
				utime(argv[i], NULL);  
			close(fd);
		}
	}
	/* 단순히 명령어 하나만 인자로 전달받으면 에러다. */
	else 
	{
		printf("touch : Wrong Input\n");
		return 1;
	}
	return 0;	
}
