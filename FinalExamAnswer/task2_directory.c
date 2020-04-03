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

int main(int argc, char *argv[])
{

    DIR *dir;
    struct stat st;
    struct dirent *dent;
    char fileName[2000];
    char subDirect[2000];
    char cwd[2000];
    char cwsd[2000];
    getcwd(cwd,sizeof(cwd));
    int parentPid = getpid();
    int childpid;    
    char alldirectory [1000];
    strcpy(subDirect,"Nosub");


    dir = opendir(".");
    strcpy(fileName,argv[1]);
    if (argv[2])
    {
        strcpy(subDirect,argv[2]);
    }
    printf("Filename = %s\n",fileName);
    printf("subDirect = %s\n", subDirect);

    if (strncmp(subDirect,"Nosub",strlen("Nosub")) == 0)
    {
        int found = 0;
        while ((dent = readdir(dir)) != NULL)
        {
            stat(dent->d_name, &st);
            if ((strcmp(dent->d_name,argv[1]) == 0) && !(S_ISDIR(st.st_mode)))
            {
                strcat(alldirectory,"file is found in ");
                strcat(alldirectory,cwd);
                found = 1;
            }
        }
        if (found == 0)
        {
            strcat(alldirectory,"File isn't found");
        }

    }
    else
    {
        int found = 0;
        int filefound = 0;
        while ((dent = readdir(dir)) != NULL)
        {
            stat(dent->d_name, &st);
            if ((strcmp(dent->d_name,argv[2]) == 0) && (S_ISDIR(st.st_mode)))
            {
                found = 1;
            }
        }
        if (found == 1)
        {
            chdir(subDirect);
            dir = opendir(".");
            getcwd(cwsd,sizeof(cwsd));
            while ((dent = readdir(dir)) != NULL)
            {
                stat(dent->d_name, &st);
                if ((strcmp(dent->d_name,argv[1]) == 0) && !(S_ISDIR(st.st_mode)))
                {
                    strcat(alldirectory,"file is found in subdirectory ");
                    strcat(alldirectory,cwsd);
                    filefound = 1;
                }
            }
        }
        if (filefound == 0)
        {
            strcat(alldirectory,"File isn't present");
        }
    }
    printf("%s",alldirectory);


    return 0;
}