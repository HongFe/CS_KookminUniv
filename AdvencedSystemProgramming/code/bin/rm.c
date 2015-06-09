/*------------------------------------------------------
고급시스템 프로그래밍 : Command [ rm ]

							국민대학교 컴퓨터공학과
							20093284 나홍철
-------------------------------------------------------*/
/*
*	 rm 명령어는 파일혹은 디렉토리를 제거하는 명령어다.
*	 -f 옵션 : 강제로 제거한다.
*	 -r 옵션 : 디렉토리 내용들 모두 제거한다.
*/

#include "minishlib.h"

#define IS_DIRECTORY 0
#define IS_FILE 1

int fFlag=FALSE,rFlag=FALSE;

/* 전달받은 위치의 디렉토리를 제거하는 함수다 */
int DeleteDir(char *path)
{
	
	DIR *dir;
	struct dirent *d;
	struct stat buf;	
	char name[MAX_SIZE];	/* 디렉토리 내부의 엔트리 이름을 절대경로로 저장한다. */
	char ans[MAX_SIZE];
	
	/* 전달받은 위치의 디렉토리 경로를 open한다. */
	if( (dir=opendir(path)) == NULL )
	{
		perror("opendir");
		exit(1);	
	}

	/* 디렉토리 내부 엔트리들을 읽는다. */
	while ( (d = readdir(dir)) )
	{
		/* '.','..' 은 제외한다. */
		if((strcmp(d->d_name,".")==0) || (strcmp(d->d_name,"..")==0) )
			continue;

		sprintf(name, "%s/%s", path, d->d_name); /* 절대 경로로 만든다. */

		if (stat(name,&buf) <0)
		{
			perror("Error Stat");
			continue;
		}
		/* 디렉토리라면 디렉토리 제거함수를 재귀적으로 수행한다. */
		if(S_ISDIR(buf.st_mode) )
		{
			/* 디렉토리 내부를 탐색 제거 할지를 결정한다. */
			if(fFlag==FALSE)
			{				
				printf("rm: descend into directory '%s'?[y/n] ",name);
				fgets(ans, MAX_SIZE, stdin); /* 스트랑을 입력 받는다. */
				/* 탐색 지우기 원치 않는다. */
				if(strcmp(ans,YES) != 0)
				{						
					continue;
				}
			}
			
			DeleteDir(name);	/* 디렉토리 경로를 주어 디렉토리 제거 함수를 재귀적으로 사용*/
			
			/* 만약 강제로 지우는 'f'옵션이 아니라면 물어본다. */
			if(fFlag==FALSE)
			{
				printf("rm: remove '%s'?[y/n] ",name);
				fgets(ans, MAX_SIZE, stdin); /* 스트랑을 입력 받는다. */
				/* 강제로 지우기 원치 않는다. */
				if(strcmp(ans,YES) != 0)
				{					
					continue;
				}
			}

			/* 디렉토리를 제거한다.*/
			if(remove(name) <0)
			{
				perror("Unlink Error");
				continue;
			}					
		}
		/* 디렉토리가 아닌 파일이라면 */
		else
		{
			/* 만약 강제로 지우는 'f'옵션이 아니라면 물어본다. */
			if(fFlag==FALSE)
			{
				printf("rm: remove '%s'?[y/n] ",name);
				fgets(ans, MAX_SIZE, stdin); /* 스트랑을 입력 받는다. */
				/* 강제로 지우지 않는다. */
				if(strcmp(ans,YES) != 0)
				{					
					continue;
				}
			}

			/* 파일을 제거한다.*/
			if(unlink(name) <0)
			{
				perror("Unlink Error");
				return 1;
			}	
		}			
	}
	closedir(dir);	

	return 0;
}


/*
*	RM 명령어
*/
int main(int argc, char **argv)
{
	char *optionTmp = argv[1]; /* 옵션이라고 임시로 지정 */
	int fileNum = 0; /* 전달받은 인자중 파일이 어느 인덱스부터 시작인가를 알려준다. */
	struct stat buf;
	char name[MAX_SIZE]; /* */
	char ans[MAX_SIZE];

	/* Option Check	*/
	if(argc > 1)
	{
		/* Existing Option	*/
		if(optionTmp[0] == '-')
		{
			fileNum=2;
			/* 'f' Option	*/
			if(strchr(optionTmp,F))	fFlag=TRUE;
			/* 'r' Option */
			if(strchr(optionTmp,smallR)) rFlag=TRUE;				
		}
		/* Not Existing Option */
		else 
		{
			fileNum=1;
		}
	}

	else 
	{		
		printf("rm:  Wrong Command\n");
		return 1;
	}
	while(fileNum < argc)
	{	
		realpath(argv[fileNum],name);
		/* Exist File */
		if(stat(name,&buf) == 0 )
		{
			/* name2 = Directory && rFlag option true */
			if(S_ISDIR(buf.st_mode) )
			{
				if(rFlag==TRUE)
				{
					/*rm: remove regular empty file ‘c/file1’? */
					if(fFlag==FALSE)
					{				
						printf("rm: descend into directory '%s'?[y/n] ",name);
						fgets(ans, MAX_SIZE, stdin); /* 스트랑을 입력 받는다. */
						/* Not over write */
						if(strcmp(ans,YES) != 0)
						{	

							fileNum++;						
							continue;
						}
					}
					DeleteDir(name); /* 디렉토리 제거 함수를 수행한다. */
				
					if(fFlag==FALSE)
					{
						printf("rm: remove '%s'?[y/n] ",name);
						fgets(ans, MAX_SIZE, stdin); /* 스트랑을 입력 받는다. */
						/* Not over write */
						if(strcmp(ans,YES) != 0)
						{	
							fileNum++;				
							continue;
						}
					}
			
					/* delete directory */
					if(remove(name) <0)
					{
						perror("rm : Unlink Error");
						continue;
					}	
				}				
				else
				{
					printf("rm : Can't delete Directory '%s'\n",name);
				}
			}
			/* name == file */
			else
			{	

				if(fFlag==FALSE)
				{
					printf("rm: remove '%s'?[y/n] ",name);
					fgets(ans, MAX_SIZE, stdin); /* 스트랑을 입력 받는다. */
					/* Not over write */
					if(strcmp(ans,YES) != 0)
					{					
						continue;
					}
				}
				/* 파일을 지운다. */
				if(unlink(name) <0)
				{
					perror("rm : Unlink Error");
					return 1;
				}	
			}		
		}
		else
		{
			printf("rm : File Not exist\n");
		}	

		fileNum++;	/* 다음 파일을 지운다. */
	}

	return 0;
	
}
