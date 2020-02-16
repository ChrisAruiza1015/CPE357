#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h> 


int main()
{
    struct stat st;
    DIR *dir;
    struct dirent *dent;
    int f1, g;
    char fileName[1000];

    printf("Enter a file name in this current directory: \n");
    scanf("%s",fileName);

    f1 = fork();

    if (f1 == 0)
    {

        printf("child process rn");
        scanf("%s",fileName);
        return 0;

    }
    else
    {
        waitpid();
    }

    
    

    return 0;   
}