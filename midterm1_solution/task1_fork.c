#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <string.h>
#include <time.h>



int main()

{

    int f1,f2,f3,b;
    f1 = fork();
    if (f1 == 0)
    {
        f2 = fork();
        if (f2 == 0)
        {
            printf("A");
            return 3;
        }
        else
        {
            wait(&b);
            printf("B");
        }
        return 3;
    }
    else
    {
        wait(&b);
        f3 = fork();
        if (f3 == 0)
        {
            printf("C");
            return 3;
        }
        else{
            wait(&b);
            printf("D");
        }

    }

}