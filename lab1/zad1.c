#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>


int main(int argc, char** argv)
{
	pid_t ppid;
	int child;
	int status;	
	ppid = getpid();
	
	printf("I'm a proud parent of number %d\n",ppid);

	if((child =fork()==0))
	{
		int cpid = getpid();
		printf("I'm a newborn of number %d\n",cpid);
		exit(0);
	}
	else
	{
		wait(&status);
	}
	return 0;
}
