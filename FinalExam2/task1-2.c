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
FILE *file;

void fct()
{
    dup2(fd[0],STDIN_FILENO);
}

int main()
{

    int* childPID1 = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    int *overwrite = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    *overwrite = 1;
    int save_std = dup(STDIN_FILENO);
    char input[1000];
    pipe(fd);

    if (fork() == 0)
    {

        file = fopen("result.txt","w+");
        char bufferout[1000];
        int byte;
        *childPID1 = getpid();
        while (1)
        {

            sleep(5);
            byte = read(fd[0],bufferout,1000);
            if (byte != 0)
            {
                printf("writing to file now\n");
                fwrite(bufferout,1,byte,file);
                fflush(file);

            }

        }
        
    }
    while(1)
    {
        fflush(0);
        printf("type something bro\n");
        dup2(save_std,STDIN_FILENO);
        scanf("%s",input);

        printf("writing to pipe\n");
        write(fd[1],input,strlen(input));
        
        if (strncmp("q",input,1) == 0)
        {
            kill(*childPID1,SIGKILL);
            fclose(file);

            wait(0);
            break;
        }

        
    }



    return 0;
}