#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>


#define VOTES 1000000
#define COMMITEES 20
#define SHMKEY ((key_t) 123919L) 

struct PKW
{
	int PSL;
	int PIS;
	int PO;
	int SLD;
	int OTH;

	int OKW_PSL[COMMITEES];
	int OKW_PIS[COMMITEES];
	int OKW_PO[COMMITEES];
	int OKW_SLD[COMMITEES];
	int OKW_OTH[COMMITEES];

	pthread_barrier_t barrier;
	pthread_mutex_t mutex;
};

struct PKW *shared;

void init_shared()
{
	int i;
	for(i = 0; i < COMMITEES; i++)
	{
		shared->OKW_PSL[i] = 0;
		shared->OKW_PIS[i] = 0;
		shared->OKW_PO[i] = 0;
		shared->OKW_SLD[i] = 0;
	}	

	shared->PSL = 0;
	shared->PIS = 0;
	shared->PO = 0; 
	shared->SLD = 0;
	shared->OTH = 0;

	pthread_barrierattr_t attr;
	pthread_barrierattr_init(&attr);
	pthread_barrierattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
	pthread_barrier_init(&shared->barrier, &attr, COMMITEES); 

	pthread_mutexattr_t mutexat;
	pthread_mutexattr_init(&mutexat);
	pthread_mutexattr_setpshared(&mutexat, PTHREAD_PROCESS_SHARED);
	pthread_mutex_init(&shared->mutex, &mutexat);
}

int randomVote()
{
	float choice = drand48();

	if(choice < 0.2368) return 0;
	else if(choice < 0.5053) return 1; 
	else if(choice < 0.7689) return 2; 
	else if(choice < 0.8567) return 3; 
	else return 4;

}

void Voting(int committeId)
{
	int i = 0;
	int psl = 0, pis = 0, po = 0, sld = 0, oth = 0;
	srand48(getpid() * time(NULL));

	for (i = 0; i < VOTES; i++)
	{
		switch(randomVote())
		{
			case 0: psl = psl+1; break;
			case 1: pis = pis+1; break;
			case 2: po =  po+1;  break;
			case 3: sld = sld+1; break;
			case 4: oth = oth+1; break;
		}
	}

	shared->OKW_PSL[committeId] = psl;
	shared->OKW_PIS[committeId] = pis;
	shared->OKW_PO[committeId] = po;
	shared->OKW_SLD[committeId] = sld;
	shared->OKW_OTH[committeId] = oth;

	pthread_mutex_lock(&shared->mutex);
		shared->PSL = shared->PSL + psl;
		shared->PIS = shared->PIS + pis;
		shared->PO = shared->PO + po; 
		shared->SLD = shared->SLD + sld;
		shared->OTH = shared->OTH + oth;
	pthread_mutex_unlock(&shared->mutex);
	
	
}

void showResults()
{
	float percent = VOTES * COMMITEES /100;

	printf("Global results :\n");
	printf("PSL = %d (%f %%)\n", shared->PSL, shared->PSL/percent);
	printf("PO = %d (%f %%)\n", shared->PO, shared->PO/percent);
	printf("PiS = %d (%f %%)\n", shared->PIS, shared->PIS/percent);
	printf("SLD = %d (%f %%)\n", shared->SLD, shared->SLD/percent);
	printf("OTH = %d (%f %%)\n", shared->OTH, shared->OTH/percent);
}

void verify()
{
	int sum = shared->PSL + shared->PO + shared->PIS + shared->SLD + shared->OTH;
	int valid_votes = VOTES * COMMITEES;

	if(sum == valid_votes) printf("Number of votes is correct\n");
	else printf("PKW syndrome - we forgot how to math\n(should be %d, but %d exists\n)", valid_votes, sum);

	int psl =0, po =0, pis =0, sld =0, oth =0;

	int i;
	for(i = 0; i < COMMITEES; i++)
	{
		psl = psl + shared->OKW_PSL[i];
		po = po + shared->OKW_PO[i];
		pis = pis + shared->OKW_PIS[i];
		sld = sld + shared->OKW_SLD[i];
		oth = oth + shared->OKW_OTH[i];	
	}

	printf("From local data, results :\n");
	printf("PSL = %d \n", psl);
	printf("PO = %d \n", po);
	printf("PiS = %d \n", pis);
	printf("SLD = %d \n", sld);
	printf("OTH = %d\n", oth);

}

int main(int argc, char** argv)
{
	int Committe[COMMITEES];
	int i = 0, j = 0, shmid = 0, status = 0;
	
	shmid = shmget(SHMKEY, sizeof(struct PKW), IPC_CREAT | 0666);

	if (shmid < 0)
	{
		perror("shmget");
		return(1);
	}

	shared = shmat(shmid, 0, 0);

	if (shared == NULL)
	{
		perror("shmat");
		return(1);
	}

	init_shared();

	for(i = 0; i<COMMITEES; i++)
	{
		Committe[i] = fork();
		if(Committe[i]==0)
		{
			pthread_barrier_wait(&shared->barrier);
			Voting(i);
			exit(0);
		}
	}

	for(j = 0; j < COMMITEES; j++)
	{
		wait(&status);
	}
	showResults();
	verify();

	shmdt(shared);
	shmctl(shmid, IPC_RMID, NULL);

	return 0;

}
