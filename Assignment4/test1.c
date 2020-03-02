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


void fct()
{
    fflush(fd[0]);
    dup2(fd[0],STDIN_FILENO);
}
int main()
{

    int parentPid = getpid();
    pipe(fd);
    printf("fd[0] = %i \n fd[1] = %i\n",fd[0],fd[1]);
    close(fd[0]);
    close(fd[1]);
    pipe(fd);
    printf("fd[0] = %i \n fd[1] = %i\n",fd[0],fd[1]);    signal(SIGUSR1,fct);
    char fileName [1000];
    int savestd = dup(STDIN_FILENO);

    while(1)
        {
        fflush(0);
        read(STDIN_FILENO,fileName,1000);
        printf("%s",fileName);
        dup2(savestd,STDIN_FILENO);
        if (strncmp(fileName,"..",2) == 0)
        {  
            printf("finding\n");
            int f1 = fork();
             if (f1 == 0)
                {
                write(fd[1],"Overwriting",12);
                sleep(3);
                kill(parentPid,SIGUSR1);
                kill(getpid(),SIGKILL);
                // if (fork() == 0)
                // {
                //     write(fd[1],"tranlam",8);
                //     kill(parentPid,SIGUSR1);
                // }
            }
        }
    }
   
     


    return 0;
}