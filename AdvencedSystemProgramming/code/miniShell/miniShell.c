/*------------------------------------------------------
고급시스템 프로그래밍 : Mini Shell

국민대학교 컴퓨터공학과
20093284 나홍철
-------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define TRUE 1
#define FALSE 0

#define AMPERSAND "&"	/* Background */
#define PIPE "|"		/* Pipe */
#define REDIR_IN "<"	/* Redirection Input */
#define	REDIR_OUT ">"	/* Redirection Output */
#define	REDIR_EOUT "2>"	/* Redirection Error Output */

#define PATH "/root/syspro/bin/"	/* Command prgoram path */
#define END_CMD "goodbye"	/* Exit shell command */
#define PROMPT "my-prompt>" /* Prompt string */
#define SPACE_BAR " "
#define NEW_LINE "\n"
#define CD "/root/syspro/bin/cd" /* Cd Command */

#define MAX_ARG 8	/* Maximum number of arguments */
#define MAX_PROCESS 8	/* Maximum number of process */
#define MAX_LENGTH 32	/* Maximum string length of one argument */
#define MAX_LINE 256	/* Maximum length of letters at single line */
#define MAX_FDTABLE 3	 /* Maximum size of descriptor table 
0=Standard In, 1=Standard Out, 2=Standard Error Out */
#define MAX_PFDTABLE 2 /* Maximum size of Pipe descriptor table */





/**
* [ Change Directory ]

* 
**/
int CDcommand(int argc, char **argv)
{
	char *chpath=getenv("HOME");

	if(argc > 1)
		realpath(chpath,argv[1]);
	chdir(chpath);
	
	return 0;
}





/**
* [ Process ]
* 이 구조체는 하나의 명령어를 실행하는데 필요한 프로세스의 정보를
* 담고 있는 구조체다. 하나의 명령어가 실행 될 때 사용되는 정보로서
* 현재 MiniShell에서는 총 8개의 명령어 프로세스을 사용 할 수 있다.
**/
struct process
{
	int fd[MAX_FDTABLE];/* 한 명령어 프로세스 Standard In, Standard Out,
						Standard error out 디스크립터 번호 테이블 배열이다.
						이 테이블 값의 변동이 있을 때 프로세스의
						파일 디스크립터도 변경시켜준다.*/
	char **argv;	/* 한 명령어 프로세스의 입력받은 명령어 배열 */
	int argc;	/* 한 명령어 프로세스의 입력받은 인수의 총 개수다. */
	int pid;		/* 한 명령어 프로세스의 pid */
	int pgid;		/* 한 명령어 프로세스의 pgid => 백그라운드 실행 시 백그라운되는
					처음 프로세스을 그룹리더로 다른 프로세스들을 같은 그룹으로 묶어준다 */
};

/**
* [ Memory Release ]
* 2차배열로 할당된 메모리를 회수하는 함수다.
**/
int MemRelease(struct process *p)
{
	int i;

	/* 각 1차 배열 메모리 회수 */
	for (i = 0; i < p->argc; i++)
	{
		free(p->argv[i]);
		if (p->argv[i] != NULL)
			return 1;
	}
	/* 2차 배열 메모리 회수 */
	free(p->argv);
	return 0;
}









/**
* [ Pipe file descriptor Create ]
* MiniShell에서 사용 될 pipe를 미리 입력가능한 명령어 개수 만큼 생성한다.
* 프로세서에 생성된 파이프의 파일 디스크립터 번호를 사용하기 위해
* 이미 생성한 파이프 전용 테이블배열에 저장하는 함수다.
**/
int PfdCreate(int pfd[][MAX_PFDTABLE])
{
	int i;
	int fd[MAX_PFDTABLE];	/* 생성된 pipe의 파일 디스크립터
							번호가 임시로 저장되는 정수배열 변수다. */

	/* 생성된 pipe의 파일 디스크립터 번호를 테이블배열 에 저장한다. */
	for (i = 0; i<MAX_ARG; i++)
	{
		if (pipe(fd) < 0)
			perror("ERROR pipe :");
		pfd[i][0] = fd[0];
		pfd[i][1] = fd[1];
	}
	return 0;
}

/**
* [ Pipe file descriptor Close ]
* 모든 pipe들을 프로세서의
* 파일 디스크립터 테이블에서 제거한다.
**/
int PfdClose(int pfd[][MAX_PFDTABLE])
{
	int i;

	/* 생성된 pipe들을 모두 닫아 준다. */
	for (i = 0; i<MAX_ARG; i++)
	{
		close(pfd[i][0]);
		close(pfd[i][1]);
	}
	return 0;
}







/**
* [ NewCommand ]
* 새로운 명령어를 입력받을 때 사용하는 함수다. 새로운 구조체를 초기화하는
* 함수역할을 한다. 입력받은 스트링을 " "(space bar)기준으로 잘라낸 토큰값이
* 새로운 명령어일 경우 새로 할당해주며 명령어의 위치를 덧 붙여준다.
**/
int NewCommand(struct process *p, char *str)
{
	char *ptr;
	int i;

	p->argc = 0;	/* 입력받은 인자의 개수를 초기화 한다. */
	p->pid = 0;	/* PID번호를 초기화 한다. */
	p->pgid = 0;	/* PGID번호를 초기화 한다. */

	/* 명령어와 인자를 저장할 배열을 초기화 한다.*/
	if ((p->argv = (char**)malloc(sizeof(char*)* MAX_ARG)) == NULL)
	{
		perror("mem faild\n");
		exit(1);
	}

	/* 프로세스 구조체의 파일 디스크립터를 초기화한다. */
	for (i = 0; i < MAX_FDTABLE; i++)
		p->fd[i] = i;


	ptr = strtok(str, SPACE_BAR); /* 스트링을 space bar 기준으로 잘라낸다. */

	/* 명령어와 인자를 저장할 배열을 초기화 한다.*/
	if ((p->argv[p->argc] = (char *)malloc(MAX_LENGTH * sizeof(char))) == NULL)
	{
		perror("mem faild\n");
		exit(1);
	}

	sprintf(p->argv[p->argc], "%s%s", PATH, ptr); /* 명령어를 명령어의 위치에 덧 붙여준다. */

	return 0;
}







/**
* [ Running Command ]
* 명령어 실행을 위해 fork()를 한 후 해당되는 명령어 프로세스을
* 실행 시켜주는 함수다. 혹시 pipe는 redirection이 있다면
* Standard In이나 Standard Out을 해당 파일 디스크립터 번호로
* 변경(dup2)시켜준다.
**/
int RunCommand(struct process *p, int pfd[][MAX_PFDTABLE])
{
	pid_t pid;
	int i;

	/* fork() 함수를 통해 프로세서를 복제한다.
	반환된 pid값이 0이라면 복제된 child 프로세스로서
	실행하고자 하는 명령을 실행한다. */
	switch (pid = fork()) {
	case -1:
		perror("miniSh");
		return (-1);
	case 0:
		/* 자식프로세스 */
		setpgid(0, p->pgid);	/* 명령어 프로세스의 PGID를 미리 지정해놓은
								Group ID로 변경해 준다 */

		/* 변경된 Standard In, Standard Out, Standard error out이 있다면
		적용시켜준다. */
		for (i = 0; i<MAX_FDTABLE; i++)
		{
			if (p->fd[i] != i)
				dup2(p->fd[i], i);
		}
		PfdClose(pfd); /* 프로세스의 파일 디스크립터 테이블에 등록된
					   pipe들 간 충돌이 일어나지 않도록 닫아주어 정리한다.*/
		execv(p->argv[0], p->argv); /* 명령을 실행한다. */
		perror("exec failed");
		return 1;
	default:
		/* 부모프로세스 */
		return pid;
	}
}






/**
* [ Command Handle ]
* MiniShell에서 가장 중요한 함수이다. 입력받은 스트링을
* Spacebar기준으로 잘라낸다. 잘라내어진 토큰은 6가지 경우로 나누어진다.
*
*   AMPESAND : "&"인 경우 프로세스을 백그라운드 실행을 하기 위해 Background flag를 TRUE로
* 변경시켜준다. 나중에 프로세스을 실행 한 후 부모 프로세스는
* 자식프로세스를 wait하지 않고 좀비 프로세스로 남긴다.
*
*   PIPE : "|"인 경우 파이프 카운터를 1증가 시켜주어 명령어가 하나 반드시 온다는 것을
* 알려주고 미리 초기화시켜 두었던 파이프의 파일디스크립터 번호테이블 배열값을
* 각 프로세스의 파일디스크립터 테이블 배열 값에 입력한다.
* 나중에 프로세스 실행 시 기존 Standard In, Standard Out 디스크립터 번호가 아니라면
* 프로세스의 파일디스크립터 테이블에 할당(dup함수) 될 것이다.
* ex) $ cat file1 | grep a
* fd[0][0] => cat명령어의 standard in으로 원래 번호인 '0'
* fd[0][1] => cat명령어의 standard out으로 원래 번호가 아닌
* 파이프의 write 디스크립터 번호를 저장한다.
* fd[1][0] => cat명령어의 standard in으로 원래 번호가 아닌
* 파이프의 read 디스크립터 번호를 저장한다.
* fd[1][1] => cat명령어의 standard out으로 원래 번호인 '1’
*
*   REDIR_IN : "<"인 경우 프로세스의 Standard In은 "<" 이후에 오는 파일을
* read open한 디스크립터번호가 된다.
*
*   REDIR_OUT : ">"인 경우 프로세스의 Standard In은 ">" 이후에 오는 파일을
* write open한 디스크립터번호가 된다.
*
*   REDIR_EOUT : "2>"인 경우 프로세스의 Standard error out은 "<" 이후에 오는 파일을
* write open한 디스크립터번호가 된다.
*
* 그 외 인자인 경우 3차원 배열에 저장하여 추후 명령어 실행 시 인자값 리스트로 넘겨준다.
**/
int CommandHandle(char *str)
{
	struct process p[MAX_PROCESS];	/* 명령어 프로세스을 실행할 구조체로서
									총 8개의 프로세스 실행이 가능하다. */
	char *ptr;	/* 나누어진 스트링 토큰주소의 처음을 가리키는 포인터다. */
	int pfd[MAX_PROCESS - 1][MAX_PFDTABLE];	/* 생성된 파이프 디스크립터
											번호 테이블 배열이다.
											파이프의 총 개수는
											'실행가능한 프로세스의 총개수-1' 이다 */

	int pCnt;	/*프로세스 카운트*/
	/* 스트링에 입력된 파이프의 개수로서 명령어의 개수를 판단 할 수 있는	정보로 사용된다. */
	int bFlag;	/* 스트링에 백그라운드 실행이 존재하는가를 알려주는 백그라운드 flag다. */
	int status;	/* 부모프로세스가 자식프로세스를 wait하여 반환된 상태를 저장 할 변수다. */
	int i;

	PfdCreate(pfd); /* 사용할 pipe를 가능한 명령어만큼 초기에 생성 해준다.*/

	pCnt = 0;	/* 실행되는 명령어 프로세스의 개수다. */
	bFlag = FALSE;

	/* 새로운 명령어를 입력받아 명령어 2차배열의 처음주소에 할당한다. */
	NewCommand(&p[pCnt], str);

	/* 입력받은 스트링을 Spacebar(" ") 단위로 잘라 토큰(단어)을 생성한다. */
	while (ptr = strtok(NULL, SPACE_BAR))
	{
		/* 나누어진 토큰이 AMPESAND(&) */
		if (strcmp(ptr, AMPERSAND) == 0)
		{
			bFlag = TRUE;
			break;
		}

		/* 나누어진 토큰이 PIPE(|)
		PIPE를 기준은 왼쪽 명령어의 Standard Out이
		Pipe의 Write 디스크립터의 번호를 할당받고
		PIPE를 기준으로 오른쪽 명령어의 Standard In이
		Pipe의 read 디스크립터의 번호를 할당받는다.	*/
		else if (strcmp(ptr, PIPE) == 0)
		{
			pCnt++;
			/* 새로운 명령어를 입력받아 명령어 2차배열의 처음주소에 할당한다. */
			NewCommand(&p[pCnt], NULL);

			p[pCnt - 1].fd[1] = pfd[pCnt - 1][1];
			p[pCnt].fd[0] = pfd[pCnt - 1][0];
		}

		/* 나누어진 토큰이 REDIR_IN(<) 일 때, 명령어의 Standard In을
		입력받은 파일을 Open한 디스크립터 번호를 할당한다. */
		else if (strcmp(ptr, REDIR_IN) == 0)
		{
			ptr = strtok(NULL, SPACE_BAR); 
			p[pCnt].fd[0] = open(ptr, O_RDONLY);
		}

		/* 나누어진 토큰이 REDIR_OUT(>) 일 때, 명령어의 Standard Out을
		입력받은 파일을 Open한 디스크립터 번호를 할당한다. */
		else if (strcmp(ptr, REDIR_OUT) == 0)
		{
			ptr = strtok(NULL, SPACE_BAR);
			p[pCnt].fd[1] = open(ptr, O_WRONLY | O_CREAT | O_TRUNC, 0755);
		}

		/* 나누어진 토큰이 REDIR_EOUT(2>) 일 때, 명령어의 Standard Out을
		입력받은 파일을 Open한 디스크립터 번호를 할당한다. */
		else if (strcmp(ptr, REDIR_EOUT) == 0)
		{
			ptr = strtok(NULL, SPACE_BAR);
			p[pCnt].fd[2] = open(ptr, O_WRONLY | O_CREAT | O_TRUNC, 0755);
		}

		/* 나누어진 토큰이 그외 일때, 인자 개수를 하나 추가시키고
		새로이 2차배열에 메모리를 할당 시켜주어 입력받는다. */
		else
		{
			p[pCnt].argc++;

			if ((p[pCnt].argv[p[pCnt].argc] = (char *)malloc(MAX_LENGTH * sizeof(char))) == NULL)
			{
				perror("mem faild\n");
				exit(1);
			}
			strcpy(p[pCnt].argv[p[pCnt].argc], ptr); /* 나누어진 토큰을 새로 할당 받은
													 인자배열 메모리에 복사 */
		}
	}


	/* 처음 실행되는 명령어 프로세스 이후 실행되는 프로세스들이다. */
	for (i = 0; i <= pCnt; i++)
	{
		/* Run Change Directory */
		if(strcmp(p[i].argv[0],CD) == 0)
			CDcommand(p[i].argc,p[i].argv);
		
		else
		{
			/* 백그라운드 실행이 아니라면 커널의 신호를 받을 수 있도록 커널 그룹을 갖는다.
			백그라운드 실행이 아닐때는 초기값인 '0'이 PGID로 셋팅되어 자기자신 PID로 하는
			그룹이되는 동시에 그룹리더가 되어 다음 프로세스들의 그룹리더가 된다. */
			if (!bFlag && (i==0))
				p[i].pgid = getpid();
			else
				p[i].pgid = p[0].pgid;

			p[i].pid = RunCommand(&p[i], pfd);	/* 명령어 프로세스를 실행하고
									PID를 반환받는다. */
		

			/* 백그라운드 실행이라면 그룹리더인 처음 프로세스의 PID를
			다음 프로세스의 그룹을 PGID로 한다. */
			if (bFlag && (i==0))
				p[i].pgid = p[0].pid;
		}
		
	}

	PfdClose(pfd); /* 생성된 모든 Pipe들을 파일디스크립터 테이블에서 제거 한다. */

	/* 자식프로세스를 백그라운드 실행이 아니라면 waitpid 함수를 통해서
	종료시 수신한다. 백그라운드 실행이라면 수신하지 않고 좀비프로세스로 만든다. */
	for (i = 0; i <= pCnt; i++)
	{		
		if (bFlag == TRUE)
			printf("[Process id %d]\n", p[i].pid);
		
		/* Not background & Not cd Command */
		else if( (bFlag == FALSE) && (strcmp(p[i].argv[0],CD) != 0))
			if (waitpid(p[i].pid, NULL, 0) == -1)
				perror("Error:waitpid ");

		MemRelease(&p[i]);	/* 프로세스 구조체에서 사용한 명령어
							배열변수의 메모리를 반환한다 */

	}
	return 0;
}


























/**
* [ Shell Program ]
* 스트링을 입력받아 그 스트링사이의 Spacebar단위로 나누어 토큰으로 만든다.
* 만들어진 각 토큰들은 판독 프로세스을 통해서 판독을 하게 되고,
* 각 역할에 맞추어진 함수들을 실행한다.
**/
int main(void)
{
	char *str; /* 입력받는 스트링 저장 변수*/
	char *ptr; /* 나누어진 토큰의 포인터 */

	while (1)
	{
		/* Initialize */
		str = (char *)malloc(MAX_LINE * sizeof(char));

		printf("%s ", PROMPT);	/* 프롬프트를 출력한다. */
		fgets(str, MAX_LINE, stdin); /* 스트랑을 입력 받는다. */

		ptr = strtok(str, NEW_LINE); /* 입력받은 스트링의 '\n'을 제외한다 */

		/* "goodbye"를 입력받으면 쉘을 종료하며, 단순히 '\n'만 입력 받은 경우
		위에서 제거되었기 때문에 NULL값을 갖는다. 그러한 경우를 제외하고
		스트링을 분석 실행하는 함수를 수행한다. */
		if (strcmp(str, END_CMD) == 0)
			return 0;
		else if (ptr != NULL)
		{
			CommandHandle(str);	/* 스트링 분석, 수행 함수*/
		}
		free(str);
	}

	return 0;
}









