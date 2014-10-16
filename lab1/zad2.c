#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define size 16
#define cntr 128
int main(int argc, char**argv)
{
        int fd[2];
        int child, count, status;
        int bytes = 1;
        char buf1[size], buf2[size];

        memset(buf1, 0, size);
        memset(buf2, 0, size);

        pipe(fd);
        printf("Main process: I will pass all the data you will type to my child\n");
        
        if((child=fork())==0)
        {
                close(fd[1]);
                for(count = 0; count < cntr; count++)
                {
                        bytes = read(fd[0], buf2, sizeof(buf2));
                        if(bytes) printf("I've got some gifts from my parent: %s\n",buf2);
                	printf("Bytes received: %d\n",bytes);
		}  
		printf("No more gifts...\n");
		exit(0);
        }
              
       	close(fd[0]);           
       	for(count = 0; count < cntr; count++)
       	{
        	sprintf(buf1,"%d",count);
		write(fd[1], buf1, sizeof(buf1));                               
        }       
       	wait(&status);
	
	return 0;   
}

     

