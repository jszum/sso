#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>

/*DEFINE SECTION*/

#define PORT 3333


int main(int argc, char** argv)
{
	int sock;
	struct sockaddr_in server;
	struct hostent *hp;

	/*Create a tcp socket and verify the act of creation*/
	sock = socket(AF_INET, SOCK_STREAM,0);
	if(sock<0) 
	{	
		printf("Cannot create socket at line %d\n",__LINE__);
		exit(1);	
	}

	/*Set up the server structure*/
	server.sin_family = AF_INET;
	
	if((hp = gethostbyname(argv[1])) == 0)
	{
		printf("Cannot get host by name at line %d\n",__LINE__);
		exit(1);	
	}

	memcpy(&server.sin_addr, hp->h_addr, hp->h_length);
	server.sin_port = htons(PORT);

	/*Try to connect*/
	if ( connect(sock, (struct sockaddr *)&server, sizeof(server) ))
	{
		printf("Cannot bind a socket at line %d\n",__LINE__);	
		exit(1);
	}
	close(sock);


	return 0;
}
