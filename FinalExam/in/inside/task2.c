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
#define KBLU  "\x1B[34m"
#define RESET "\x1B[0m"
#include <stdlib.h>
int fd[2][2];
int *wordCount;


void fct() //Overwriting STDIN
{
    dup2(fd[1][0],STDIN_FILENO);
    *wordCount = 0;
}

int main()
{
    int parentpid = getpid();
    int save_std = dup(STDIN_FILENO);
    int *childPid = mmap(NULL,4,PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    wordCount = mmap(NULL,4,PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    int *numberOfWords = mmap(NULL,4,PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    *numberOfWords = 0;
    *wordCount = 1;
    for (int i = 0; i<2; i++)
    {
        pipe(fd[i]);
    }

    signal(SIGUSR1,fct);
    char input [1000];

    if (fork() == 0)
    {
        char bufferOutput[1000];
        char pipeIn [1000];
        *childPid = getpid();

        int flag = 0;
        int start_sec = 0;
        fflush(0);
        while(1)
        {
            sleep(10);
            printf("Word Count = %i \n",*numberOfWords);
            read(fd[0][0],pipeIn,1000);
            write(fd[1][1],"\n",strlen("\n"));
            kill(parentpid,SIGUSR1);



        }
    }
    while (1)
    {
        memset(input, 0, 1000);

        printf("Entering Something: ");
        fflush(0);
        read(STDIN_FILENO,input,1000);
        dup2(save_std,STDIN_FILENO);
        if (*wordCount == 1)
        {
            *numberOfWords = *numberOfWords + 1;
        }
        write(fd[0][1],input,strlen(input));
        printf("\n input = %s",input);
        *wordCount = 1;
        if (strcmp(input,"q\n") == 0)
        {
            kill(*childPid,SIGKILL);
            break;
        }



    }

}