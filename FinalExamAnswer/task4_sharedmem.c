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

int main(int argc, char *argv[])
{
    int fd;
    int *pid;
    if (strncmp(argv[1],"0",1) == 0)
    {
        fd = shm_open("pid", O_RDWR|O_CREAT, 0777);
        ftruncate(fd,sizeof(int*)*1000);
        pid = mmap(0,sizeof(int)*1000,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    }
    if (strncmp(argv[1],"1",1) == 0)
    {
        fd = shm_open("pid", O_RDWR|O_CREAT, 0777);
        pid = mmap(0,sizeof(int)*1000,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
        kill(*pid,SIGKILL);
        shm_unlink("pid");
        close(fd);

    }
    *pid = getpid();

    printf("Pid = %i",*pid);

    if (strncmp(argv[1],"0",1) == 0)
    {
        while(1)
        {
            printf("Random number = %i\n",rand());
        }
    }



    return 0;
}