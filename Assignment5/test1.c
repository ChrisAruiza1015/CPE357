#include <stdio.h>
#include <stdlib.h>

int main()
{

    for (int c = 0; c < 100; c++)
    {
    int n = rand() % 10;
    printf("n = %i\n",n);
    }

    for (int c = 0; c < 100; c++)
    {
    int b = rand() % 10;
    printf("b = %i\n",b);
    }
    return 0;
}