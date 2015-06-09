/*------------------------------------------------------
고급시스템 프로그래밍 : Command [ cat ]

							국민대학교 컴퓨터공학과
									20093284 나홍철
-------------------------------------------------------*/
/*
*	 cat 명령어는 입력받은 파일들을 출력하는 명령어다.
*	-b옵션은 공백을 제외한 스트링을 라인넘버와 함께 출력되며
*	-n옵션은 공백을 포함한 스트링을 라인넘버와 함께 출력한다.
*/

#include "minishlib.h"

#define NL '\n'

// cat Command (complete)
int main(int argc, char **argv)
{
	char *optionTmp = argv[1];	/* 임시로 옵션을 저장한다. */
	int fileNum=0;				/* 여러개의 파일을 입력받을 경우 전달받은 인자에서 파일의 시작 인덱스다. */
	char charBuf[1];			/* 출력될 값을 한 바이트씩 읽어온다. */
	int nread, bNread;			/* 옵션에 사용된다. '\n' 하나뿐인 라인처리를 위함이다. */
	int bFlag = FALSE;	/* 플래그 옵션이다. bFlag -> 공백을 제외 한 각문장 앞에 번호를 표시한다. */
	int nFlag = FALSE;	/* 플래그 옵션이다. nFlag -> 공백을 포함 한 각문장 앞에 번호를 표시한다. */
	int redir = FALSE;	/* 인자에 입력받은 파일이 모두 없다면 redirection 된 것이다. */
	int fpRead;			/* 읽을 파일의 fd 번호다. */
	int count;			/* 라인넘버를 저장한다. */
	char str[BUFSIZ];	/* 한 바이트로 입력받은 캐릭터들이 저장되어 스트링이 되고 출력된다.*/
	
	/* 옵션을 체크한다. */
	if(argc > 1)
	{	
		if(optionTmp[0] == '-')
		{
			/* 'b' Option */	
			if(strchr(optionTmp,B)) bFlag=TRUE;
			
			/* 'n' Option */	
			else if(strchr(optionTmp,N)) nFlag=TRUE;
			
			/* 입력파일이없으므로 redirection 된것이다. */
			if(argc == 2)
			{
				redir=TRUE;
				fileNum = 1;	
				fpRead=0;	/* standard in */
			}
			/* 입력 파일이 존재한다. */
			else				
				fileNum = 2;			
		}
			
		/* 옵션이 존재 하지 않는다. */
		else 
		{
			fileNum = 1;	
		}			
	}
	/* 옵션과 입력파일이 존재하지 않는다. */
	else 
	{
		redir=TRUE;
		fileNum = 0;	
		fpRead=0;	/* standard in */
	}

	/* 입력받은 입력파일 모두 수행한다. */
	while(fileNum<argc)
	{
		/* redirection이 아닌 입력파일이 있을 경우 읽기 모드로 파일을 연다. */
		if (!redir)
			if( (fpRead = open(argv[fileNum],O_RDONLY)) < 0)
			{
				perror("cat : f open");
				return 1;
			}

		/* 옵션이 있을 경우 출력을 위한 라인넘버를 출력될 값 앞에 붙여준다. */
		if(bFlag || nFlag)
		{			
			count=1;
			sprintf(str,"%d  ",count);
		}

		bNread = 0;  /* 입력받은 바이트 수를 저장하며 한줄에 '\n'만 있는가를 판별한다. */
		
		/* 열기되거나 혹은 redirection된 파일에서 1바이트씩 읽어온다. */
		while( (nread=read(fpRead,charBuf,1))>0 ){
			bNread+=nread;
			
			/* b옵션이 설정되어 있고 '\n'이 한줄의 전부라면 '\n'만 출력한다. 
			b옵션의 경우 공백을 제외하고 출력하므로 다음 줄 처음 시작에 카운트를 붙여준다.*/
			if (bFlag && (charBuf[0]==NL) && bNread==1)
			{				
				printf("\t\n");
				sprintf(str,"%d  ",count);
				bNread=0;
				continue;
			}

			/* 한줄의 끝에 온다면 출력을 한다. */
			else if (charBuf[0]==NL)
			{	
				/* 옵션이 설정되어있다면 카운트가 붙여진 입력받은 스트링을 출력하고
				다음 입력받을 스트링에 카운트를 붙인다. */
				if(bFlag || nFlag)
				{							
					printf("\t%s\n",str);
					count++;	
					sprintf(str,"%d  ",count);
					bNread=0;
				}
				/* 아무 옵션이 없는 경우 카운트는 붙여있지 않으며 단시 스트링을 출력한다.*/
				else	
				{									
					printf("%s\n",str);		
					sprintf(str,"%c  ",0);
				}
			}
			/* 끝이 아니라면 캐릭터를 스트링에 붙인다. */
			else
				sprintf(str,"%s%c",str,charBuf[0]);			
		}
		close(fpRead);
		
		/* 다음 입력파일을 가져온다. */
		fileNum++;
	}		
	return 0;	
}
