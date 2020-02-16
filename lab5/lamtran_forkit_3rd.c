#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{

    int f1,f2, f3, g;

    f1 = fork();
    if (f1 == 0)
    {
        f2 = fork();
        if (f2 == 0)
        {
            printf("1\n");
            return 3;
        }
        else
        {
            wait(&g);
            printf("2\n");
        }
        return 3;
    }
    else
    {
        wait(&g);
        f3 = fork();
        if (f3 == 0)
        {
            printf("3\n");
            return 3;

        }
        else
        {
            wait(&g);
            printf("4\n");
        }

    }

    return 0;


}
