#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>

/*DEFINE SECTION*/

#define PORT 3333
#define QUEUE 5
#define SIZE 16

/*Socket handler in thread*/
void* socket_thread(void *socket)
{
	int sock = *(int*)socket;
	printf("Connection OK\n");
	char msg[SIZE];
	memset(msg,0,SIZE);
	sprintf(msg, "Connected\n");
	sleep(10);
	int chars;
	if((chars=write(sock,msg,SIZE))<0)
	{
		perror("thread");
	}
	close(sock);
	printf("Connection CLOSED %d\n",chars);
	
	return (void*)0;
} 

int main(int argc, char** argv)
{
	int sock, lsock, i = 0;
	socklen_t length;
	struct sockaddr_in server;	
	pthread_t pthread_id[10];

	/*Create a tcp socket and verify the act of creation*/
	sock = socket(AF_INET, SOCK_STREAM,0);
	if(sock<0) 
	{	
		printf("Cannot create socket at line %d\n",__LINE__);
		exit(1);	
	}

	/*Set up the server structure*/
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = ntohs(PORT);

	length = sizeof(server);

	/*Bind socket and start to listen*/
	if ( bind(sock, (struct sockaddr *)&server, length ))
	{
		printf("Cannot bind a socket at line %d\n",__LINE__);	
		exit(1);
	}
	listen(sock, QUEUE);

	/*Do server things*/
	do
	{
	
		lsock = accept(sock,0,0);

		if(lsock == -1)
		{
			printf("Cannot accept connection at line %d\n",__LINE__);	
			exit(1);
		}
		else 
		{
			/*Handling connection in new thread*/
			if(pthread_create(&pthread_id[(i++)%10], NULL, socket_thread, (void*) &lsock) < 0)
			{
				printf("Cannot create thread %d\n",__LINE__);
				exit(1);
			}
		}


	}while(1);	

	return 0;
}
