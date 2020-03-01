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

typedef struct directory
{
    char cwd[1000];
    int visit;
}directory;

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

void findFile(char *d_name, char*fileName, char *buffer);

int main()
{

    char test1[10] = {'t','r','a',0,'n','h','a','a',0};

    printf("test1 = %c",(test1[1]));

    int f1,g,status;
    DIR *dir;
    struct stat sb;
    struct dirent *dent;
    char fileName[1000];
    int parentPid = getpid();
    int childpid;

    childrenCounter = mmap(NULL,4,PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS,-1,0);

    directory *dirContainer = (directory*)mmap(NULL,sizeof(directory)*10000,PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

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
        // int calc = read(STDIN_FILENO,fileName,10000);
        // printf("calc = %i\n",calc);
        read(STDIN_FILENO,fileName,1000);
        printf("User Input = %s \n",fileName);
        // printf("stringlength file name = %d\n",strlen(fileName));
        // printf("Input2 %s = \n",fileName+strlen(fileName)+1);
        // for(int i =0; i < calc; i++)
        //     printf("'%c' ",fileName[i]);
        
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
    //HANDLE CASE KILL!
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
            if (*childrenCounter < 11)
            {
                if (fork() == 0)
                {
                    char bufferOutput [2000];
                    printf("Childrencounter = %d\n",currentCounter + 1);

                    
                    char cwdf [1024];
                    char swdf [1024];
                    getcwd(cwdf,sizeof(cwdf));   
                    children child;
                    strcpy(child.task,fileName);
                    child.pid = getpid();
                    child.active = 1;
                    childrenList[currentCounter] = child;
                    // itoa(currentCounter,buffer,10);

                    int firstDirectory = 1;
                    int dirElement = 0;

                    // sleep(5); //finding stuff
                    if (strstr(fileName,"-s"))
                    {
                        while (1)
                        {
                            getcwd(cwdf,sizeof(cwdf));
                            printf("Current Directory = %s\n",cwdf);
                            chdir("..");
                            getcwd(swdf,sizeof(swdf));
                            printf("Sub directory = %s\n",swdf);
                            int found = 0;

                            while ((dent = readdir(dir)) != NULL)
                            {
                                if (strncmp(dent->d_name, fileName+5,strlen(dent->d_name)) == 0)
                                {
                                    char childNumber[10];
                                    itoa(*childrenCounter,childNumber,10);
                                    strcat(bufferOutput,"\nChild ");
                                    strcat(bufferOutput,childNumber);
                                    strcat(bufferOutput," reporting!!!! \n");
                                    strncat(bufferOutput,fileName+5,strlen(dent->d_name));
                                    strcat(bufferOutput," was found in directory ");
                                    strcat(bufferOutput,cwdf);
                                    strcat(bufferOutput,"\n");
                                    printf("%s was found in directory %s\n",fileName, cwdf);
                                    found = 1;
                                }
                                // dirContainer[dirElement] = bufferOutput;
                                // dirElement++;
                                // printf("found = %i\n",found);
                                
                            }     

                            if (found == 0)
                            {
                                    char childNumber[10];
                                    itoa(*childrenCounter,childNumber,10);
                                    strcat(bufferOutput,"\nChild ");
                                    strcat(bufferOutput,childNumber);
                                    strcat(bufferOutput," reporting!!!! \n");
                                    strcat(bufferOutput,"The file ");
                                    strcat(bufferOutput,fileName+5);
                                    strcat(bufferOutput,"was not found \n");

                            }

                            // dir = open(".");

                            // //IF IT'S A DIRECTORY THEN NAVIGATE INTO IT!
                            //     while ((dent = readdir(dir)) != NULL)
                            //     {

                            //         printf("Navigating through %s",dent->d_name);
                            //         stat(dent->d_name, &sb);
                            //         if (S_ISDIR(sb.st_mode))
                            //         {
                            //             printf(" is directory ---- ");
                            //         }
                            //         printf("\n");
                            //     }
                            // close(dir);
                            


                            if (strcmp(swdf,cwdf) == 0)
                            {
                                break;
                            }
                            sleep(1);
                        }
                    }


                    else
                    {
                        printf("Finding!\n");
                        dir = opendir(".");
                        int found = 0;

                        while ((dent = readdir(dir)) != NULL)
                        {
                            // printf("dent->d_name == %s\n",dent->d_name);
                            // printf("fileName+5 = %s\n",fileName+5);
                            if (strncmp(dent->d_name, fileName+5,strlen(dent->d_name)) == 0)
                            {
                                char childNumber[10];
                                itoa(*childrenCounter,childNumber,10);
                                strcat(bufferOutput,"\nChild ");
                                strcat(bufferOutput,childNumber);
                                strcat(bufferOutput," reporting!!!! \n");
                                strncat(bufferOutput,fileName+5,strlen(dent->d_name));
                                strcat(bufferOutput," was found in directory ");
                                strcat(bufferOutput,cwdf);
                                strcat(bufferOutput,"\n");
                                printf("%s was found in directory %s\n",fileName, cwdf);
                                found = 1;
                            }
                            // printf("found = %i\n",found);
                            
                        }
                        if (found == 0)
                        {
                                char childNumber[10];
                                itoa(*childrenCounter,childNumber,10);
                                strcat(bufferOutput,"\nChild ");
                                strcat(bufferOutput,childNumber);
                                strcat(bufferOutput," reporting!!!! \n");
                                strcat(bufferOutput,"The file ");
                                strcat(bufferOutput,fileName+5);
                                strcat(bufferOutput,"was not found \n");

                        }
                        close(dir);
                    }
                    
                    // if (stat(fileName, &sb) == -1)
                    // {
                    //     *active = 1;
                    //     printf("File does not exist!!! \n");
                    // }
                    int a = 0;
                    printf("Buffer Output = %s\n",bufferOutput);
                    printf("fd %d\n",fd[currentCounter][1]);
                    write(fd[currentCounter][1],bufferOutput,1024);
                    write(fd[currentCounter][1],"ALIBABA",1024);
                    
                    kill(parentPid,SIGUSR1);
                    childrenList[currentCounter].active = 0; 
                    
                    sleep(50);

                    printf("\n");
                    return 9;
                }   

                printf("after child process %i\n",*childrenCounter);
            }
            else
            {
                printf("Exceeding limit!!!! \n");
            }
            

        }
    }


}
