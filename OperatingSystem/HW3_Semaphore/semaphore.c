#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#define SEMPERM 0600
#define TRUE 1
#define FALSE 0

typedef struct union_semun{
	int val;
	struct semid_ds *buf;
	ushort *array;
}semun;

int initsem(key_t semkey, int n)
{
	int status = 0, semid;
	if ((semid = semget(semkey, 1, SEMPERM | IPC_CREAT | IPC_EXCL)) == -1)
	{
		if (errno == EEXIST)
			semid = semget(semkey, 1, 0);
	}
	else
	{
		semun arg;
		arg.val = n;
		status = semctl(semid, 0, SETVAL, arg);
	}
	if (semid == -1 || status == -1)
	{
		perror("initsem failed");
		return (-1);
	}
	return (semid);
}

int p(int semid)
{
	struct sembuf p_buf;
	p_buf.sem_num = 0;
	p_buf.sem_op = -1;
	p_buf.sem_flg = SEM_UNDO;
	if (semop(semid, &p_buf, 1) == -1)
	{
		perror("p(semid) failed");
		exit(1);
	}
	return (0);
}

int v(int semid)
{
	struct sembuf v_buf;
	v_buf.sem_num = 0;
	v_buf.sem_op = 1;
	v_buf.sem_flg = SEM_UNDO;
	
	if (semop(semid, &v_buf, 1) == -1)
	{
		perror("v(semid) failed");
		exit(1);
	}
	return (0);
}

void hw1(){
	FILE *rf,*wf;
	char ch;
	int value;
	int i;
	for (i=0;i<100;i++){
		// Read last value in file
		rf = fopen("input.txt","r"); //a+	
		if(rf == NULL)
			printf("Error open file\n");
		
		while(feof(rf)==0)	
		{
			fscanf(rf,"%d",&value);
		}
	
		fclose(rf);	

		// Write value in file
		wf = fopen("input.txt","a");	
		if(wf == NULL)
			printf("Error open file\n");
	
	        fprintf(wf,"%d\n",value+1);	
		fclose(wf);
	}
	exit(0);	
}

void add1(key_t skey)
{
	int semid;
	pid_t pid = getpid();

	if ((semid = initsem(skey, 1)) < 0)
		exit(1);

	printf("\nprocess %d before critical section\n", pid);
	p(semid);
	printf("process %d in critical section\n", pid);
	/* 실제로 무언가 한다 */
	hw1();
	/* 화일에서 읽어서 1 더하기 */
	printf("process %d leaving critical section\n", pid);
	v(semid);
	printf("process %d exiting\n", pid);
	exit(0);
}

main()
{
	key_t semkey = 0x200;
	add1(semkey);			
}
