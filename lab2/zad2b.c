#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#define size 32

int main(int argc, char** argv)
{
	char buf[size];	
	memset(buf, 0, size);

	char *name = "/tmp/myfifo2";
	int fd;
	int bytes;	

	fd = open(name, O_RDONLY);

	if(fd<0)
	{
		fprintf(stderr, "Cannot open the fifo file\n");
		exit(2);
	}

	for(int i = 0; i < 1024; i++)
	{
		bytes = read(fd, buf, size);
		printf("Msg: %s\n", buf);
	}

	close(fd);
	return 0;
}
