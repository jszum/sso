#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/wait.h>

#define size 1024
int main(int argc, char**argv)
{
        int fd[2];
        int child, status;
        char buf1[size];

        memset(buf1, 0, size);

       	printf("argc = %d\n",argc);

	pipe(fd);
        printf("Two programs connected\n");
        
        if((child=fork())==0)
        {
                close(fd[0]);
		close(1);
		dup(fd[1]);
               	execlp(argv[1],argv[1],0);
        }              
       	else
	{
		wait(&status);
		close(fd[1]);
		close(0);
		dup(fd[0]);
		execlp(argv[2],argv[2],0);
        }       
	
	return 0;   
}

     

