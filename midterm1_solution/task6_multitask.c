#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <string.h>
#include <time.h>


int main()
{

    int *data = mmap(NULL, 1000000*sizeof(int),PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1,0);
    int f1, i;
    clock_t start1, start2, end1, end2;
    double total1, total2;
    int b1, b2, b3, g;

    f1 = fork();
    start1 = clock();

    if (f1 == 0)
    {
        for (int i = 0; i < 1000000/2; i++)
        {
            data[i] = i*5 + i/2;
        }

        return 3;
    }
    else
    {   
        for (int i = 1000000/2; i < 1000000; i++)
        {
            data[i] = i*5 + i/2;
        }
        
        wait(&i);

    }
    end1 = clock();


    
    b1 = fork();
    start2 = clock();
    if (b1 == 0)
    {
        b2 = fork();
        if (b2 == 0)
        {
            for (int i = 0; i < 1000000/4; i++)
            {
                data[i] = i*5 + i/2;
            }
        }
        else
        {
            for (int i = 1000000/4; i < 1000000/2; i++)
            {
                data[i] = i*5 + i/2;
            }
            wait(&g);
        }
        return 3;
    }
    else
    {
        b3 = fork();
        if (b3 == 0)
        {
            for (int i = 1000000/2; i < (1000000 - 1000000/4); i++)
            {
                data[i] = i*5 + i/2;
            }
            return 4;
        }
        else
        {
            for (int i = (1000000 - 1000000/4); i < 1000000; i++)
            {
                data[i] = i*5 + i/2;
            }
            wait(&g);
        }
    }

    end2 = clock();

    total2 = (double)(end2 - start2);
    total1 = (double)(end1 - start1);

    printf("Time it took for 2 process %f", total1);
    printf("\nTime it took for 4 processes %f", total2);









}