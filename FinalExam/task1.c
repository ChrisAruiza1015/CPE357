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
int fd[2];
int *childProcess;


void fct() //Overwriting STDIN
{
    dup2(fd[0],STDIN_FILENO);
}

int main()
{

    int parentpid = getpid();
    childProcess = mmap(NULL,4,PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    int* childPID1 = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    int* childPID2 = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    int* childPID3 = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    int* childPID4 = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

 
    int save_std = dup(STDIN_FILENO);
    char input [1000];
    char num [1000];

    pipe(fd);

    signal(SIGUSR1,fct);
//CHILD1
    if (fork() == 0)
    {
        *childPID1 = getpid();
        int count = 0;
        while (1)
        {
            sleep(5);
            write(fd[1],"5s x",strlen("5s x"));
            count++;
            sprintf(num, " %d\n", count);
            write(fd[1],num,strlen(num));
            kill(parentpid,SIGUSR1);
            
        }
    }
//CHILD2
    if (fork() == 0)
    {
        *childPID2 = getpid();
        int count = 0;
        while (1)
        {
            sleep(10);
            write(fd[1],"10s x",strlen("10s x"));
            count++;
            sprintf(num, " %d\n", count);
            write(fd[1],num,strlen(num));
            kill(parentpid,SIGUSR1);

        }
    }
//CHILD3
    if (fork() == 0)
    {
        *childPID3 = getpid();
        int count = 0;
        while (1)
        {
            sleep(20);
            write(fd[1],"20s x",strlen("20s x"));
            count++;
            sprintf(num, " %d\n", count);
            write(fd[1],num,strlen(num));
            kill(parentpid,SIGUSR1);

        }
    }
//CHILD4
    if (fork() == 0)
    {
        *childPID4 = getpid();
        int count = 0;
        while (1)
        {
            sleep(30);
            write(fd[1],"30s x",strlen("30s x"));
            count++;
            sprintf(num, " %d\n", count);
            write(fd[1],num,strlen(num));
            kill(parentpid,SIGUSR1);

        }
    }

    while (1)
    {
        
        memset(input, 0, 1000);

        printf("Entering Something: \n");
        fflush(0);
        dup2(save_std,STDIN_FILENO);

        read(STDIN_FILENO,input,1000);
        printf("%s",input);
        if (strcmp("q\n",input) == 0)
        {
            kill(*childPID1,SIGKILL);
            kill(*childPID2,SIGKILL);
            kill(*childPID3,SIGKILL);
            kill(*childPID4,SIGKILL);
            break;

        }




    }

    return 0;
}