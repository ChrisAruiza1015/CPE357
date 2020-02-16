#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>

int main()
{
    time_t tstart,tstop;
    clock_t start1,end1, start4, end4;
    double cpu_time_used4, cpu_time_used1;

    int *data = (int*)mmap(NULL,sizeof(int)*1000000,PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS,-1,0);
    
    int middle = 1000000/2;
    int g;

    // start timer
    start1 = clock();
    //--- 2 processes ------
    if(fork() == 0)
    {
        for (int i = 0; i < middle; i++)
        {
            data[i] = i*5 + i/2;
        }
        return 5;
    }
    else
    {
        for (int c = middle; c < 1000000; c++)
        {
            data[c] = c*5 + c/2;
        }    
        wait(&g);
    }
    // wait(&g);
    end1 = clock();
    // ---- 4 processes -------
    start4 = clock();
    if(fork() == 0)
    {
        if(fork() == 0)
        {
            for (int i = 0; i < 250000; i++)
            {
                data[i] = i*5 + i/2;
            }
            return 5;
        }
        else
        {
            for (int i = 250000; i < 500000; i++)
            {
                data[i] = i*5 + i/2;
            }
            wait(&g);

        }
        
        return 5;
    }
    else
    {
        
        if(fork() == 0)
        {
            for (int i = 500000; i < 750000; i++)
            {
                data[i] = i*5 + i/2;
            }
            return 5;
        }
        else
        {
            for (int i = 750000; i < 1000000; i++)
            {
                data[i] = i*5 + i/2;
            }
            wait(&g);


        }
    }
    // wait(&g);
    end4 = clock();
    // for (int p = 999998; p < 1000000; p++)
    // {
    //     printf("Element %d: %d\n",p,data[p]);
    // }
    cpu_time_used1 = ((double)(end1-start1));
    printf("Time took to complete 2 processes: %f microseconds\n", cpu_time_used1);  
    cpu_time_used4 = ((double)(end4-start4));
    printf("Time took to complete 4 processes: %f microseconds\n", cpu_time_used4);    

    munmap(data,sizeof(data));

    return 0;
}