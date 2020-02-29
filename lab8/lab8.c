
#include <time.h>
#include <sys/types.h>
#include <signal.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h> 
#include <string.h>
#include <sys/mman.h>
#include <stdlib.h>

typedef struct mypipe
{
    char* pipebuffer;
    int buffersize;
    int start_occupied;
    int end_occupied;
}mypipe;

//initializes (malloc) the pipe with a size of "size" and sets start and end.
void init_pipe(mypipe* pipe, int size);
//writes "size" bytes from buffer into the pipe, returns size
int mywrite(mypipe *pipe,char *buffer, int size);
//reads "size" bytes from pipe into buffer, returns how much it read (max size), 0 if
// pipe is empty
int myread(mypipe* pipe, char* buffer, int size);

int main()
{
    // char text[100] = "hello worsld";
    // char *str1;
    // str1 = text;
    // strcat(str1,"lam tran");
    // system(str1);
    // // strcat(str1,"my name is");
    // printf("Str1 = %s\n", str1);
    // // strcat(str1,"lam dinh tran");
    // printf("Str1 = %s\n", str1);


   

    char text[100];
    mypipe pipeA;
    init_pipe(&pipeA, 32);


    mywrite(&pipeA, "hello world", 12);
    mywrite(&pipeA, "it's a nice day", 16);
    myread(&pipeA, text, 12);
    printf("%s\n", text);
    myread(&pipeA, text, 16);
    printf("%s\n", text);
    
    mywrite(&pipeA, "and now we test the carryover", 30);
    myread(&pipeA, text, 30);
    printf("%s\n", text);

    printf("Testing carry over pipeA.pipebuffer[0] = %c\n",pipeA.pipebuffer[0]);

    
}

void init_pipe(mypipe* pipe, int size)
{

    pipe->pipebuffer = (char*)malloc(size);
    pipe->buffersize = size;
    pipe->start_occupied = 0;
    pipe->end_occupied = 0;

}

int mywrite(mypipe *pipe,char *buffer, int size)
{

    int count = 0;
    if (pipe->end_occupied == 0)
    {
        for(int i=0;i<=size && i<pipe->buffersize;i++)
        {
            pipe->pipebuffer[i] = buffer[i]; 
        }
    }

    else

    {
        for (int i = pipe->end_occupied; i <= (pipe->end_occupied + size) /*&& i < pipe->buffersize*/; i++)
        {
            if (count < pipe->buffersize)
            {
                pipe->pipebuffer[i%pipe->buffersize] = buffer[count];
                count++; 
            }
        }
    }

    pipe->end_occupied = (pipe->end_occupied + size) % pipe->buffersize;
    return pipe->end_occupied;

}

int myread(mypipe* pipe, char* buffer, int size)
{
    int count = 0;
    for (int i = pipe->start_occupied; i < (pipe->start_occupied + size) /*&& i < pipe->buffersize*/; i++)
    {
        if (count < pipe->buffersize)
        {
            buffer[count] = pipe->pipebuffer[i % pipe->buffersize];
            count++;
        }
    }
    
    pipe->start_occupied = (pipe->start_occupied + size) % pipe->buffersize;
    
    
    return count;

}



