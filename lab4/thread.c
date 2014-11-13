#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define ADD_COUNT 20
#define IN_AMOUNT 100

#define OUT_COUNT 8
#define OUT_AMOUNT 200

#define MOVE_AMOUNT 300

#define TAKE 1
#define RELEASE 0

pthread_mutex_t acc1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t acc2 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cash =  PTHREAD_COND_INITIALIZER;
pthread_cond_t acc1Taken =  PTHREAD_COND_INITIALIZER;
pthread_cond_t acc2Taken =  PTHREAD_COND_INITIALIZER;

int account1 = 0;
int busy1 = 0;
int account2 = 0;
int busy2 = 0;

int timer = 1;
int ival = 1;

void printAcc1()
{
	printf("status of acc1 is %d\n", account1);
}

void printAcc2()
{
	printf("status of acc2 is %d\n", account2);
}

void* add(void * arg)
{
	for(int i = 0; i < ADD_COUNT; i++)
	{
		pthread_mutex_lock(&acc1);
			if(busy1) pthread_cond_wait(&acc1Taken, &acc1);
			busy1 = TAKE;
		pthread_mutex_unlock(&acc1);

		int local_acc = account1;
		local_acc = local_acc + IN_AMOUNT;
		account1 = local_acc;

		printf("Adder :status of acc1 is %d\n", account1);
		if(timer) usleep(ival);

		pthread_mutex_lock(&acc1);
			busy1 = 0;
			pthread_cond_signal(&acc1Taken);
			pthread_cond_signal(&cash);
		pthread_mutex_unlock(&acc1);
	}
	return 0;
}

void* withdraw(void * arg)
{
	for(int i = 0; i < OUT_COUNT; i++)
	{

		pthread_mutex_lock(&acc1);

			if(busy1) pthread_cond_wait(&acc1Taken, &acc1);

			while(account1 < OUT_AMOUNT)	
			{
				pthread_cond_wait(&cash, &acc1);
			}

			busy1 = TAKE;
		pthread_mutex_unlock(&acc1);


		int local_acc = account1;
		local_acc = local_acc - OUT_AMOUNT;
		account1 = local_acc;

		printf("--Withdrawer: status of acc1 is %d\n", account1);
		if(timer) usleep(ival);

		pthread_mutex_lock(&acc1);
			busy1 = RELEASE;
			pthread_cond_signal(&acc1Taken);
			pthread_cond_signal(&cash);
		pthread_mutex_unlock(&acc1);
	}
	return 0;
}

void* move(void * arg)
{

	pthread_mutex_lock(&acc1);

	if(busy1) pthread_cond_wait(&acc1Taken, &acc1);

	while(account1 < MOVE_AMOUNT)	
	{
		pthread_cond_wait(&cash, &acc1);
	}

	busy1 = TAKE;
	pthread_mutex_unlock(&acc1);

	int local_acc = account1;
	local_acc = local_acc - MOVE_AMOUNT;
	account1 = local_acc;

	printf("\tMover: status of acc1 is %d\n", account1);
	if(timer) usleep(ival);

	pthread_mutex_lock(&acc1);
		busy1 = RELEASE;
		pthread_cond_signal(&acc1Taken);
	pthread_mutex_unlock(&acc1);



	pthread_mutex_lock(&acc2);
		if(busy2) pthread_cond_wait(&acc2Taken, &acc2);
		busy2 = TAKE;
	pthread_mutex_unlock(&acc2);

	local_acc = account2;
	local_acc = local_acc + MOVE_AMOUNT;
	account2 = local_acc;

	printf("\tMover :status of acc2 is %d\n", account2);
	if(timer) usleep(ival);

	pthread_mutex_lock(&acc2);
    		busy2 = RELEASE;
		pthread_cond_signal(&acc2Taken);
	pthread_mutex_unlock(&acc1);

	return 0;
}


int main(int argc, char** argv)
{
	void* status;
	pthread_t tids[] = { 0, 1, 2 };

	pthread_create(&tids[0], NULL, add, NULL);
	pthread_create(&tids[1], NULL, withdraw, NULL);
	pthread_create(&tids[2], NULL, move, NULL);

	pthread_join(tids[0], &status);
	pthread_join(tids[1], &status);
	pthread_join(tids[2], &status);
	
	printAcc1();
	printAcc2();
	

	return 0;
}
