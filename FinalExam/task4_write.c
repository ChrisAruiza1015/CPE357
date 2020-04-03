#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
int main()
{
    int fd;
    char *content;
    fd = shm_open("write", O_RDWR|O_CREAT, 0777);
    ftruncate(fd,sizeof(char*)*1000);
    content = mmap(0,sizeof(char)*1000,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    int *childPID = mmap(NULL,sizeof(int),PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS,-1,0);

    char input[1000];
    char *args[2];
    args[0] = malloc(sizeof(char)*15);
    strcpy(args[0],"listen");
    args[1] = 0;


    if (fork() == 0)
    {
        *childPID = getpid();
        execv("listen",args);
        return 0;
    }

    while (1)
    {
        scanf("%s",input);
        strcat(content,input);
        if (strcmp("q",input) == 0)
            break;
    }
    kill(*childPID,SIGKILL);
    wait(0);
    close(fd);
    shm_unlink("write");

    return 0;
}