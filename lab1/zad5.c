#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define size 1024
#define pcnt 10
int main(int argc, char**argv)
{
    int fd[pcnt-1][2];
    int child, status;

    char programs[pcnt][16];
    char args[pcnt][16];

    for(int i = 0; i < pcnt; i++)
    {
        memset(programs[i],0,16);
	memset(args[i],0,16);
    }

    int dot = 1;
    int program = 0;

    int program_cnt = 0;

    for(int i = 1; i < argc; i++)
    {

	if(strcmp(argv[i], ".") == 0)
        {
            program = 0;
            dot = 1;
            program_cnt++;
	    continue;
        }	

        if(dot==1 && program==0)
        {
            sprintf(programs[program_cnt],"%s",argv[i]);
            program = 1;
            dot = 0;
            continue;
        }
        if(dot==0 && program==1)
        {
            sprintf(args[program_cnt],"%s",argv[i]);
            continue;

        }

    }

    /*
    for(int i = 0; i <= program_cnt; i++)
    {
        printf("program[%d] + arg[%d] = %s + %s\n",i,i,programs[i],args[i]);
    }

    printf("argc = %d\n",argc);

    for(int i = 0; i < argc; i++)
    {
        printf("arg[%d] = %s\n",i,argv[i]);
    }
    */

    for(int i=0; i < pcnt; i++)
    {
        pipe(fd[i]);
    }

    for(int i = 0; i <= program_cnt; i++)
    {
        if((child=fork())==0)
        {

	    if(i == (program_cnt-1))
            {
                close(0);
                dup(fd[i-1][0]);
		if(strcmp(args[i], "")==0) execlp(programs[i],programs[i],0);
		else execlp(programs[i],programs[i],args[i],0);
            }

            if(i==0)
            {
                close(1);
                dup(fd[i][1]);

		if(strcmp(args[i], "")==0) execlp(programs[i],programs[i],0);
		else execlp(programs[i],programs[i],args[i],0);
            }

            close(0);
            dup(fd[i-1][0]);
            close(1);
            dup(fd[i][1]);
	    if(strcmp(args[i], "")==0) execlp(programs[i],programs[i],0);
	    else execlp(programs[i],programs[i],args[i],0);
        }
    }


    for(int i = 0; i < pcnt; i++)
    {
        wait(&status);
    }


    return 0;
}



