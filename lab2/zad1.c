#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define size 32

int main(int argc, char** argv)
{	

	char buf[size];
	
	char *name = "myfifo";
	int fd;
	int res;

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

	close(1);
	dup(fd);
	
	printf("I am The Creator of Great Fifo");

	close(fd);
	return 0;
}
