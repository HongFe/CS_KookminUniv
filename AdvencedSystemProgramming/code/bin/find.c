/*------------------------------------------------------
고급시스템 프로그래밍 : Command [ find ]

							국민대학교 컴퓨터공학과
							20093284 나홍철
-------------------------------------------------------*/
/*
*	 find 명령어는 찾고자 하는 파일과 디렉토리를 
*	currnet working directory로 부터 찾아준다.
*	-name 옵션사용시 해당하는 이름의 파일및 디렉토리를 찾아준다.
*	-type 옵션사용시 해당하는 타입의 파일및 디렉토리를 찾아준다.
*	-user 옵션사용시 해당하는 user의 파일및 디렉토리를 찾아준다.
*/

#include "minishlib.h"

#define OPTION_NAME 0
#define OPTION_USER 1
#define OPTION_TYPE 2

#define DI 'd'
#define BL 'b' // Block Device
#define CH 'c' // Char Device
#define LN 'l' // Symbol
#define RE 'r' // Regular


int optionFlag=0; // 0-> name , 1->user , 2->type

/* 디렉토리 내부를 찾아보는 함수다. */
int FindEntry(char *dirPath, char *findName)
{
	DIR *dir;
	struct dirent *d;
	struct stat buf;
	struct passwd *uName;	/* password 파일의 내용을 저장할 구조체이다. */
	char absPath[MAX_SIZE];	/* 절대경로를 저장할 스트링*/
	char mode;

	/* 전달받은 인자디렉토리를 연다. */
	if( (dir=opendir(dirPath)) == NULL )
	{
		perror("find: opendir");
		exit(1);	
	}
	/* 디렉토리 내부 엔트리를 검사한다. */
	while ( (d = readdir(dir)) != NULL )
	{ 
		/* '.', '..' 는 검색에서 제외한다. */
		if((strcmp(d->d_name,".")==0) || (strcmp(d->d_name,"..")==0) )
			continue;

		sprintf(absPath, "%s/%s", dirPath, d->d_name);	/* 절대 경로를 저장한다. */
		if (stat(absPath,&buf) == -1) 
		{
				perror("find:   ");
		}
		/* 디렉토리라면 디렉토리 검색 함수를 재귀적으로 호출한다. */
		if(S_ISDIR(buf.st_mode))
		{
			FindEntry(absPath,findName);
		}

		/* 'name' 옵션의 경우 */
		if( (optionFlag == OPTION_NAME) && (strcmp(d->d_name,findName) == 0) )
		{
			printf("%s\n",absPath);
		}

		/* 'user' 옵션의 경우 */
		else if( optionFlag == OPTION_USER )
		{
			uName = getpwuid(buf.st_uid);	/* 해당하는 uid값의 user name을
											password file에서 가져온다. */
			if( strcmp(uName->pw_name,findName) == 0)
			{
				printf("%s\n",absPath);
			}
		}
		/* 'type' 옵션의 경우 */
		else if( optionFlag == OPTION_TYPE )
		{
			/* 타입 검사를 진행한다. */
			switch (buf.st_mode & S_IFMT) 
			{
			// Directory
			case S_IFDIR:	
				mode=DI;
				break;
			// Block Device
			case S_IFBLK:
				mode=BL;
				break;
			// Char Device
			case S_IFCHR:
				mode=CH;
				break;
			// Symbol
			case S_IFLNK:
				mode=LN;
				break;
			// Regular file
			default:
				mode=RE;
			}

			/* 찾고자하는 타입을 찾았다면 */
			if( mode==findName[0] )
			{
				printf("%s\n",absPath);
			}
		}			
	}
	closedir(dir);
	return 0;
}

/*
*	find 명령어
*/
int main(int argc, char **argv)
{
	char *optionTmp = argv[1]; /* Temporally get option */
	char *name;
	char *curD = getcwd(NULL, BUFSIZ); /* Current Directory */

	/* Option Check */
	if(argc > 1)
	{
		/* Existing Option */
		if(optionTmp[0] == '-')
		{
			name = argv[2];
			/* 'name' Option */	
			if(strcmp(optionTmp,"-name") == 0 ) optionFlag=0;
			/* 'user' Option */
			else if(strcmp(optionTmp,"-user") == 0 ) optionFlag=1;
			/* 'type' Option */
			else if(strcmp(optionTmp,"-type") == 0 ) optionFlag=2;		
		}
		/* Not Existing Option */
		else 
		{
			name = argv[1];		
		}	
	}
	else 
	{
		printf("find: Wrong Command\n");
		exit(1);
	}
	
	FindEntry(curD, name);	/* 입력된 디렉토리로부터 찾고자하는 파일및 디렉토리를 찾는함수 */
	return 0;
	
}