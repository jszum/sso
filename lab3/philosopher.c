#include <sys/sem.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <fcntl.h>

#define SHMKEY ((key_t) 123456L) 
#define SEMKEY ((key_t) 234567L)
#define PERM 0666
#define DEBUG 1
#define COUNT 5

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
		fprintf(stderr, "samaphire cannot be created");
	} 

	return semid;
}

void semSet()
{
	semunion.val = COUNT;
	if(semctl(semid, 0, SETVAL , semunion) < 0 )
	{
		fprintf(stderr, "cannot set sem");
	} 
}


int semTake(int num) {
    struct sembuf sops;
    sops.sem_num = 0;
    sops.sem_op = -2;
    sops.sem_flg = 0;
    if(semop(semid, &sops, 1) < 0)
    {
		fprintf(stderr, "cannot take sem - I wont eat Ph.[%d]\n", num);
		return -1;
    } 
    return 0;
}

void semGet()
{
	int value = semctl(semid, 0, GETVAL);
	printf("sem value = %d\n", value);
}

int semRelease() {
    struct sembuf sops;
    sops.sem_num = 0;
    sops.sem_op = 2;
    sops.sem_flg = 0;
    if(semop(semid, &sops, 1) < 0)
    {
	fprintf(stderr, "cannot release sem\n");
	return -1;
    } 
    return 0;

}

void semFree() {

    semctl(semid, 1, IPC_RMID );
}

void printStatus(char *stat, int num, int myeat)
{
	printf("[%c][%c][%c][%c][%c] - Ph.[%d] ate %d times\n",stat[0],stat[1],stat[2],stat[3],stat[4], num, myeat);
	if(DEBUG)
	{
		int fd = open("debug.txt", O_APPEND | O_RDWR);
		char buf[16];
		sprintf(buf,"[%c][%c][%c][%c][%c]\n",stat[0],stat[1],stat[2],stat[3],stat[4]);
		write(fd, buf, 16);
		close(fd);
	}
}

int main(int argc, char **argv)
{
	size_t size;
	int myeat = 0;
	size = sizeof(char)*COUNT;

	int mynumber = atoi(argv[1]);
	char *status;
	if((shmid = shmget(SHMKEY, size, PERM | IPC_CREAT))<0)
	{
		fprintf(stderr, "Cannot create shared memory\n");
	}
	if((status = shmat(shmid, 0, 0)) < 0)
	{
		fprintf(stderr, "Cannot attach shared memory\n");
	}

	init_semaphore();

	while(1)
	{
		status[mynumber] = 'm';
		printStatus(status, mynumber, myeat);
		sleep(2);
		status[mynumber] = 'w';
		printStatus(status, mynumber, myeat);
		sleep(1);
		while(semTake(mynumber) == -1);
		status[mynumber] = 'j';
		myeat = myeat+1;
		printStatus(status, mynumber, myeat);
		sleep(4);
		semRelease();
	}

	shmdt(status);

	return 0;
	
}
