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
void findDir(DIR *dir, char *directTarget, char*homeDirect, char alldirectory[1000], char *word);
int main(int argc, char *argv[])
{

    DIR *dir;
    struct stat st;
    struct dirent *dent;
    char fileName[20000];
    int parentPid = getpid();
    int childpid;    
    char alldirectory [1000];
    char cwd[1000];
    getcwd(cwd,sizeof(cwd));
    int subdir = 0;

    if (strcmp(argv[2],"s") == 0)
    {
        subdir = 1;
    }

    dir = opendir(".");

    if (subdir == 1)
    {

        while ((dent = readdir(dir)) != NULL)
        {

            stat(dent->d_name, &st);
            if ((strcmp(dent->d_name, argv[1]) == 0) && !(S_ISDIR(st.st_mode)))
            {
                strcat(alldirectory,cwd);
                strcat(alldirectory,"\n");
            }
            if ((strcmp(dent->d_name, ".") != 0) && (strcmp(dent->d_name, "..") != 0) && S_ISDIR(st.st_mode)) 
            {               
                findDir(dir,dent->d_name,cwd,alldirectory,argv[1]);
            }
        }
    }
    if (subdir == 0)
    {
        while ((dent = readdir(dir)) != NULL)
        {
            stat(dent->d_name, &st);

            if ((strcmp(dent->d_name, argv[1]) == 0) && !(S_ISDIR(st.st_mode)))
            {
                strcat(alldirectory,cwd);
                strcat(alldirectory,"\n");
            }
         }

    }
    printf("File %s is found in \n%s",argv[1],alldirectory);

    return 0;
}
void findDir(DIR *dir, char *directTarget, char*homeDirect, char alldirectory[1000],char *word)
{

    struct stat st;
    struct dirent *dent;
    chdir(directTarget);
    char cwd[1000];
    getcwd(cwd,sizeof(cwd));
    dir = opendir(".");

    while ((dent = readdir(dir)) != NULL)
    {

        stat(dent->d_name, &st);
        if ((strcmp(dent->d_name, word) == 0) && !(S_ISDIR(st.st_mode)))
        {
            strcat(alldirectory,cwd);
            strcat(alldirectory,"\n");
        }
        if ((strcmp(dent->d_name, ".") != 0) && (strcmp(dent->d_name, "..") != 0) && S_ISDIR(st.st_mode)) 
        {               
            findDir(dir,dent->d_name,cwd,alldirectory,word);
        }
    }
    chdir(homeDirect);


}

