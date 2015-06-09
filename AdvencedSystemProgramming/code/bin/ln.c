/*------------------------------------------------------
고급시스템 프로그래밍 : Command [ ln ]

							국민대학교 컴퓨터공학과
							20093284 나홍철
-------------------------------------------------------*/
/*
*	 ln 명령어는 파일혹은 디렉토리의 hard link, soft link를 설정하는
*	명령어다.
*	 -s 옵션 : soft link를 설정한다.
*/

#include "minishlib.h"

/* 
* LN 명령어
*/
int main(int argc, char **argv)
{
	struct stat buf; // Stat structure
	char *optionTmp = argv[1]; // Temporally get option
	int sFlag = FALSE; // Option 's'(symbolick link) flag
	int i;
	char *name1,*name2; //  링크 할 인자(name1)와 링크 될 인자(name2)

	// Option Check
	if(argc > 2)
	{
		// Existing Option
		if(optionTmp[0] == '-')
		{
			name1 = argv[2];
			name2 = argv[3];
			// 's' Option	
			if(strchr(optionTmp,S))	sFlag=TRUE;			
		}
		// Not Existing Option
		else 
		{
			name1 = argv[1];
			name2 = argv[2];		
		}
	}
	// Lack of arguements
	else 
	{
		printf("ERROR : [ln] Wrong Command\n");
		return 1;	
	}

	// Symbolic Link
	if(sFlag) symlink(name1,name2);

	// Hard Link
	else link(name1,name2);	
		
	return 0;
}
