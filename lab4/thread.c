#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

/****Define const values****/
#define ADD_COUNT 20
#define IN_AMOUNT 100

#define OUT_COUNT 8
#define OUT_AMOUNT 200

#define MOVE_AMOUNT 300

#define TAKE 1
#define RELEASE 0

#define timer 1

/****Init mutex mechanisms****/
pthread_mutex_t acc1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t acc2 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cash =  PTHREAD_COND_INITIALIZER;
pthread_cond_t acc1Taken =  PTHREAD_COND_INITIALIZER;
pthread_cond_t acc2Taken =  PTHREAD_COND_INITIALIZER;

/****Global value for representing accounts and conditional variables****/
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

/****Thread that makes some income for account 1****/
void* add(void * arg)
{
	for(int i = 0; i < ADD_COUNT; i++)
	{
		/****lock access to acc1****/
		pthread_mutex_lock(&acc1);
			
			/****if busy then wait on cond_var****/
			while(busy1) pthread_cond_wait(&acc1Taken, &acc1);
			busy1 = TAKE;
		pthread_mutex_unlock(&acc1);

		/****Critical section - do your job****/
		int local_acc = account1;
		local_acc = local_acc + IN_AMOUNT;
		account1 = local_acc;

		printf("Adder :status of acc1 is %d\n", account1);
		
		if(timer) 
		{
			int ival = rand()%20000;
			usleep(ival);
		}

		/****Inform others that finished****/
		pthread_mutex_lock(&acc1);
			busy1 = 0;
			pthread_cond_signal(&acc1Taken);
			pthread_cond_signal(&cash);
		pthread_mutex_unlock(&acc1);
	}
	return 0;
}

/****Thread to withdraw some money****/
void* withdraw(void * arg)
{
	for(int i = 0; i < OUT_COUNT; i++)
	{

		/****Try to lock access to acc1****/
		pthread_mutex_lock(&acc1);
			
			/****wait if busy****/
			while(busy1) pthread_cond_wait(&acc1Taken, &acc1);

			/****wait if not enough money****/
			while(account1 < OUT_AMOUNT)	
			{
				pthread_cond_wait(&cash, &acc1);
			}

			busy1 = TAKE;
		pthread_mutex_unlock(&acc1);

		/****Critical section - do your job****/
		int local_acc = account1;
		local_acc = local_acc - OUT_AMOUNT;
		account1 = local_acc;

		printf("--Withdrawer: status of acc1 is %d\n", account1);
		if(timer) 
		{
			int ival = rand()%20000;
			usleep(ival);
		}

		/****Informt others that finished ****/
		pthread_mutex_lock(&acc1);
			busy1 = RELEASE;
			pthread_cond_signal(&acc1Taken);
			pthread_cond_signal(&cash);
		pthread_mutex_unlock(&acc1);
	}
	return 0;
}

/****Thread that moves some amount of money from one account to another ****/
void* move(void * arg)
{

	/****All locking part for first account - try to access, check if enough money to withdraw etc.****/
	pthread_mutex_lock(&acc1);

		while(busy1) pthread_cond_wait(&acc1Taken, &acc1);

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
	if(timer) 
	{
		int ival = rand()%20000;
		usleep(ival);
	}
		

	pthread_mutex_lock(&acc1);
		busy1 = RELEASE;
		pthread_cond_signal(&acc1Taken);
	pthread_mutex_unlock(&acc1);


	/****All locking part for second account - try to access, add some money****/
	pthread_mutex_lock(&acc2);
		while(busy2) pthread_cond_wait(&acc2Taken, &acc2);
		busy2 = TAKE;
	pthread_mutex_unlock(&acc2);

	local_acc = account2;
	local_acc = local_acc + MOVE_AMOUNT;
	account2 = local_acc;

	printf("\tMover :status of acc2 is %d\n", account2);
	if(timer) 
	{
		int ival = rand()%20000;
		usleep(ival);
	}

	pthread_mutex_lock(&acc2);
    		busy2 = RELEASE;
		pthread_cond_signal(&acc2Taken);
	pthread_mutex_unlock(&acc1);

	return 0;
}


int main(int argc, char** argv)
{
	srand(time(NULL));	

	void* status;
	pthread_t tids[] = { 0, 1, 2 };

	/****Create threads****/
	pthread_create(&tids[0], NULL, move, NULL);
	pthread_create(&tids[1], NULL, add, NULL);
	pthread_create(&tids[2], NULL, withdraw, NULL);

	/****Wait for threads to end****/
	pthread_join(tids[0], &status);
	pthread_join(tids[1], &status);
	pthread_join(tids[2], &status);
	
	/****At the end - print the status to check if everything is right****/
	printAcc1();
	printAcc2();
	

	return 0;
}
