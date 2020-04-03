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
#define KNRM "\x1B[0m"
#define KRED "\x1B[31m"
#define KGRN "\x1B[32m"
#define KYEL "\x1B[33m"
#define KBLU "\x1B[34m"
#define KMAG "\x1B[35m"
#define KCYN "\x1B[36m"
#define KWHT "\x1B[37m"
#define RESET "\x1B[0m"
#include <stdlib.h>
int fd[2];
int *childProcess;

void fct()
{
    dup2(fd[0],STDIN_FILENO);
}

int main()
{

    int parentpid = getpid();
    int save_std = dup(STDIN_FILENO);
    childProcess = mmap(NULL,4,PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    int* childPID1 = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    int* childPID2 = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    int* childPID3 = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    int* childPID4 = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    char input[1000];
    signal(SIGUSR1,fct);

    if (fork() == 0)
    {
        *childPID1 = getpid();
        printf("childPID1 = %d\n",*childPID1);
        while(1)
        {
            sleep(2);
            write(fd[1],KRED,strlen(KRED));
            kill(parentpid,SIGUSR1);
        }

    }
    
    if (fork() == 0)
    {
        *childPID2 = getpid();
        printf("childPID2 = %d\n",*childPID2);
        while(1)
        {
            sleep(4);

            write(fd[1],KBLU,strlen(KBLU));
            kill(parentpid,SIGUSR1);
        }
    }

    if (fork() == 0)
    {
        *childPID3 = getpid();
        printf("childPID3 = %d\n",*childPID3);
        while(1)
        {
            sleep(6);

            write(fd[1],KGRN,strlen(KGRN));
            kill(parentpid,SIGUSR1);
        }
    }

    if (fork() == 0)
    {
        *childPID4 = getpid();
        printf("childPID4 = %d\n",*childPID4);
        while(1)
        {
            sleep(8);

            write(fd[1],KYEL,strlen(KYEL));
            kill(parentpid,SIGUSR1);
        }
    }



    while (1)
    {
        printf("Lams_Program_task1 $ ");
        fflush(0);
        dup2(save_std,STDIN_FILENO);
        read(STDIN_FILENO,input,1000);
        printf("input = %s",input);
        if (strncmp(input,"q",1) == 0)
        {
            kill(*childPID1,SIGKILL);
            kill(*childPID2,SIGKILL);
            kill(*childPID3,SIGKILL);
            kill(*childPID4,SIGKILL);
            printf(RESET);
            break;
        }
        

    }
    

    return 0;
}