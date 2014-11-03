#include <sys/types.h>
#include <sys/unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

#define SHMKEY ((key_t) 123456L) 
#define SEMKEY ((key_t) 234567L)
#define PERM 0666
#define count 5

int semid;
int shmid;

union semun
{
	int val;
	struct semid_ds *buf;
	unsigned short int *array;
	struct seminfo *__buf;
} semunion;

int init_semaphore()
{
	if( (semid=semget(SEMKEY, 1, PERM | IPC_CREAT)) < 0 )
	{
		fprintf(stderr, "semaphore cannot be created");
	} 

	return semid;
}

void semSet()
{
	semunion.val = count;
	semctl(semid, 0, SETVAL , semunion);
}

void init_shared()
{
	size_t size;
	size = sizeof(char)*count;
	char *status;
	if((shmid = shmget(SHMKEY, size, PERM | IPC_CREAT))<0)
	{
		fprintf(stderr, "Cannot create shared memory\n");
		exit(1);
	}
	if((status = shmat(shmid, 0, 0)) < 0)
	{
		fprintf(stderr, "Cannot attach shared memory\n");
		exit(1);
	}

	for(int i = 0; i < count; i++)
	{
		status[i] = 0;
	}
	shmdt(status);

}

int main(int argc, char **argv)
{
	int fd;
	init_semaphore();
	semSet();
	init_shared();
	if( (fd = open("debug.txt", O_CREAT | O_TRUNC, 0666)) < 0)
	{
		fprintf(stderr, "Cannot create a file\n");
		exit(1);
	}
	close(fd);

	return 0;
	
}
