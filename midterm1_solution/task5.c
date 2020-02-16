#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <string.h>

int main()
{
    char userIN[100];
    char *text[10];
    
    
    for(int i = 0; i < 10; i++)
    {
        text[i] = mmap(NULL, 100*sizeof(char), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0 );
        text[i][0] = 0;
    }
    while(1)
    {
        scanf("%s", userIN);
        if(strcmp(userIN, "print") == 0)
        {
            printf("these are the elements in the heap:\n");
            for(int i =0; i < 10; i++)
            {  
                if(text[i][0] != NULL)
                {
                    printf("%s\n", text[i]);
                }
            }
        }
        else if(strcmp(userIN, "quit") == 0)//quit program
        {
            for(int i = 0; i < 10; i++)
            {
                munmap(text[i], 100);
            }
            break;
        }
        else
        {
            for(int i = 0; i < 10; i++)
            {
                if(strcmp(text[i], userIN) == 0)//if the item is already in the list
                {
                    text[i][0] = 0;
                    userIN[0] = '\0';
                    
                }
            }
            for(int i = 0; i < 10; i++)
            {
                if(text[i][0] == 0)
                {
                    strcpy(text[i], userIN);
                    userIN[0] = '\0';
                    break;
                }
            }
            
        }

    }

    return 0;
}