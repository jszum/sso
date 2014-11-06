#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t acc1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t acc2 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cash =  PTHREAD_COND_INITIALIZER;
pthread_cond_t acc1Taken =  PTHREAD_COND_INITIALIZER;
pthread_cond_t acc2Taken =  PTHREAD_COND_INITIALIZER;
int account1 = 0;
int busy1 = 0;
int account2 = 0;
int busy2 = 0;

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
	for(int i = 0; i < 20; i++)
	{
		pthread_mutex_lock(&acc1);
			if(busy1) pthread_cond_wait(&acc1Taken, &acc1);
			busy1 = 1;
		pthread_mutex_unlock(&acc1);

		account1 = account1 + 100;
		printf("Adder :status of acc1 is %d\n", account1);

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
	for(int i = 0; i < 8; i++)
	{

		pthread_mutex_lock(&acc1);

			if(busy1) pthread_cond_wait(&acc1Taken, &acc1);

			while(account1 < 200)	
			{
				pthread_cond_wait(&cash, &acc1);
			}

			busy1 = 1;
		pthread_mutex_unlock(&acc1);


		account1 = account1 - 200;
		printf("Withdrawer: status of acc1 is %d\n", account1);

		pthread_mutex_lock(&acc1);
			busy1 = 0;
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

	while(account1 < 300)	
	{
		pthread_cond_wait(&cash, &acc1);
	}

	busy1 = 1;
	pthread_mutex_unlock(&acc1);


	account1 = account1 - 300;
	printf("\tMover: status of acc1 is %d\n", account1);

	pthread_mutex_lock(&acc1);
		busy1 = 0;
		pthread_cond_signal(&acc1Taken);
		pthread_cond_signal(&cash);
	pthread_mutex_unlock(&acc1);



	pthread_mutex_lock(&acc2);
		if(busy1) pthread_cond_wait(&acc2Taken, &acc2);
		busy2 = 1;
	pthread_mutex_unlock(&acc2);

	account2 = account2 + 300;
	printf("\tMover :status of acc2 is %d\n", account2);

	pthread_mutex_lock(&acc2);
    		busy2 = 0;
		pthread_cond_signal(&acc2Taken);
		pthread_cond_signal(&cash);
	pthread_mutex_unlock(&acc1);

	return 0;
}


int main(int argc, char** argv)
{
	void* status;
	pthread_t tids[] = { 0, 1, 2 };

	pthread_create(&tids[0], NULL, add,NULL);
	pthread_create(&tids[1], NULL, withdraw,NULL);
	pthread_create(&tids[2], NULL, move,NULL);

	pthread_join(tids[0], &status);
	pthread_join(tids[1], &status);
	pthread_join(tids[2], &status);
	
	printAcc1();
	printAcc2();
	

	return 0;
}
