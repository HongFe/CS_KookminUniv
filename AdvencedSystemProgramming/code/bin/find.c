/*------------------------------------------------------
��޽ý��� ���α׷��� : Command [ find ]

							���δ��б� ��ǻ�Ͱ��а�
							20093284 ��ȫö
-------------------------------------------------------*/
/*
*	 find ��ɾ�� ã���� �ϴ� ���ϰ� ���丮�� 
*	currnet working directory�� ���� ã���ش�.
*	-name �ɼǻ��� �ش��ϴ� �̸��� ���Ϲ� ���丮�� ã���ش�.
*	-type �ɼǻ��� �ش��ϴ� Ÿ���� ���Ϲ� ���丮�� ã���ش�.
*	-user �ɼǻ��� �ش��ϴ� user�� ���Ϲ� ���丮�� ã���ش�.
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

/* ���丮 ���θ� ã�ƺ��� �Լ���. */
int FindEntry(char *dirPath, char *findName)
{
	DIR *dir;
	struct dirent *d;
	struct stat buf;
	struct passwd *uName;	/* password ������ ������ ������ ����ü�̴�. */
	char absPath[MAX_SIZE];	/* �����θ� ������ ��Ʈ��*/
	char mode;

	/* ���޹��� ���ڵ��丮�� ����. */
	if( (dir=opendir(dirPath)) == NULL )
	{
		perror("find: opendir");
		exit(1);	
	}
	/* ���丮 ���� ��Ʈ���� �˻��Ѵ�. */
	while ( (d = readdir(dir)) != NULL )
	{ 
		/* '.', '..' �� �˻����� �����Ѵ�. */
		if((strcmp(d->d_name,".")==0) || (strcmp(d->d_name,"..")==0) )
			continue;

		sprintf(absPath, "%s/%s", dirPath, d->d_name);	/* ���� ��θ� �����Ѵ�. */
		if (stat(absPath,&buf) == -1) 
		{
				perror("find:   ");
		}
		/* ���丮��� ���丮 �˻� �Լ��� ��������� ȣ���Ѵ�. */
		if(S_ISDIR(buf.st_mode))
		{
			FindEntry(absPath,findName);
		}

		/* 'name' �ɼ��� ��� */
		if( (optionFlag == OPTION_NAME) && (strcmp(d->d_name,findName) == 0) )
		{
			printf("%s\n",absPath);
		}

		/* 'user' �ɼ��� ��� */
		else if( optionFlag == OPTION_USER )
		{
			uName = getpwuid(buf.st_uid);	/* �ش��ϴ� uid���� user name��
											password file���� �����´�. */
			if( strcmp(uName->pw_name,findName) == 0)
			{
				printf("%s\n",absPath);
			}
		}
		/* 'type' �ɼ��� ��� */
		else if( optionFlag == OPTION_TYPE )
		{
			/* Ÿ�� �˻縦 �����Ѵ�. */
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

			/* ã�����ϴ� Ÿ���� ã�Ҵٸ� */
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
*	find ��ɾ�
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
	
	FindEntry(curD, name);	/* �Էµ� ���丮�κ��� ã�����ϴ� ���Ϲ� ���丮�� ã���Լ� */
	return 0;
	
}