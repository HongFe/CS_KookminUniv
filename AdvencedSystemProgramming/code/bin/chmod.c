/*------------------------------------------------------
고급시스템 프로그래밍 : Command [ chmod ]

							국민대학교 컴퓨터공학과
							20093284 나홍철
-------------------------------------------------------*/
/*
*	 chmod 명령어로 해당 파일과 디렉토리의
*	퍼미션 모드를 변경해 줄수 있다.
*	이때 퍼미션모드는 숫자와 문자로 변경이 가능하다.
*	문자로 입력받는 다면 그 문자 스트링을 변경해주는 
*	함수를 통해 넘버 퍼미션으로 변경한다.
*/
#include "minishlib.h"

/* 스트링을 정수형 퍼미션 모드로 변경해준다. */
int StrChange(char *modeStr, int modeNum)
{
	int operFlag;	// '-' => 0 / '+' => 1 / '=' => 2
	int shiftNum;	/* 넘버 퍼미션에서 각각 user는 6비트, other는 3비트 왼쪽으로 이동한다.*/
	int i,tmp;
	
	for( i = 0 ; i<strlen(modeStr);i++)
	{
		tmp=0;
		/* 넘버 퍼미션에서 얼마나 쉬프트를 해야하는지 정한다. 
		총 9비트의 user, group, other 비트를 말하는것임. */
		if(modeStr[i] == 'u')
			shiftNum=6;
		else if(modeStr[i] == 'g')
			shiftNum=3;
		else if(modeStr[i] == 'o') 
			shiftNum=0;
		else 
		{ 
			printf("chmod: Wrong Command [u,g,o]\n"); 
			return 1; 
		}

		i++; 
		/* '-'는 기본 퍼미션에서 제거
		'+'는 기본 퍼미션에 추가
		'='는 기본 퍼미션을 무시하고 덮어쓰기가 된다. */
		if(modeStr[i]=='-') 
			operFlag = 0;
		else if(modeStr[i]=='+') 
			operFlag = 1;
		else if(modeStr[i]=='=') 
			operFlag = 2;
		else 
		{ 
			printf("chmod: Wrong Operator\n"); 
			return 1; 
		}
				
		i++; 
		/* 스트링에서 read, write, excute를 찾는다. */
		while(i<strlen(modeStr) )
		{
			if(modeStr[i]=='r')
			{
				tmp=tmp|4; 
				i++;
			}
			else if(modeStr[i]=='w') 
			{		
				tmp=tmp|2; 
				i++;
			}
			else if(modeStr[i]=='x')
			{		
				tmp=tmp|1; 
				i++;
			}
			else if(modeStr[i]==',') 
				break; /* 다음 모드 변경 스트링값을 찾는다. */
			else 
			{ 
				printf("chmod: Wrong Command\n"); 
				return 1; 
			}
		}
			/*	분석된 스트링의 모드 변경값을 실제 파일의 모드와 함게 알맞게 처리한다. */
			/* '-' */
			if(operFlag == 0) 
				modeNum = (modeNum & ~(tmp << shiftNum)); 
			/* '+' */
			else if(operFlag == 1)
				modeNum = (modeNum | (tmp << shiftNum));
			/* '=' */
			else if(operFlag == 2) 
			{
				/* 새로 덮어쓸 부분을 깨끗하게 지운다. */
				if (shiftNum == 6)
					modeNum=modeNum & 65087; // 177077 (octal)
				else if (shiftNum == 3)
					modeNum=modeNum & 65479; // 177707 (octal)
				else if (shiftNum == 0)
					modeNum=modeNum & 65528; // 177770 (octal)
					
				modeNum = (modeNum | (tmp << shiftNum) ); 
			}			
	}
	return modeNum;	/* 넘퍼 퍼미션 모드를 리턴한다. */
}

/*
*	chmod 명령어
*/
int main(int argc, char **argv)
{
	char *modeTmp = argv[1]; /* 임시로 모드라고 지정한다. */
	char *name;		/* 변경할 파일,폴더의 이름 */
	char *ptr;		/* 토큰 포인터다 */
	int modeNum;	/* 변경팔 퍼미션 넘버*/
	struct stat buf;

	/* 인자를 체크한다. */
	if(argc > 2)
	{		
		name = argv[2];
		/* 만약 임시로 모드라고 지정한 값이 숫자라면 퍼미션 모드는 숫자 */
		if(isdigit(modeTmp[0]))
		{
			modeNum = strtol(modeTmp, &ptr, 8); /* 퍼미션 숫자를 8진수로 변경한다. */
			
			/* 입력받은 넘버로 퍼미션모드를 바꾸어 준다 */
			if( chmod(name, modeNum)<-1)
				perror("chmod: Call to chmod failed.");					
		}
		/* 퍼미션 모드가 문자 */
		else
		{
			/* 파일이 존재 하지 않는다면 에러*/
			if (stat(name,&buf) == -1) 
			{
				printf("chmod: \n");
				exit(1);
			}
			modeNum = buf.st_mode;	/* 해당 파일의 퍼미션 모드를 할당받는다. */
			
			/* 퍼미션 모드를 변경한다. 이때 문자퍼미션 입력시 
			퍼미션 넘버를 반환해주는 함수를 사용하여 반환받아 사용한다. */
			if( chmod(name, StrChange(modeTmp,modeNum))<-1)
				perror("chmod: Call to chmod failed.");
		}
	}
	else 
	{
		printf("chmod:Wrong Command\n");
		return 1;
	}

	return 0;	
}
