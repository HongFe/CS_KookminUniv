/*------------------------------------------------------
��޽ý��� ���α׷��� : Command [ mv ]

							���δ��б� ��ǻ�Ͱ��а�
							20093284 ��ȫö
-------------------------------------------------------*/
/*
*	 cat ��ɾ�� �Է¹��� ���ϵ��� ����ϴ� ��ɾ��.
*	-b�ɼ��� ������ ������ ��Ʈ���� ���γѹ��� �Բ� ��µǸ�
*	-n�ɼ��� ������ ������ ��Ʈ���� ���γѹ��� �Բ� ����Ѵ�.
*/

#include "minishlib.h"

/* ������ �Է¹��� file1��ġ�� ������ file2�� �����Ѵ�. */
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
*	MV ��ɾ�
*/
int main(int argc, char **argv)
{
	int fFlag=FALSE;
	char *optionTmp = argv[1];	/* �ӽ÷� �����ص� �ɼ� */
	char absoultepath[BUFSIZ];	/* �����θ� �����ϴ� ��Ʈ�� */
	char mvfile[BUFSIZ];		/* �ű� �����̸� */
	char ans[BUFSIZ];			/* ��ġ�� �̹� ������ ������ �� ������� ���� �� */
	struct stat buf;			/* ���� ���¸� ������ ����ü */
	
	int fbegin, fend;			/* ���޹��� ���ڵ��� ������ ���۰� �� �ε��� */
	int dirflag = 0, existflag = 0;	/* dirflag-> ������ ���ڰ��� ���丮��� üũ,
									existflag-> ������ ���ڰ��� �����Ѵٸ� üũ, ������� ����� ��� */

	fend = argc - 1;	/* ���� ���� ������ ������ �ε��� */

	/* �ɼ� üũ */
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
			/* �̵��� ���� ������ �����Ѵ�*/
			if (CopyFile(argv[fbegin],mvfile) == 1)
				printf("ERROR : [mv] copy error\n");
			

			/* ���� ��ġ�l ������ �����.*/
			if(unlink(argv[fbegin]) <0)
			{
				perror("ERROR: [mv] Unlink");
				return 1;	
			}
			fbegin++;	/* ���� ������ �����Ѵ�. */
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

		/* file1�� file2�� �����Ѵ�. */
		if (CopyFile(argv[fbegin],argv[fend]) == 1)
		{
			printf("ERROR : [mv] copy error\n");
			return 1;
		}

		/* file1�� ���� �̵��� ��ó�� ���̰� �Ѵ�. */
		if(unlink(argv[fbegin]) <0)
		{
			perror("ERROR: [mv] Unlink");
			return 1;	
		}
	}	
	return 0;	
}

