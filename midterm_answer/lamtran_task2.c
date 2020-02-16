#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <string.h>
#include <time.h>

int main () {

    unsigned char *firstMiddleLastCity = mmap(NULL, 1000000,PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1,0);
    unsigned char first[50];
    unsigned  char last[50];
    unsigned  char middle[50];
    unsigned char city[50];
    strcpy(first,  "[Lam] ");
    strcpy(middle, "[Dinh] ");
    strcpy(last, "[Tran] ");
    strcpy(city, "[San Jose] ");
    
    int f1,f2,f3, g;


    f1 = fork();
    if (f1 == 0)
    {
        f2 = fork();
        if (f2 == 0)
        {
            strcat(firstMiddleLastCity, first);
            return 3;
        }
        else
        {
            wait(&g);
            strcat(firstMiddleLastCity,middle);


        }
        return 3;
        
    }
    else
    {
        wait(&g);
        f3 = fork();
        if (f3 == 0)
        {
            strcat(firstMiddleLastCity,last);
            return 3;

        }
        else
        {
            wait(&g);
            strcat(firstMiddleLastCity,city);
        }
        
        printf("Final destination string : |%s|", firstMiddleLastCity);

    }
    munmap(&firstMiddleLastCity,1000000);
   
}