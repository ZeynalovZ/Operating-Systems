#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

// читатели и писатели
#define READER_COUNT 5
#define WRITER_COUNT 3
#define ITER 3


// операции на семафорах
#define SEM_INC 1
#define SEM_DEC -1
#define SEM_WAIT 0

#define N 10
// Операции на семафорах
#define SEM_OP_INC 1
#define SEM_OP_DEC -1
#define SEM_OP_WAIT 0


// Ключи семафор
#define RR	3	//ждущие читатели     WW
#define SB	2	//бинарный            AW
#define WR	1	//ждущие писатели     WR
#define RI	0	//активные писатели   AR


struct sembuf read_start[]  =  {{SB, SEM_OP_WAIT, 0}, 
                                {RI, SEM_OP_INC, 0},
                                {WR, SEM_OP_WAIT, 0}};
                               
struct sembuf read_stop[]   =  {{RI, SEM_OP_DEC, 0}};
                               
struct sembuf write_start[] =  {{RR, SEM_OP_INC,0},
                                {RI, SEM_OP_WAIT, 0}, 
                                {SB, SEM_OP_WAIT, 0},
								{SB, SEM_OP_INC, 0}, 
                                {RR, SEM_OP_DEC, 0}};
                               
struct sembuf write_stop[]  =  {{SB, SEM_OP_DEC, 0}};

static int PERMS = S_IRWXU | S_IRWXG | S_IRWXO;

int *sh_buffer;
int sem_id = -1;
int status;
void startRead()
{ 
	semop(sem_id, read_start, 3); 
}

void stopRead()
{ 
	semop(sem_id, read_stop, 1);
}

void startWrite()
{
	semop(sem_id, write_start, 5);
}

void stopWrite()
{ 
	semop(sem_id, write_stop, 1); 
}

void reader(int reader_id)
{
    while(1)
    {
        
		startRead();
		printf ("Reader%d read %d\n", reader_id, *sh_buffer);
		stopRead(); 
        sleep(rand() % 4);
    }
        
}

void writer(int writer_id)
{   
    while(1)
    {
        
		startWrite();
		printf("Writer%d write %d\n", writer_id, ++(*sh_buffer));
		stopWrite();
        sleep(rand() % 4);
    }
        

}

int main(void)
{
    srand(time(NULL));
    pid_t childs[READER_COUNT + WRITER_COUNT];
    int shmid;
    shmid = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | PERMS);
    if (shmid == -1)
    {
        perror("shmget");
        exit(1);
    }

    sh_buffer = shmat(shmid, 0, 0);
    *sh_buffer = 0;
    //printf("memory allocated successfully");

    sem_id = semget(IPC_PRIVATE, 4, IPC_CREAT | PERMS);
    if (sem_id == -1)
    {
        perror("sem_id error");
        exit(1);
    }
    //printf("sems successfully created");
    semctl(sem_id, RR, SETVAL, 0);
    semctl(sem_id, SB, SETVAL, 0);
    semctl(sem_id, RI, SETVAL, 0);
    semctl(sem_id, WR, SETVAL, 0);

    for (int i = 0; i < READER_COUNT + WRITER_COUNT; i++)
	{
		childs[i] = fork();
        
		if (childs[i] == -1)
		{
			perror("Can't fork\n"); 
			exit(1);
		}
		else if (childs[i] == 0)
		{
            if (i < WRITER_COUNT)
            {
                // while(1)
                writer(i);
                return 0;
            }
            else if (i >= WRITER_COUNT)
            {
                // while(1)
                reader(i - WRITER_COUNT);              
                return 0;
            }
		}
	}

    /* Ожидать окончание всех дочерних процессов */
	for (int i = 0; i < READER_COUNT + WRITER_COUNT; i++)
	{
		wait(&status);
	}
    
	/* Отсоединить разделяемый сегмент */
	if (shmdt(sh_buffer) == -1)
	{
		perror("Error: shmdt!\n");
	}
    return 0;
}
