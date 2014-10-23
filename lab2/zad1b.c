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
	int bytes;	

	fd = open(name, O_RDWR);

	if(fd<0)
	{
		fprintf(stderr, "Cannot open the fifo file\n");
		exit(2);
	}

	bytes = read(fd, buf, size);
	printf("I found a message \"%s\"\n", buf);

	return 0;
}
