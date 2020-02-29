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

typedef struct children
{
    char task[1000];
    int active;
    int pid;
    
}children;

int fd[10][2];
children *childrenList;
int *childrenCounter;

void fct()
{
    int currentActual;
    currentActual = *childrenCounter;
    printf("\nChildren %i overwriting input with task %s\n",currentActual, childrenList[currentActual - 1].task);
    dup2(fd[currentActual - 1][0],STDIN_FILENO);
}

void strreverse(char* begin, char* end) {
	
	char aux;
	
	while(end>begin)
	
		aux=*end, *end--=*begin, *begin++=aux;
	
}

void itoa(int value, char* str, int base) {
	
	static char num[] = "0123456789abcdefghijklmnopqrstuvwxyz";
	
	char* wstr=str;
	
	int sign;
	


	
	// Validate base
	
	if (base<2 || base>35){ *wstr='\0'; return; }
	

	
	// Take care of sign
	
	if ((sign=value) < 0) value = -value;
	


	
	// Conversion. Number is reversed.
	
	do *wstr++ = num[value%base]; while(value/=base);
	
	if(sign<0) *wstr++='-';
	
	*wstr='\0';
	

	
	// Reverse string

	
	strreverse(str,wstr-1);
	
}


int main()
{



    int f1,g,status;
    DIR *dir;
    struct stat sb;
    struct dirent *dent;
    char fileName[1000];
    int parentPid = getpid();
    int childpid;

    childrenCounter = mmap(NULL,4,PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS,-1,0);

    // childrenCounter = 0;
    // int *childrenCounter = mmap(NULL,4,PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS,-1,0);
    // *childrenCounter = 0;


    childrenList = (children*)mmap(NULL,sizeof(children)*10,PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    for (int i = 0; i<10; i++)
        pipe(fd[i]);
    int save_stdin = dup(STDIN_FILENO);

    signal(SIGUSR1,fct);


    
    printf("Parent PID = %d\n",parentPid);

    while (1)
    {
        char cwd [1024];
        getcwd(cwd,sizeof(cwd));    
        printf(KBLU);
        printf("findstuff %s", cwd);
        printf(RESET);
        printf("$ ");
        memset(fileName, 0, 1000);

        // scanf("%s",fileName);
        read(STDIN_FILENO,fileName,1000);
        printf("User Input = %s \n",fileName);

        dup2(save_stdin,STDIN_FILENO); //restore user input

     


        if (strncmp("list",fileName,4) == 0)
        {
            printf("------------------------------------------------------\n");
            printf("Listing!!! \n");
            if (*childrenCounter == 0)
            {
                printf("No children! \n");
            }

            for (int i = 0; i < *childrenCounter; i++)
            {
                printf("\n");
                printf("Child %i:",i + 1);
                printf("            Task %s\n",childrenList[i].task);
                printf("                    PID = %i\n",childrenList[i].pid);
                printf("                    Active = %i\n", childrenList[i].active);
            }
            printf("\n");

        }

        if(strncmp("kill",fileName,4) == 0)
        {
            int childNumber = atoi(fileName + 5);
            printf("killing\n");
            printf("fileName number = %i\n",childNumber);
            if (childrenList[childNumber - 1].active == 1)
            {
                printf("Killed child %i\n",childNumber);
                printf("Child %i PID = %i\n",childNumber,childrenList[childNumber - 1].pid);
                childrenList[childNumber - 1].active = 0;
                kill(childrenList[childNumber - 1].pid,SIGKILL);
            }
            else
            {
                printf("Child Isn't Active or non-existing. \n");
            }
        }

        if (strncmp("quit",fileName,4) == 0)
        {
            printf("Quitting!\n");
            for (int i =0; i < *childrenCounter; i++)
            {
                kill(childrenList[i].pid, SIGKILL);
            }
            break;



        }



        if(strncmp("find",fileName,4) == 0)
        {
            int currentCounter;
            currentCounter = *childrenCounter;
            *childrenCounter = *childrenCounter + 1;
            printf("Parent PID = %d\n",getpid());
            if (fork() == 0)
            {
                char buffer [20];
                printf("Childrencounter = %d",currentCounter + 1);

                
                char cwdf [1024];
                getcwd(cwd,sizeof(cwdf));   
                children child;
                strcpy(child.task,fileName);
                child.pid = getpid();
                child.active = 1;
                childrenList[currentCounter] = child;
                itoa(currentCounter,buffer,10);


                sleep(5); //finding stuff
                
                write(fd[currentCounter][1],buffer,1024);
                kill(parentPid,SIGUSR1);
                childrenList[currentCounter].active = 0; 
                


                printf("\n");
                return 9;
            }   

            printf("after child process %i\n",*childrenCounter);
            

        }
    }



}
