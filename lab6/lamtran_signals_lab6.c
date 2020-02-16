#include <time.h>
#include <sys/types.h>
#include <signal.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h> 



void sigintHandlerC(int sig)
{
    signal(SIGINT, sigintHandlerC); 
    printf("\n Cannot be terminated using Ctrl+C \n"); 
    fflush(stdout); 
}

void sigintHandlerZ(int sig)
{
    signal(SIGTSTP, sigintHandlerZ); 
    printf("\n Cannot be terminated using Ctrl+Z \n"); 
    fflush(stdout); 
}

int main()
{

    int f1 , g;
    struct stat st;
    DIR *dir;
    struct dirent *dent;
    signal(SIGINT, sigintHandlerC);
    signal(SIGTSTP, sigintHandlerZ);



    while (1)
    {
        printf("Parent ID is %d\n", getpid());

        f1 = fork();


        if (f1 == 0)
        {
        

            while(1)
            {
                printf("Child ID: %d\n",getpid());

                // printf("while loop");
                time_t T= time(NULL);
                struct tm tm = *localtime(&T); //tm.tm_hour, tm.tm_min
                

                printf("%d %d\n",tm.tm_hour, tm.tm_min);

                dir = opendir(".");
                if (dir == NULL)
                {
                    printf("Can't find anything\n");
                }
                else
                {
                    while ((dent = readdir(dir)) != NULL)
                    {
                        printf("%s",dent->d_name);
                        stat(dent->d_name, &st);
                        if (S_ISDIR(st.st_mode))
                            printf("is directory ---- ");
                        printf("\n");
                    }
                }
                closedir(dir);
                sleep(10);
                
            }
            // return 2;
        }
        else
        {
            wait(0);
            printf("yoyoyoyo");
        }
    }
    return 0;

}
