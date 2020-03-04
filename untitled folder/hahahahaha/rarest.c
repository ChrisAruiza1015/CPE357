#include <time.h>
#include <sys/types.h>
#include <signal.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h> 
#include <string.h>
#include <sys/mman.h>
// a this is a test line


int main()
{
    char *text = (char*)mmap(NULL, 1000, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    int *flag = mmap(NULL,4,PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    int *c1 = mmap(NULL,4,PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    int *c2 = mmap(NULL,4,PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    char parent[1000];
    int f1,g;
    int *alternative = mmap(NULL,4,PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    *alternative = 0;

    f1 = fork();
    if (f1 == 0)
    {
        while (1)
        {
            *c1 = 1;
            *flag = 1;
            while(*flag == 1 && *c2 == 1);
            if (*alternative == 0)
            {
                
                strcpy(text,"My name is Lam Tran and I am an Undergraduate in Computer Engineer at Cal Poly and I hope that this will.\n");
                *alternative = 1;

            }
            else    
            {
                strcpy(text,"I dont know if this lab is going to work but I will try my best hahahahahahaha hehehehehehe hihihihihihi huhu\n");
                *alternative = 0;
            }
            *c1 = 0;
        }

    }
    else
    {
        while (1)
        {
            *c2 = 1;
            *flag = 0;
            while (*flag == 0 && *c1 == 1);
            strcpy(parent,text);
            printf("%s",parent);
            *flag = 0;
            *c2 = 0;
        }

    }

    return 0;
}