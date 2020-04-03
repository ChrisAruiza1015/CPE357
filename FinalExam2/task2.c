#include <time.h>
#include <sys/types.h>
#include <signal.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h> 
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>
#define KRED "\x1B[31m"
#define KGRN "\x1B[32m"
#define RESET "\x1B[0m"

#include <stdlib.h>
int fd[2];
void fct()
{
    dup2(fd[0],STDIN_FILENO);
}

int main()
{
    int *childPid = (int*)mmap(NULL,sizeof(int),PROT_READ|PROT_WRITE,MAP_ANONYMOUS|MAP_SHARED,-1,0);
    int *childPid2 = (int*)mmap(NULL,sizeof(int),PROT_READ|PROT_WRITE,MAP_ANONYMOUS|MAP_SHARED,-1,0);
    int parentpid = getpid();
    signal(SIGUSR1, fct);
    int save_std = dup(STDIN_FILENO);
    char input[1000];
    pipe(fd);
    if (fork() == 0)
    {
        *childPid = getpid();
        char bufferOutput[1000];

        while(1)
        {
            memset(bufferOutput,0,1000);
            sleep(5);

            time_t T;
            time(&T);
                
            strcat(bufferOutput,KGRN);
            strcat(bufferOutput,ctime(&T));
            write(fd[1], bufferOutput, 1000);
            kill(parentpid,SIGUSR1);


        }
    }

    if (fork() == 0)
    {
        *childPid2 = getpid();
        char bufferOutput[1000];

        while(1)
        {
            memset(bufferOutput,0,1000);

            sleep(11);

            time_t T;
            time(&T);
                
            strcat(bufferOutput,KRED);
            strcat(bufferOutput,ctime(&T));

            write(fd[1], bufferOutput, 1000);
            kill(parentpid,SIGUSR1);

        }
    }
    while(1)
    {
        fflush(0);
        printf(RESET);
        printf("Type something boy\n");
        memset(input,0,1000);
        dup2(save_std,STDIN_FILENO);
        read(STDIN_FILENO,input,1000);
        printf("Input = %s",input);


    }

    
}