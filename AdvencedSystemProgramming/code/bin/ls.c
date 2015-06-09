/*------------------------------------------------------
고급시스템 프로그래밍 : Command [ ls ]

								국민대학교 컴퓨터공학과
										20093284 나홍철
-------------------------------------------------------*/
/*
*	 ls명령어를 사용하면 현재의 path를 기준으로 존재하는 엔트리들의
*	정보를 화면에 출력할 수 있다.
*/

#include "minishlib.h"
/*	lFlag -> 긴리스트의 포맷으로 출력, aFlag -> 경로안의 모든 내용을 출력한다,
	rFlag -> 정렬의 순서를 역방향으로 한다, RFlag -> 현재 디렉터리를 기준으로 모든 하위의 디렉터리를 출력 */
int lFlag=FALSE, aFlag=FALSE, rFlag=FALSE, RFlag=FALSE;

/* Time ouput standard */
char *outpT = "%b %e %H:%M";


/* 숫자 퍼미션 모드를 인자로 받으면 인자로 받은 스트링변수에 반환하는 함수다 */
int StrMode(char pMode[], mode_t stmode)
{
	int mode;
	mode = stmode & S_IFMT; // Kind of Mode
	switch (mode)
	{
		// Directory
	case S_IFDIR:
		pMode[0] = 'd';
		break;

		// Block Device
	case S_IFBLK:
		pMode[0] = 'b';
		break;

		// Char Device
	case S_IFCHR:
		pMode[0] = 'c';
		break;

		// Symbol
	case S_IFLNK:
		pMode[0] = 'l';
		break;
	}
	if (stmode & S_ISUID)
		pMode[3] = 's';
	if (stmode & S_ISGID)
		pMode[6] = 's';
	if (stmode & S_ISVTX)
		pMode[9] = 's';

	if (stmode & S_IRUSR)
		pMode[1] = 'r';
	if (stmode & S_IWUSR)
		pMode[2] = 'w';
	if (stmode & S_IXUSR)
		pMode[3] = 'x';

	if (stmode & S_IRGRP)
		pMode[4] = 'r';
	if (stmode & S_IWGRP)
		pMode[5] = 'w';
	if (stmode & S_IXGRP)
		pMode[6] = 'x';

	if (stmode & S_IROTH)
		pMode[7] = 'r';
	if (stmode & S_IWOTH)
		pMode[8] = 'w';
	if (stmode & S_IXOTH)
		pMode[9] = 'x';

	return 0;
}


/* 입력받은 스트링을 정렬하는 함수다. */
int SortingStr(char **str, int sumfiles)
{
	int i, j;
	char tmp[MAX_SIZE];

	for (i = sumfiles - 2; i >= 0; i--)
	{
		for (j = 0; j <= i; j++)
		{
			if (strcmp(str[j], str[j + 1]) > 0)
			{
				strcpy(tmp, str[j]);
				strcpy(str[j], str[j + 1]);
				strcpy(str[j + 1], tmp);
			}
		}
	}
	return 0;
}

/*
*	디렉토리 파일리스트와 총 갯수를 입력받으면
*	리스트의 내용을 포멧에 맞추어 출력한다.
*/
void EntryPrint(char **filelist,int sumfiles)
{	
	struct stat buf;		/* 파일의 상태 저장 버퍼 */
	char tBuf[TIME_SIZE];	/* 시간정보를 포멧에 맞추어 저장할 버퍼 */
	struct tm *tm;			/* 시간정보를 저장할 버퍼 */
	char pMode[CHAR_PERM_SIZE]; /* 퍼미션 모드 스트링 */
	struct passwd *uName;	/* 패스워드 파일을 참조할 구조체 */
	struct group *gName;	/* 그룹정보를 참조할 구조체 */
	char file[MAX_SIZE];	/* 입력받은 엔트리 하나를 임시로 저장할 스트링 변수 */
	int i;

	/*	입력받은 엔트리의 갯수만큼 반복적으로 
	각 엔트리의 정보를 flag에 맞게 입력받아 출력한다. */
	for(i=0;i<sumfiles;i++)
	{ 
		/* 정렬의 순서를 역방향으로 할때 뒤의 엔트리부터 검사한다. */
		if(rFlag)
			strcpy(file,filelist[sumfiles-1-i]);
		else
			strcpy(file,filelist[i]);

		/* 경로안에 모든 내용을 출력하지 않을 때는 "."과".."은 제외한다.  */
		if(!aFlag && ((strcmp(file, ".")==0) || (strcmp(file, "..") ==0)) )
			continue;

		/* 긴 리스트의 포맷으로 출력할때 */
		else if(lFlag)
		{
			/* 파일의 상태를 불러온다 */
			if (stat(file,&buf) == -1) 
			{
				perror("ls : \n");
				exit(1);
			}
			tm = localtime(&buf.st_mtime);	/* 파일의 수정시간을 불러온다. */
			strftime(tBuf, sizeof(tBuf), outpT, tm);	/* 불러온 시간을 포맷에 맞추어 저장한다. */

			memset(pMode,'-',9 * sizeof(char));
			pMode[10]='\0';

			StrMode(pMode, buf.st_mode);	/* 파일의 퍼미션 모드를 입력하여 스트링으로 반환받는 함수다. */

			uName = getpwuid(buf.st_uid);	/* 사용자의 User Name */
			gName = getgrgid(buf.st_gid);	/* 사용자의 Group Name */

			printf("\n%s %2d %3s %3s %6d %s %s"
			,pMode,(int)buf.st_nlink,uName->pw_name, gName->gr_name
			, (int)buf.st_size, tBuf, file);		/* 포맷에 맞추어 출력한다. */
		}
			
		/* 리스트 포맷이 아니라면 파일 이름만 출력한다. */
		else
		{
			printf("%s  ",file);
		}
	}
	printf("\n\n");
}


/* 
*	디렉토리를 인자로 입력받으면 해당 디렉토리의 엔트리를 검사하여
*	디렉토리가 존재한다면 같은함수 recursive하게 호출되는 함수다. 
*/
int EntrySearch(char *dirpath)
{
	DIR *dir;			/* 입력받은 디렉토리의 정보를 담는다. */
	struct dirent *d;	/* 디렉토리의 엔트리 정보를 입력받는다. */
	struct stat buf;	/* 파일 정보를 저장한다. */
	int sumfiles = 0, i;	/* 디렉토리 내부의 엔트리 총 갯수다. */
	char **filelist;	/* 엔트리의 이름이 스트링들로 저장된다. */
	char absoultepath[MAX_SIZE];	/* 파일의 절대경로를 저장한다. */

	/* 입력받은 인자 디렉토리를 open한다. */
	if( (dir=opendir(dirpath)) == NULL )
	{
		perror("ls : opendir");
		exit(1);	
	}

	/* 엔트리를 입력받을 메모리를 할당한다. */
	if ( (filelist = (char**)malloc(sizeof(char*) * MAX_SIZE) ) == NULL)
	{
		perror("ls : mem faild\n");
		exit(1);
	}

	/* open된 디렉토리의 엔트리를 읽는다.*/
	while ( (d = readdir(dir)) )
	{	/* 엔트리를 저장할 메모리를 할당한다. */
		if ((filelist[sumfiles] = (char*)malloc(sizeof(char) * MAX_SIZE) ) == NULL)
		{
		    perror("ls : mem faild\n");
		    exit(1);
		}
		strcpy(filelist[sumfiles], d->d_name);	/* 엔트리 이름을 할당된 메모리에 복사한다.*/
		sumfiles++;  /* 엔트리 갯수를 하나 추가한다. */
	}
	closedir(dir);	/* 디렉토리를 닫아준다.*/

	SortingStr(filelist, sumfiles);	/* 입력받은 엔트리들을 알파벳순으로 정렬하는 함수다.*/
	EntryPrint(filelist, sumfiles);	/* 입력받은 엔트리들을 출력하는 함수다. */

	/* 하위디렉토리모두 출력을 한다면 */
	if(RFlag)
		/* 일일히 모든 엔트리를 검사하여 '.'와'..'가 아니고 디렉토리라면 디렉토리의 절대경로를 출력한 후 
		엔트리 검사 함수를 recursive하게 호출하여 검사한다. */
		for(i=0;i<sumfiles;i++)
		{	sprintf(absoultepath,"%s/%s",dirpath,filelist[i]);
			if((strcmp(filelist[i],".")!=0) && (strcmp(filelist[i],"..")!=0) && (stat(absoultepath,&buf) == 0))
			{			
				if(S_ISDIR(buf.st_mode))
				{	
					printf("%s : \n",absoultepath);
					EntrySearch(absoultepath);
				}
			}
		}
	/* 동적 메모리를 모두 해제한다.*/
	for(i=0;i<sumfiles;i++)	
		free(filelist[i]);
	free(filelist);
	return 0;
}

/*
*	LS COMMAND
*/
int main(int argc, char **argv)
{
	char *optionTmp;	/* 옵션스트링을 임시로 저장한다. */
	int numfile;		/* 입력받은 인자의 어느 부분부터 탐색하고자 하는 위치인지 인덱스를 알려준다 */
	int noin = FALSE;	/* 전달받은 인자가 있다면 현재위치를 탐색한다. */
	char *curD = getcwd(NULL, BUFSIZ); /* 현재의 디렉토리를 받는다. */

	/* 옵션의 존재를 검사한다. */
	if( argc > 1 )
	{
		optionTmp = argv[1];
		if (optionTmp[0] == '-')
		{
			/* 'a' Option */
			if (strchr(optionTmp, A))	aFlag = TRUE;
			/*  'l' Option */
			if (strchr(optionTmp, L))	lFlag = TRUE;
			/*  'R' Option */
			if (strchr(optionTmp, bigR)) RFlag = TRUE;
			/*  'r' Option */
			if (strchr(optionTmp, smallR)) rFlag = TRUE;

			numfile = 2;

			/* 옵션만 있고 입력값이 없다면 현재 위치를 탐색한다. */
			if (argc == 2)
			{
				noin = TRUE;
				numfile = 1;
			}
		}
		/* 전달받은 인자들이 존재한다. */
		else
		{
			numfile = 1;	/* 입력값은 전달받은 인자의 2번째 인덱스부터. */
		}
	}
	/* 전달받은 인자가 없다면 현재 위치를 탐색한다. */
	else
	{
		numfile = 0;
		noin = TRUE;
	}

	while (numfile < argc)
	{		
		/* 전달받은 인자가 없다면 현재 위치를 탐색한다. */
		if (noin)
		{
			/* 하위 디렉토리 출력 포멧이라면 현재 위치도 출력한다. */
			if (RFlag)
				printf("%s : \n", curD);
			EntrySearch(curD);
		}

		else
		{
			/* 하위 디렉토리 출력 포멧이라면 현재 위치도 출력한다. */
			if (RFlag)
				printf("%s : \n", argv[numfile]);
			EntrySearch(argv[numfile]);	/* 엔트리 탐색을 시작한다. */
		}
		numfile++;
	}
	return 0;
}
