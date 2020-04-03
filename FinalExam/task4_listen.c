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
    printf("Executing listen\n");
    int fd;
    char *content;
    fd = shm_open("write", O_RDWR|O_CREAT, 0777);
    content = mmap(0,sizeof(char)*1000,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);

    while (1)
    {
        sleep(5);
        printf("Checking! \n");
        if (content[0] == 0)
        {
            printf("Nothing to print \n");
        }
        else
        {
            printf("Everything that is inside: %s\n",content);
            content[0] = 0;
        }
    }
}