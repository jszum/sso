#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char** argv)
{	

	char *name = "/tmp/mpfifo";
	int fd;
	int res;
	int bytes;
	int key;

	res = mkfifo(name, O_CREAT | 0660);
	
	if(res<0 && errno!=EEXIST) 
	{ 
		fprintf(stderr, "Mkfifo failed\n");
		exit(1);
	}

	fd = open(name, O_RDWR);

	if(fd<0)
	{
		fprintf(stderr, "Cannot open the fifo file\n");
		exit(2);
	}
	
	printf("Menu options\n");
	printf("1 - Play/Pause\n");
	printf("2 - Loop forever\n");
	printf("3 - Unloop\n");
	printf("4 - Go forward\n");
	printf("5 - Go backward\n");
	printf("0 - Quit console mode\n");

	do
	{
		scanf("%d", &key);
		switch(key)
		{
			case 1:	bytes = write(fd, "pause\n", strlen("pause\n")); 
				if(bytes < 0)  perror("message not sent");
				break;

			case 2:	bytes = write(fd, "loop 1\n", strlen("loop 1\n")); 
				if(bytes < 0)  perror("message not sent");	
				break;

			case 3:	bytes = write(fd, "loop -1\n", strlen("loop -1\n")); 
				if(bytes < 0)  perror("message not sent");	
				break;
			
			case 4:	bytes = write(fd, "seek 5\n", strlen("seek 5\n")); 
				if(bytes < 0)  perror("message not sent");	
				break;

			case 5:	bytes = write(fd, "seek -5\n", strlen("seek -5\n")); 
				if(bytes < 0)  perror("message not sent");	
				break;
				
			case 0:	bytes = write(fd, "quit\n", strlen("quit\n")); 
				if(bytes < 0)  perror("message not sent");	
				break;

		}
	}while (key != 0);
	
	close(fd);
	return 0;
}
