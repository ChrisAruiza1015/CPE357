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

int main()
{

    char *buffer1 = "lamtran";
    char *buffer2 = "lam";
    printf("comparison = %d",strncmp(buffer1,buffer2,strlen(buffer2)));


    return 0;
}