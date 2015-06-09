/*------------------------------------------------------
고급시스템 프로그래밍 : Command [ mv ]

							국민대학교 컴퓨터공학과
							20093284 나홍철
-------------------------------------------------------*/
/*
*	 cat 명령어는 입력받은 파일들을 출력하는 명령어다.
*	-b옵션은 공백을 제외한 스트링을 라인넘버와 함께 출력되며
*	-n옵션은 공백을 포함한 스트링을 라인넘버와 함께 출력한다.
*/

#include "minishlib.h"

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

/*
*	MV 명령어
*/
int main(int argc, char **argv)
{
	int fFlag=FALSE;
	char *optionTmp = argv[1];	/* 임시로 저장해둔 옵션 */
	char absoultepath[BUFSIZ];	/* 절대경로를 저장하는 스트링 */
	char mvfile[BUFSIZ];		/* 옮길 파일이름 */
	char ans[BUFSIZ];			/* 위치에 이미 파일이 존재할 때 덮어쓸지에 대한 답 */
	struct stat buf;			/* 파일 상태를 저장할 구조체 */
	
	int fbegin, fend;			/* 전달받은 인자들중 파일의 시작과 끝 인덱스 */
	int dirflag = 0, existflag = 0;	/* dirflag-> 마지막 인자값이 디렉토리라면 체크,
									existflag-> 마지막 인자값이 존재한다면 체크, 덮어쓸지를 물어볼때 사용 */

	fend = argc - 1;	/* 전달 받은 인자중 마지막 인덱스 */

	/* 옵션 체크 */
	if(argc > 2)
	{
		if(optionTmp[0] == '-')
		{
			fbegin = 2;
			/* 'f' Option */	
			if(strchr(optionTmp,F))	
				fFlag=TRUE;			
		}
		/*  Not Existing Option */
		else 
		{
			fbegin = 1;		
		}
	}
	else 
	{		
		printf("mv: Wrong Input\n");
		return 1;
	}
	
	/*last file is existed */
	if(stat(argv[fend],&buf) == 0)
	{
		/* last file is directory */
		if(S_ISDIR(buf.st_mode))
		{	
			existflag=1;
			dirflag=1;
		}
		
		/* last file is not directory */
		else
		{

		// ex) if "mv file1 file2 file3", but file3 is not directory. file3 is must be existed directory!
			if(fend - fbegin > 1) 
			{
				printf("mv: %s is not directory",argv[fend]);
				return 1;
			}

			existflag=1;
			dirflag=0;
		}
	}

	/* last file is not existed */
	else
	{
		// ex) if "mv file1 file2 file3", but file3 is not existed. file3 is must be existed directory!
		if(fend - fbegin > 1) 
		{
			printf("mv: %s is not directory",argv[fend]);
			return 1;
		}
		existflag=0;
		dirflag=0;	
	}

	/* move 'file1' to 'dir/file2' */
	if(dirflag)
	{		
		realpath(argv[fend],absoultepath);
		while (fbegin != fend)
		{
			sprintf(mvfile,"%s/%s",absoultepath,argv[fbegin]);
			/* Existing 'file2' */
			if( (stat(mvfile,&buf) == 0) && (fFlag==FALSE) )
			{
				printf("mv: overwrite \"%s/%s\"?[y/n] ",argv[fend],argv[fbegin]);
				/* Overwrite? */
				fgets(ans, BUFSIZ, stdin);
				if(strcmp(ans,"y") != 0)
				{
					fbegin++;
					continue;
				}		
			}
			/* 이동할 곳에 파일을 복사한다*/
			if (CopyFile(argv[fbegin],mvfile) == 1)
				printf("ERROR : [mv] copy error\n");
			

			/* 원래 위치릐 파일을 지운다.*/
			if(unlink(argv[fbegin]) <0)
			{
				perror("ERROR: [mv] Unlink");
				return 1;	
			}
			fbegin++;	/* 다음 파일을 수행한다. */
		}						
	}

	/* move 'file1' to 'file2' */
	else
	{
		/* Existing 'file2' */
		if(existflag && (fFlag==FALSE) )
		{
			printf("mv: overwrite \"%s\"?[y/n] ",argv[fend]);

			/* Overwrite? */
			fgets(ans, BUFSIZ, stdin); 
			if(strcmp(ans,"y") != 0)
			{
				fbegin++;
				return 0;
			}			
		}

		/* file1을 file2에 복사한다. */
		if (CopyFile(argv[fbegin],argv[fend]) == 1)
		{
			printf("ERROR : [mv] copy error\n");
			return 1;
		}

		/* file1을 지워 이동한 것처럼 보이게 한다. */
		if(unlink(argv[fbegin]) <0)
		{
			perror("ERROR: [mv] Unlink");
			return 1;	
		}
	}	
	return 0;	
}

