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
	int res;
	int bytes;

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
	
	for(int i=0;;i++)
	{
		sprintf(buf,"%d: %s",i,"hello");
		bytes = write(fd, buf, size);	
		if(bytes==size) printf("Sent message no. %d\n", i);
		if(bytes<0)  perror("message not sent");

		sleep(4);	
	}
	close(fd);
	return 0;
}
