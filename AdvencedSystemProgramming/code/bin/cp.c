/*------------------------------------------------------
고급시스템 프로그래밍 : Command [ cp ]

							국민대학교 컴퓨터공학과
							20093284 나홍철
-------------------------------------------------------*/
/*
*	 cp 명령어는 파일혹은 디렉토리를 복사하는 명령어다.
*	 -f 옵션 : 강제로 복사한다.
*	 -R 옵션 : 디렉토리 내용들 모두 복사한다.
*/
#include "minishlib.h"


int fFlag=FALSE, RFlag=FALSE;

/* 파일을 입력받은 file1위치의 파일을 file2에 복사한다. */
int CopyFile(char *file1, char *file2)
{
	int fpRead, fpWrite;
	char charBuf[BUFSIZ];
	int nread;

	// reading file fd
	if ((fpRead = open(file1, O_RDONLY)) < 0)
	{
		perror("cp:  copy read file open");
		exit(-3);
	}

	// writing file fd
	if ((fpWrite = open(file2, O_WRONLY | O_CREAT | O_TRUNC, 0644)) <0){
		perror("cp: copy write file open");
		exit(-3);
	}

	// Copy reading file to writing file
	while ((nread = read(fpRead, charBuf, BUFSIZ))>0)
	if (write(fpWrite, charBuf, nread)< nread)
	{
		close(fpRead);
		close(fpWrite);
		exit(1);
	}
	close(fpRead);
	close(fpWrite);

	return 0;
}

/* 디렉토리를 복사하는 함수다. */
int CopyDir(char *fromDir,char *toDir)
{
	DIR *dir;
	struct dirent *fromd;
	struct stat buf;
	char fromAbsPath[MAX_SIZE],toAbsPath[MAX_SIZE];
	int mkdErr=0;
	char ans[MAX_SIZE];
	
	/* 복사 될 곳의 디렉토리를 생성한다 만약 존재하는 에러 이외의 에러는 에러다.*/
	if(mkdir(toDir, 0755) < 0 && errno != EEXIST )
	{
		perror("cp: mkdir error");
		return 1;
	}

	/* 전달받은 복사 할 디렉토리를 open한다. */
	if( (dir=opendir(fromDir)) == NULL )
	{
		perror("cp: opendir");
		exit(1);	
	}

	/* 복사 할 디렉토리의 엔트리를 읽어 복사될곳에 복사해주기위한 작업을 한다. */
	while ( (fromd = readdir(dir)) != NULL )
	{ 
		if((strcmp(fromd->d_name,".")==0) || (strcmp(fromd->d_name,"..")==0) )
			continue;

		sprintf(toAbsPath,"%s/%s",toDir,fromd->d_name);
		sprintf(fromAbsPath,"%s/%s",fromDir,fromd->d_name);

		/* 복사 할 파일의 상태를 확인한다. */
		if (stat(fromAbsPath,&buf) <0)
		{
			perror("cp : Error Stat");
			continue;
		}
		/* 만약 디렉토리라면 디렉토리 복사 함수를 재귀적으로 호출한다. */
		if(S_ISDIR(buf.st_mode))
		{				
			CopyDir(fromAbsPath,toAbsPath);
			continue;
		}

		/* 디렉토리가 아닌 파일이면서 복사될 곳에 이미 같은 이름이 존재한다면 
		복사할 지를 물어본다. */
		else if(!fFlag && (stat(toAbsPath,&buf) == 0 ))
		{
			printf("cp: overwrite '%s'? [y/n]",toAbsPath);
			fgets(ans, MAX_SIZE, stdin); /* 스트랑을 입력 받는다. */
			
			/* 붙여넣기 하지 않는다. */
			if(strcmp(ans,YES) != 0)
			{						
				continue;
			}		
		}		
		CopyFile(fromAbsPath, toAbsPath);	/* 파일 복사를 진행한다. */
	}
		
	closedir(dir);
	return 0;
}

// cp Command
int main(int argc, char **argv)
{
	
	char *optionTmp = argv[1]; // Temporally get option
	char *name1,*name2;
	char n1RealPath[MAX_SIZE],n2RealPath[MAX_SIZE],n2tmpPath[MAX_SIZE];
	int name2Exist=1;
	char *ptr,*name1Ptr; // For Name Split
	struct stat buf1,buf2;
	char charBuf[BUFSIZ];
	char ans[MAX_SIZE];
	int beginFile;

	/* 옵션을 체크한다. */
	if(argc > 2)
	{
		if(optionTmp[0] == '-')
		{
			beginFile = 2;
			name1 = argv[beginFile];
			name2 = argv[argc-1];
			/* 'f' Option */	
			if(strchr(optionTmp,F))	fFlag=TRUE;	
			/* 'R' Option */
			if(strchr(optionTmp,bigR)) RFlag=TRUE;		
		}
		/* 옵션이 존재하지 않는다면 시작파일은 넘겨받은 인자중
		명령어 다음이며 여러개의 파일 인자값들이 존재할 때는
		마지막 인자는 디렉토리이어야 한다. */
		else 
		{
			beginFile = 1;
			name1 = argv[beginFile];
			name2 = argv[argc-1];		
		}
	}

	else 
	{
		printf("cp : Wrong Command\n");
		return 1;
	}

	/* 단 두개의 인자만 존재한다면 한파일을 다른 파일로 복사하는것이지만
	여러개의 파일이 존재하는 경우 여러인자 파일들이 마지막 한 디렉토리에 복사되는 것이다. */
	while( strcmp(name1,name2) != 0)
	{
		realpath(name1, n1RealPath);		/* 절대경로로 치환한다.*/
		realpath(name2, n2RealPath);		/* 절대경로로 치환한다.*/

		if (stat(n1RealPath, &buf1) < 0) {
			printf("cp : name1 not exist\n");
			return 1;
		}
		if (stat(n2RealPath, &buf2) < 0) {
			name2Exist = 0;  /* name2 not exist */
		}

	
		/* Name1 is directory */
		if (S_ISDIR(buf1.st_mode))
		{	
			/* Directory Copy option true */
			if(RFlag)
			{			
				/* Name2 is not directory */
				if (!S_ISDIR(buf2.st_mode))
				{				
					printf("cp : cannot overwrite non-directory\n");
					return 1;
				}

				/* Name2 is directory or not exist. */
				else
				{
					CopyDir(n1RealPath, n2RealPath);	/* 디렉토리 복사 함수를 수행한다. */
				}
			}
			
			/* Directory Copy option false */
			else
			{
				printf("cp : omitting directory ‘%s’\n",name1);				
				return 1;
			}
			
		} 
	 
		/* Name1 is not directory */
		else
		{
			/* Name2 is directory => renew dir(name2) to file(name2/name1) */
			if (S_ISDIR(buf2.st_mode))
			{	
			
					realpath(name1,n1RealPath);
			
					/* split just name */
					ptr = strtok( name1 , "/"); /* Split name1 -> Just File name */
					name1Ptr = ptr;
					while(ptr != NULL)
					{		
						name1Ptr = ptr;		
						ptr = strtok(NULL,"/");				
					}		
			
					sprintf(n2tmpPath,"%s/%s",n2RealPath,name1Ptr);

					/* 'n1RealPath' -> 'n1RealPath/name1'*/
					if (stat(n2tmpPath, &buf2) < 0) {
						name2Exist = 0;  // name2 not exist
					}				
				
					/* No Force Copy Option && name2 exist file */
					if(!fFlag && name2Exist )
					{
						printf("cp: overwrite '%s'? [y/n]",n2tmpPath);
						fgets(ans, MAX_SIZE, stdin); /* 스트랑을 입력 받는다. */
						/* 강제 복사 하지 않는다. */
						if(strcmp(ans,YES) != 0)
						{
							beginFile++;	
							name1=argv[beginFile];
							continue;
						}
					
					}	
					CopyFile(n1RealPath, n2tmpPath);	/* 파일을 복사한다. */	

			}
			/* name1 is file name2 is file */
			else{
				if( ((argc -1) - beginFile) != 1)
				{				
					printf("cp : ‘%s’ is not directory\n",name2);
					return 1;	
				}
				
				/* No Force Copy Option && name2 exist file */
				if(!fFlag && name2Exist )
				{
					printf("cp: overwrite '%s'? [y/n]",n2RealPath);
					fgets(ans, MAX_SIZE, stdin); /* 스트랑을 입력 받는다. */
					/* 강제 복사 하지않는다. */
					if(strcmp(ans,YES) == 0)
					{						
						return 0;
					}
				}	
				CopyFile(n1RealPath, n2RealPath);	/* 파일을 복사한다. */
				return 0;
			}

		}						
		beginFile++;	/* 다음 인자를 복사한다. */
		name1=argv[beginFile];/* 다음 인자를 복사한다. */
	}
	return 0;
}
