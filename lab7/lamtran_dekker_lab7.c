#include <time.h>
#include <sys/types.h>
#include <signal.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h> 
#include <string.h>
#include <sys/mman.h>


int main()
{
    char *text = (char*)mmap(NULL, 1000, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    int *alternative = mmap(NULL, 4, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    *alternative = 0;
    char parent[1000];

    int f, g;
    int *childProcessEnter = mmap(NULL, 4, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    int *parentProcessEnter = mmap(NULL, 4, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    int *completion = mmap(NULL, 4, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    int *favouredThread = mmap(NULL, 4, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    *favouredThread = 1;
    int *parentCompletion = mmap(NULL, 4, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    *childProcessEnter = 0;
    *parentProcessEnter = 0;


    f = fork();
    if (f == 0)
    {
        while (1)
        {
            *childProcessEnter = 1;

            while (*parentProcessEnter == 1)
            {
                if (*favouredThread == 2)
                {
                    *childProcessEnter = 0;
                }
                while (*favouredThread == 2);
                *childProcessEnter = 1;
            }
            //Critical Section here

            if(*alternative == 0)
            {
                strcpy(text,"My name is Lam Tran and I am an Undergraduate in Computer Engineer at Cal Poly and I hope that this will.\n");
                *alternative = 1;         

            }
            else
            {

                strcpy(text,"I dont know if this lab is going to work but I will try my best hahahahahahaha hehehehehehe hihihihihihi huhu\n");
                *alternative = 0;
            }
            *favouredThread = 2;
            *childProcessEnter = 0;

            



        }
        return 0;
    }

    else
    {
        while(1)
        {
            *parentProcessEnter = 1;


            while (*childProcessEnter == 1)
            {
                if (*favouredThread == 1)
                {
                    *parentProcessEnter = 0;
                }
                while (*favouredThread == 1);
                *parentProcessEnter = 1;
            }
            strcpy(parent,text);
            printf("%s",parent);
            *favouredThread = 1;
            *parentProcessEnter = 0;
        }

    }





}