#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#define PAGESIZE 1024

typedef struct chunkhead
{
unsigned int size;
unsigned int info;
unsigned char *next,*prev;
}chunkhead;

unsigned char *mymalloc(int size);
chunkhead* get_last_chunk();
void myfree(unsigned char*p);
void analyse();


void *heap = NULL;

int main()
{
//     // //Test #1
//     unsigned char*a[100];
//     analyse();//50% points
//     for(int i=0;i<100;i++)
//         a[i]= mymalloc(1000);
//     for(int i=0;i<90;i++)
//         myfree(a[i]);

//     analyse(); //50% of points if this is correct
//     myfree(a[95]);
//     a[95] = mymalloc(1000);

//     analyse();//25% points, this new chunk should fill the smaller free one
//     //(best fit)
//     for(int i=90;i<100;i++)
//         myfree(a[i]);

//     analyse();// 25% should be an empty heap now with the start address
// //from the program start
// ////////////////////////////////////////

// //test 2

    unsigned char*a[100];
    clock_t ca, cb;
    ca = clock();
    for(int i=0;i<100;i++)
        a[i]= mymalloc(1000);
    for(int i=0;i<90;i++)
        myfree(a[i]);
    myfree(a[95]);
    a[95] = mymalloc(1000);
    for(int i=90;i<100;i++)
        myfree(a[i]);
    cb = clock();
    printf("\nduration: %f\n", (double)(cb - ca));

    

    return 0;
}

// void analyse()
// {
//     chunkhead *ch = (chunkhead*)heap;
//     printf("-----------------------------\n");

//     int counter = 1;
//     while (ch != NULL)
//     {
//         printf("\nChunk # %d\n", counter);
//         printf("Chunkaddress = %x \n", ch);
//         printf("Size %d bytes\n", ch->size);
//         if (ch-> info == 1)
//         {
//             printf("Occupied\n");
//         }
//         else if (ch->info == 0)
//         {
//             printf("Free\n");
//         }
//         printf("Next = %x \n", ch->next);
//         printf("Prev = %x \n", ch->prev);
//         ch = (chunkhead*)ch->next;
//         counter = counter + 1;
//     }
// }

void analyse()
{
    printf("\n--------------------------------------------------------------\n");
    if(!heap)
    {
        printf("no heap\n");
        printf("program break on address: %x\n",sbrk(0));

        return;
    }
    chunkhead* ch = (chunkhead*)heap;
    for (int no=0; ch; ch = (chunkhead*)ch->next,no++)
    {
        printf("%d | current addr: %x |", no, ch);
        printf("size: %d | ", ch->size);
        printf("info: %d | ", ch->info);
        printf("next: %x | ", ch->next);
        printf("prev: %x", ch->prev);
        printf(" \n");
    }
    printf("program break on address: %x\n",sbrk(0));
}



unsigned char *mymalloc(int sizes)
{
    int size = ((sizes/4096)+1 )*4096;
    
    if (heap == NULL)
    {
        heap = sbrk(size);
        chunkhead *ch = (chunkhead*)heap;
        ch->size = size;
        ch->info = 1;
        ch->prev = NULL;
        ch->next = NULL;
        return ((unsigned char *)ch) + sizeof(chunkhead);
    }

    chunkhead *ch = (chunkhead*)heap;
    chunkhead *smallest = ch;
    chunkhead *chNext;
    chunkhead *split;
    int smallestChunk = 0;
    int counter = 0;
    int chunkSize = 0;
    int tempSize;
    unsigned char *address;
    
    while (ch != NULL)
    {
        if ((ch-> info == 0) && (ch->size < smallest->size))
        {
            smallest = ch;
        }

        if ((ch->info == 0) && (size == ch->size))
        {
            ch->info = 1;
            return ((unsigned char *)ch) + sizeof(chunkhead);
        }
       
        ch = ch->next;
    }


    if ((smallest) && (size < smallest->size) && (smallest->next != NULL))
        {
            //split the cell
            chunkhead *previous = smallest;
            smallest->info = 1;
            tempSize = smallest->size;
            smallest->size = size;
            
            split = (chunkhead*)(unsigned char *)ch + size + sizeof(chunkhead); 
            chNext = (chunkhead*)(smallest->next); 


            split->next = chNext;
            split->size = tempSize - size;
            smallest->next = (chunkhead*)split;
            split->prev = (chunkhead *)ch;
            chNext->prev = (chunkhead*)split;

            return ((unsigned char*)smallest) + sizeof(chunkhead);

        }

    else if ((smallest) && (smallest->info == 0) && (size < smallest->size))
        {
            chunkhead *previous = smallest;
            smallest->info = 1;
            tempSize = smallest->size;
            smallest->size = size;
            
            chNext = (chunkhead*)(unsigned char *)ch + size + sizeof(chunkhead); 
   
            smallest->next = (chunkhead*)(chNext);

            smallest = smallest->next;
            

            smallest->info = 0;
            smallest->prev = previous;
            smallest->size = tempSize - size - sizeof(chunkhead); 
            smallest->next = NULL;

            return ((unsigned char *)previous) + sizeof(chunkhead);


        }

        chunkhead *last = get_last_chunk();
        void *end = sbrk(size);
        ch = (chunkhead*)end;
        ch->size = size;
        ch->info = 1;
        last->next = (unsigned char*)ch;
        ch->prev = (unsigned char*)last;
        ch->next = NULL;
        return ((unsigned char *)ch) + sizeof(chunkhead);
       
}

chunkhead* get_last_chunk() //you can change it when you aim for performance
{
    if(!heap) //I have a global void *startofheap = NULL;
        return NULL;
    chunkhead* ch = (chunkhead*)heap;
    for (; ch->next; ch = (chunkhead*)ch->next);
    return ch;  
}

void myfree(unsigned char *p)
{
    chunkhead *ch = (chunkhead *) (p - sizeof(chunkhead));
    ch->info = 0;

    
    while(ch->info == 0)
    {
        
        
        if ((ch->next) &&((chunkhead *) (ch->next))->info == 0)
        {
            chunkhead * next = (chunkhead *) ch->next;
            ch->size += next->size;
            ch->next = next->next;
            next->prev = (unsigned char *)ch;
        }
        if (ch->prev) {
            ch = (chunkhead *)ch->prev;
        }
        else {
            break;
        }
        
    }

    
    chunkhead *last = get_last_chunk();
    chunkhead *beforeLast;
    beforeLast = (chunkhead*)last->prev;
    if (last->info == 0 && last->next == NULL && last->prev == NULL)
    {
        heap = NULL;
    }
    if (last->info == 0)
    {
        if (beforeLast)
        {
            beforeLast->next = NULL;

        }
        last->prev = NULL;
        brk(sbrk(0) - last->size);
    }


 

    


}



