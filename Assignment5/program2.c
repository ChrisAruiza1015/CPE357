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
#define MATRIX_DIMENSION_XY 10
int main(int argc, char *argv[])
{

    
    
    char *args[4];
    args[0] = malloc(100);
    args[1] = malloc(100);
    args[2] = malloc(100);
    args[3] = NULL;
    // args[4] = malloc(100);

    strcpy(args[0], argv[1]);
    strcpy(args[2], argv[2]);
    int count = 0;
    for (int i = 0; i < atoi(argv[2]); i++)
        if (fork() == 0)
        {

            sprintf(args[1],"%d",i);
            printf("args for %i = %s and pid = %i\n",i,args[1],getpid());
            execv("prog1",args);
            printf("unsucessful!\n");

        }





    return 0;
}