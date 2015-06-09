/*------------------------------------------------------
��޽ý��� ���α׷��� : Command [ ls ]

								���δ��б� ��ǻ�Ͱ��а�
										20093284 ��ȫö
-------------------------------------------------------*/
/*
*	 ls��ɾ ����ϸ� ������ path�� �������� �����ϴ� ��Ʈ������
*	������ ȭ�鿡 ����� �� �ִ�.
*/

#include "minishlib.h"
/*	lFlag -> �丮��Ʈ�� �������� ���, aFlag -> ��ξ��� ��� ������ ����Ѵ�,
	rFlag -> ������ ������ ���������� �Ѵ�, RFlag -> ���� ���͸��� �������� ��� ������ ���͸��� ��� */
int lFlag=FALSE, aFlag=FALSE, rFlag=FALSE, RFlag=FALSE;

/* Time ouput standard */
char *outpT = "%b %e %H:%M";


/* ���� �۹̼� ��带 ���ڷ� ������ ���ڷ� ���� ��Ʈ�������� ��ȯ�ϴ� �Լ��� */
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


/* �Է¹��� ��Ʈ���� �����ϴ� �Լ���. */
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
*	���丮 ���ϸ���Ʈ�� �� ������ �Է¹�����
*	����Ʈ�� ������ ���信 ���߾� ����Ѵ�.
*/
void EntryPrint(char **filelist,int sumfiles)
{	
	struct stat buf;		/* ������ ���� ���� ���� */
	char tBuf[TIME_SIZE];	/* �ð������� ���信 ���߾� ������ ���� */
	struct tm *tm;			/* �ð������� ������ ���� */
	char pMode[CHAR_PERM_SIZE]; /* �۹̼� ��� ��Ʈ�� */
	struct passwd *uName;	/* �н����� ������ ������ ����ü */
	struct group *gName;	/* �׷������� ������ ����ü */
	char file[MAX_SIZE];	/* �Է¹��� ��Ʈ�� �ϳ��� �ӽ÷� ������ ��Ʈ�� ���� */
	int i;

	/*	�Է¹��� ��Ʈ���� ������ŭ �ݺ������� 
	�� ��Ʈ���� ������ flag�� �°� �Է¹޾� ����Ѵ�. */
	for(i=0;i<sumfiles;i++)
	{ 
		/* ������ ������ ���������� �Ҷ� ���� ��Ʈ������ �˻��Ѵ�. */
		if(rFlag)
			strcpy(file,filelist[sumfiles-1-i]);
		else
			strcpy(file,filelist[i]);

		/* ��ξȿ� ��� ������ ������� ���� ���� "."��".."�� �����Ѵ�.  */
		if(!aFlag && ((strcmp(file, ".")==0) || (strcmp(file, "..") ==0)) )
			continue;

		/* �� ����Ʈ�� �������� ����Ҷ� */
		else if(lFlag)
		{
			/* ������ ���¸� �ҷ��´� */
			if (stat(file,&buf) == -1) 
			{
				perror("ls : \n");
				exit(1);
			}
			tm = localtime(&buf.st_mtime);	/* ������ �����ð��� �ҷ��´�. */
			strftime(tBuf, sizeof(tBuf), outpT, tm);	/* �ҷ��� �ð��� ���˿� ���߾� �����Ѵ�. */

			memset(pMode,'-',9 * sizeof(char));
			pMode[10]='\0';

			StrMode(pMode, buf.st_mode);	/* ������ �۹̼� ��带 �Է��Ͽ� ��Ʈ������ ��ȯ�޴� �Լ���. */

			uName = getpwuid(buf.st_uid);	/* ������� User Name */
			gName = getgrgid(buf.st_gid);	/* ������� Group Name */

			printf("\n%s %2d %3s %3s %6d %s %s"
			,pMode,(int)buf.st_nlink,uName->pw_name, gName->gr_name
			, (int)buf.st_size, tBuf, file);		/* ���˿� ���߾� ����Ѵ�. */
		}
			
		/* ����Ʈ ������ �ƴ϶�� ���� �̸��� ����Ѵ�. */
		else
		{
			printf("%s  ",file);
		}
	}
	printf("\n\n");
}


/* 
*	���丮�� ���ڷ� �Է¹����� �ش� ���丮�� ��Ʈ���� �˻��Ͽ�
*	���丮�� �����Ѵٸ� �����Լ� recursive�ϰ� ȣ��Ǵ� �Լ���. 
*/
int EntrySearch(char *dirpath)
{
	DIR *dir;			/* �Է¹��� ���丮�� ������ ��´�. */
	struct dirent *d;	/* ���丮�� ��Ʈ�� ������ �Է¹޴´�. */
	struct stat buf;	/* ���� ������ �����Ѵ�. */
	int sumfiles = 0, i;	/* ���丮 ������ ��Ʈ�� �� ������. */
	char **filelist;	/* ��Ʈ���� �̸��� ��Ʈ����� ����ȴ�. */
	char absoultepath[MAX_SIZE];	/* ������ �����θ� �����Ѵ�. */

	/* �Է¹��� ���� ���丮�� open�Ѵ�. */
	if( (dir=opendir(dirpath)) == NULL )
	{
		perror("ls : opendir");
		exit(1);	
	}

	/* ��Ʈ���� �Է¹��� �޸𸮸� �Ҵ��Ѵ�. */
	if ( (filelist = (char**)malloc(sizeof(char*) * MAX_SIZE) ) == NULL)
	{
		perror("ls : mem faild\n");
		exit(1);
	}

	/* open�� ���丮�� ��Ʈ���� �д´�.*/
	while ( (d = readdir(dir)) )
	{	/* ��Ʈ���� ������ �޸𸮸� �Ҵ��Ѵ�. */
		if ((filelist[sumfiles] = (char*)malloc(sizeof(char) * MAX_SIZE) ) == NULL)
		{
		    perror("ls : mem faild\n");
		    exit(1);
		}
		strcpy(filelist[sumfiles], d->d_name);	/* ��Ʈ�� �̸��� �Ҵ�� �޸𸮿� �����Ѵ�.*/
		sumfiles++;  /* ��Ʈ�� ������ �ϳ� �߰��Ѵ�. */
	}
	closedir(dir);	/* ���丮�� �ݾ��ش�.*/

	SortingStr(filelist, sumfiles);	/* �Է¹��� ��Ʈ������ ���ĺ������� �����ϴ� �Լ���.*/
	EntryPrint(filelist, sumfiles);	/* �Է¹��� ��Ʈ������ ����ϴ� �Լ���. */

	/* �������丮��� ����� �Ѵٸ� */
	if(RFlag)
		/* ������ ��� ��Ʈ���� �˻��Ͽ� '.'��'..'�� �ƴϰ� ���丮��� ���丮�� �����θ� ����� �� 
		��Ʈ�� �˻� �Լ��� recursive�ϰ� ȣ���Ͽ� �˻��Ѵ�. */
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
	/* ���� �޸𸮸� ��� �����Ѵ�.*/
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
	char *optionTmp;	/* �ɼǽ�Ʈ���� �ӽ÷� �����Ѵ�. */
	int numfile;		/* �Է¹��� ������ ��� �κк��� Ž���ϰ��� �ϴ� ��ġ���� �ε����� �˷��ش� */
	int noin = FALSE;	/* ���޹��� ���ڰ� �ִٸ� ������ġ�� Ž���Ѵ�. */
	char *curD = getcwd(NULL, BUFSIZ); /* ������ ���丮�� �޴´�. */

	/* �ɼ��� ���縦 �˻��Ѵ�. */
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

			/* �ɼǸ� �ְ� �Է°��� ���ٸ� ���� ��ġ�� Ž���Ѵ�. */
			if (argc == 2)
			{
				noin = TRUE;
				numfile = 1;
			}
		}
		/* ���޹��� ���ڵ��� �����Ѵ�. */
		else
		{
			numfile = 1;	/* �Է°��� ���޹��� ������ 2��° �ε�������. */
		}
	}
	/* ���޹��� ���ڰ� ���ٸ� ���� ��ġ�� Ž���Ѵ�. */
	else
	{
		numfile = 0;
		noin = TRUE;
	}

	while (numfile < argc)
	{		
		/* ���޹��� ���ڰ� ���ٸ� ���� ��ġ�� Ž���Ѵ�. */
		if (noin)
		{
			/* ���� ���丮 ��� �����̶�� ���� ��ġ�� ����Ѵ�. */
			if (RFlag)
				printf("%s : \n", curD);
			EntrySearch(curD);
		}

		else
		{
			/* ���� ���丮 ��� �����̶�� ���� ��ġ�� ����Ѵ�. */
			if (RFlag)
				printf("%s : \n", argv[numfile]);
			EntrySearch(argv[numfile]);	/* ��Ʈ�� Ž���� �����Ѵ�. */
		}
		numfile++;
	}
	return 0;
}
