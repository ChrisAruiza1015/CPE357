#include <stdio.h>
#include <stdlib.h>

#define PAGESIZE 1024

typedef struct chunkhead
{
unsigned int size;
unsigned int info;
unsigned char *next,*prev;
}chunkhead;

unsigned char *mymalloc(int size);
void myfree(unsigned char*p);
void analyse();


unsigned char myheap[1048576];
int main()
{

    chunkhead *ch = (chunkhead*)myheap;
    ch->size = 1048576 - sizeof(chunkhead);  
    ch->info = 0;//means its free space
    ch->next = ch->prev = 0;
    
    printf("analyzing the first chunk head\n");
//   unsigned char *a,*b,*c;
//     a = mymalloc(1000);
//     b = mymalloc(1000);
//     c = mymalloc(1000);
//     myfree(b);
//     myfree(a);
//     analyse();

	unsigned char *a,*b,*c,*d;
	a = mymalloc(1000);
	b = mymalloc(1024);
	c = mymalloc(1000);
	d = mymalloc(3000);
	analyse();
	myfree(c);
	analyse();
	c = mymalloc(1000);
	analyse();
	b[0] = b[1023] = 0;
	myfree(b);
	myfree(d);
	analyse();
	myfree(c);
	analyse();

    // unsigned char *a,*b,*c, *d, *e, *f;
    // a = mymalloc(1024*10); 
    // b = mymalloc(1024*5);
    // c = mymalloc(1024);
    // analyse();
    // printf("after free\n");

    // myfree(b);
    // analyse();
    // d = mymalloc(1024);
    // e = mymalloc(1024*5);
    // f = mymalloc(1024*4);

    // analyse();
    return 0;
}

void analyse()
{
    chunkhead *ch = (chunkhead*)myheap;
    printf("-----------------------------\n");

    int counter = 1;
    while (ch != NULL)
    {
        printf("\nChunk # %d\n", counter);
        printf("Chunkaddress = %x \n", ch);
        printf("Size %d bytes\n", ch->size);
        if (ch-> info == 1)
        {
            printf("Occupied\n");
        }
        else if (ch->info == 0)
        {
            printf("Free\n");
        }
        printf("Next = %x \n", ch->next);
        printf("Prev = %x \n", ch->prev);
        ch = (chunkhead*)ch->next;
        counter = counter + 1;
    }
}

unsigned char *mymalloc(int sizes)
{
    int size = ((sizes/1024)+1 )*1024;
    chunkhead *ch = (chunkhead*)myheap;
    chunkhead *chNext;
    chunkhead *split;
    int counter = 0;
    int totalSize = 1048576;
    int chunkSize = 0;
    int tempSize;
    unsigned char *address;
    while (ch != NULL)
    {
 
        chunkSize = ch->size + chunkSize;
        printf("%x",ch->next);
        if ((ch->info == 0) && (size == ch->size))
        {
            ch->info = 1;
            return ((unsigned char *)ch) + sizeof(chunkhead);
        }
        else if ((ch->info == 0) && (size < ch->size) && (ch->next != NULL) )
        {
            //split the cell
            printf("haha");
            chunkhead *previous = ch;
            ch->info = 1;
            tempSize = ch->size;
            ch->size = size;
            
            split = (chunkhead*)(unsigned char *)ch + size + sizeof(chunkhead); 
            chNext = (chunkhead*)(ch->next); 


            split->next = chNext;
            split->size = tempSize - size;
            ch->next = (chunkhead*)split;
            split->prev = (chunkhead *)ch;
            chNext->prev = (chunkhead*)split;

            return ((unsigned char*)ch) + sizeof(chunkhead);

        }
        else if ((ch->info == 0) && (size < ch->size))
        {
            chunkhead *previous = ch;
            ch->info = 1;
            tempSize = ch->size;
            ch->size = size;
            
            chNext = (chunkhead*)(unsigned char *)ch + size + sizeof(chunkhead); 
   
            ch->next = (chunkhead*)(chNext);

            ch = ch->next;


            ch->info = 0;
            ch->prev = previous;
            ch->size = tempSize - size - sizeof(chunkhead); 
            ch->next = NULL;

            return ((unsigned char *)previous) + sizeof(chunkhead);


        }
        ch = ch->next;
    }
    return NULL;
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



 

    


}



