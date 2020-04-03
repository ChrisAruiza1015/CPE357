#include <time.h>
#include <sys/types.h>
#include <signal.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h> 
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>
#define KBLU  "\x1B[34m"
#define RESET "\x1B[0m"
#include <stdlib.h>
int fd[2];

void fct()
{
    dup2(fd[0],STDIN_FILENO);
}
int main(int argc, char *argv[])
{
    srand(time(0));
    char text[1000];
    char userinput[1000];
    pipe(fd);
    int savestd = dup(STDIN_FILENO);
    int parentpid = getpid();
    int *childPid = mmap(NULL,4,PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);


    signal(SIGUSR1,fct);


    for (int i = 0; i < 4; i++)
    {
        char onedigit = (rand() % 4) + 48;

        text[i] = onedigit;
    }
    printf("Here is the cheat sheet! Don't look at it if you want challenge %c%c%c%c\n",text[0],text[1],text[2],text[3]);
    if (fork() == 0)
    {
        *childPid = getpid();
        sleep(5);
        write(fd[1],"5 second have passed, you have 15 second remaining!",1000);
        kill(parentpid,SIGUSR1);
        sleep(5);
        write(fd[1],"10 second have passed, you have 10 second remaining!",1000);
        kill(parentpid,SIGUSR1);

        sleep(5);
        write(fd[1],"15 second have passed, you have 5 second remaining!",1000);
        kill(parentpid,SIGUSR1);

        sleep(5);
        write(fd[1],"20 second have passed, time is up!",1000);
        kill(parentpid,SIGUSR1);
        kill(parentpid,SIGKILL);
        return 0;
    }
    
    while(1)
    {
        int firstcorrect = 0;
        int secondcorrect = 0;
        int thirdcorrect = 0;
        int fourthcorrect = 0;
        printf("\nPlease enter4 digits number: (EXAMPLE: 0123)! ");
        fflush(0);

        memset(userinput,0,1000);
        dup2(savestd,STDIN_FILENO);
        read(STDIN_FILENO,userinput,1000);
        printf("You guessed = %s\n",userinput);
        if ((userinput[0] == text[0]))
        {
            printf("First digit is correct\n");
            firstcorrect = 1;
        }
        if ((userinput[1] == text[1]))
        {
            printf("Second digit is correct\n");
            secondcorrect = 1;
        }
        if ((userinput[2] == text[2]))
        {
            printf("Third digit is correct\n");
            thirdcorrect = 1;
        }
        if ((userinput[3] == text[3]))
        {
            printf("Fourth digit is correct\n");
            fourthcorrect = 1;
        }

        if ((firstcorrect == 1) && (secondcorrect == 1) && (thirdcorrect == 1) && (fourthcorrect == 1))
        {
            kill(*childPid,SIGKILL);
            printf("Congrats you entered all of it right!\n");
            wait(0);
            break;
        }

        

    }



}