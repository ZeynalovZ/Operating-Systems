#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <unistd.h>

#define N 10

static int PERMS = S_IRWXU | S_IRWXG | S_IRWXO;

int *sh_buffer;
int *sh_iter_prod = NULL;
int *sh_iter_cons = NULL;


struct sembuf prod_start[2] = {{1, -1, 0}, {2, -1, 0}};
struct sembuf prod_stop[2] = {{0, 1, 0}, {2, 1, 0}};
struct sembuf cons_start[2] = {{0, -1, 0}, {2, -1, 0}};
struct sembuf cons_stop[2] = {{1, 1, 0}, {2, 1, 0}};

void producer(int semid)
{

	int k = 0;
	while (k < N)
	{
		semop(semid, prod_start, 2);
		*sh_iter_prod = k;
		printf("producer mean %d from %d pos\n", k, (*sh_iter_prod));
		sh_iter_prod++;
		semop(semid, prod_stop, 2);
		sleep(rand() % 5);
		k++;
	}
}

void consumer(int semid)
{
	int k = 0;
	while (k < N)
	{
		semop(semid, cons_start, 2);
		printf("consumer mean %d from %d pos\n", *sh_iter_cons, (*sh_iter_cons));
		sh_iter_cons++;
		semop(semid, cons_stop, 2);
		sleep(rand() % 8);
		k++;
	}
}


int main(void)
{
	int semid;
	int shmid;
	
	if ((shmid = shmget(IPC_PRIVATE, 10 * sizeof(int), PERMS)) == -1)
	{
		perror("shmget");
		exit(1);
	}


	sh_iter_prod = sh_iter_cons = shmat(shmid, 0, 0);
	sh_buffer = sh_iter_cons + sizeof(int);


	//printf("%d and %d\n", *sh_iter_prod, *sh_iter_cons);

	if (*sh_buffer == -1)
	{
		perror("shmat");
		exit(1);
	}

	if((semid = semget(IPC_PRIVATE, 3, PERMS)) == -1)
	{
		perror("semget");
		exit(1);
	}

	semctl(semid, 0, SETVAL, 0);
	semctl(semid, 1, SETVAL, N);
	semctl(semid, 2, SETVAL, 1);


	pid_t pid;
	if ((pid = fork()) == -1)
	{
		perror("can't fork");
		exit(1);
	}
	if (pid == 0)
	{
		producer(semid);
	}
	else
	{
		consumer(semid);
	}
	if (pid != 0)
	{
		wait();
		if (shmdt(sh_iter_prod) == -1)
		{
			perror("shmdt");
			exit(1);
		}
	}
	else




	return 0;
}