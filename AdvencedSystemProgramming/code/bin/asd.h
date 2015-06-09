#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <utmpx.h>
#include <fcntl.h>
#include <ftw.h>
#include <errno.h>

#define TRUE 1
#define FALSE 0

#define NO "n\n"
#define YES "y\n"

#define A 'a'
#define B 'b'
#define N 'n'
#define smallR 'r'
#define bigR 'R'
#define F 'f'
#define H 'h'
#define I 'i'
#define S 's'
#define L 'l'

#define REDIR_IN_ALERT "[REDIR_IN]"

#define MAX_SIZE 100
#define CHAR_PERM_SIZE 11
#define TIME_SIZE 20

/* Time ouput standard */
char *outpT[]={"%b %e %H:%M"
		,"%F %H:%M"};

/* Copy file to another file */
int copyfile(char *file1, char *file2)
{
	int fpRead,fpWrite;	 
	char charBuf[BUFSIZ];
	int nread;

	// reading file fd
	if( (fpRead = open(file1,O_RDONLY)) < 0)
	{
		perror("[ERROR] copy read file open");
		exit(-3);
	}		

	// writing file fd
	if( (fpWrite = open(file2,O_WRONLY | O_CREAT | O_TRUNC, 0644)) <0 ){
		perror("[ERROR] copy write file open");
		exit(-3);
	}
		
	// Copy reading file to writing file
	while( (nread=read(fpRead,charBuf,BUFSIZ))>0)
		if(write(fpWrite,charBuf, nread)< nread)
		{
			close(fpRead);
			close(fpWrite);
			exit(1);
		}
	close(fpRead);
	close(fpWrite);

	return 0;
}


/* compare string */
int compareStr(const void *str1, const void *str2)
{	
	return strcmp(str1,str2);
}


/* Sorting string */
int SortingStr(char **str,int sumfiles)
{
	int i,j;
	char tmp[MAX_SIZE];

	for(i=sumfiles-2; i>=0; i--)
	{
		for(j=0; j<=i; j++)
		{
			if(strcmp(str[j],str[j+1]) > 0)
				{
					strcpy(tmp,str[j]);
					strcpy(str[j],str[j+1]);
					strcpy(str[j+1],tmp); 
				}
		}
	}
	return 0;
}


/* 숫자 퍼미션 모드를 인자로 받으면 인자로 받은 스트링변수에 반환하는 함수다 */
int StrMode(char pMode[], mode_t stmode)
{
	int mode;
	mode = stmode & S_IFMT; // Kind of Mode
	switch (mode) 
	{
		// Directory
		case S_IFDIR:
			pMode[0]='d';
			break;

		// Block Device
		case S_IFBLK:
			pMode[0]='b';
			break;

		// Char Device
		case S_IFCHR:
			pMode[0]='c';
			break;

		// Symbol
		case S_IFLNK:
			pMode[0]='l';
			break;
	}
	if(stmode & S_ISUID)
		pMode[3]='s';	
	if(stmode & S_ISGID)
		pMode[6]='s';	
	if(stmode & S_ISVTX)
		pMode[9]='s';	
		
	if(stmode & S_IRUSR)
		pMode[1]='r';
	if(stmode & S_IWUSR)
		pMode[2]='w';
	if(stmode & S_IXUSR)
		pMode[3]='x';

	if(stmode & S_IRGRP)
		pMode[4]='r';
	if(stmode & S_IWGRP)
		pMode[5]='w';
	if(stmode & S_IXGRP)
		pMode[6]='x';
	
	if(stmode & S_IROTH)
		pMode[7]='r';
	if(stmode & S_IWOTH)
		pMode[8]='w';
	if(stmode & S_IXOTH)
		pMode[9]='x';

	return 0;
}


