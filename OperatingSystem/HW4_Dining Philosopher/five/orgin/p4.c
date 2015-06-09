#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SEMPERM 0600
#define TRUE 1
#define FALSE 0

typedef struct union_semun {
	int val;
	struct semid_ds *buf;
	ushort *array;
} semun;

int initsem(key_t semkey, int n){
	int status = 0, semid;
	if ((semid = semget(semkey, 1, SEMPERM | IPC_CREAT | IPC_EXCL)) == -1){
		if ((errno == EEXIST))
			semid = semget(semkey, 1, 0);
	}
	else{
		semun arg;
		arg.val = n;
		status = semctl(semid, 0, SETVAL, arg);
	}


	if (semid == -1 || status == -1){
		perror("initsem failed");
		return (-1);
	}
	return (semid);
}

int p(int semid){
	struct sembuf p_buf;
	p_buf.sem_num = 0;
	p_buf.sem_op = -1;
	p_buf.sem_flg = SEM_UNDO;

	if ((semop(semid, &p_buf, 1)) == -1){
		perror("p(semid) failed");
		exit(1);
	}
	return (0);
}

int v(int semid){
	struct sembuf v_buf;
	v_buf.sem_num = 0;
	v_buf.sem_op = 1;
	v_buf.sem_flg = SEM_UNDO;
	if ((semop(semid, &v_buf, 1)) == -1){
		perror("v(semid) failed");
		exit(1);
	}
	return (0);
}

void pick(key_t skey1, key_t skey2)
{
	int semid1, semid2;
	pid_t pid = getpid();
	if ((semid1 = initsem(skey1, 1)) < 0)
		exit(1);

	if ((semid2 = initsem(skey2, 1)) < 0)
		exit(1);


	printf("\n%d wait to Pick up chops4 \n", pid);
	sleep(5);       // think first chop2
	p(semid1);
	printf("%d pick up chops4\n", pid);
	printf("%d wait to Pick up chops0 \n", pid);
	sleep(5);       // think first chop2
	p(semid2);
	printf("%d pick up chops0\n", pid);
	sleep(5);       // think first chop2
	v(semid1);
	v(semid2);
	printf("%d drop chops4, chops0\n", pid);
	exit(0);
}

main()
{
	key_t semkey0 = 0x204;  // chopstick0
	key_t semkey1 = 0x200;  // chopstick1
	while (1){
		sleep(2);
		pick(semkey0, semkey1);
	}
}
