#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <string.h>

int main()
{
    char *text[10] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    char userInput[100];
    int keepAdding = 1;

    // for(int i = 0; i < 10; i++)
    // {
    //     text[i] = mmap(NULL, 100*sizeof(char), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0 );
    // }

    while (keepAdding == 1)
    {
        printf("Enter your word: \n");
        scanf("%s",userInput);
        if (strcmp(userInput,"print") == 0)
        {
            printf("These are the elements on the heap: \n");
            for (int i = 0; i < 10; i++)
            {
                if (text[i] != NULL) printf("%s\n",text[i]);

            }
        }
        else if (strcmp(userInput,"stop") == 0)
        {
            printf("Stopping program! \n");
            for (int i = 0; i < 10; i++)
            {
                munmap(text[i],100);
            }
            keepAdding = 0;

        }
        else
        {
            int duplicate = 0;
            int duplicateSpot;
            int freeSpace = -1;

           for (int i = 0; i < 10; i++)
           {
               if (text[i] != NULL && strcmp(text[i],userInput) == 0)
               {
                   duplicate = 1;
                   duplicateSpot = i;
                   break;
               }
               if (text[i] == NULL && freeSpace == -1)
               {
                   freeSpace = i;
               }
           }

           if (duplicate == 1)
           {
               munmap(&text[duplicateSpot],100);
               text[duplicateSpot] = NULL;
           }
           if (duplicate == 0)  
           {
               char *textptr = (char*)mmap(NULL,100,PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS,-1,0);
               strcpy(textptr,userInput);
               text[freeSpace] = textptr;
           }
        }
        

    }



}