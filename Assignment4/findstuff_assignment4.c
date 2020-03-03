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
    DIR *dir;
    char cwd[1000];
    int visit;
}directory;

int fd[10][2];

children *childrenList;
int *childrenCounter;
int *globalCount;

int *overwritePipe;

void fct()
{

    printf("\nChildren %i overwriting input with task %s\n",*overwritePipe, childrenList[*overwritePipe].task);
    dup2(fd[*overwritePipe][0],STDIN_FILENO);
    close(fd[*overwritePipe][0]);
    close(fd[*overwritePipe][1]);


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

void reaDir(DIR *dir, char *directTarget, char *homeDirect, char bufferOutput[2000],char inputBuffer[1000], int currentCounter);

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

    int *childIndex = (int*)mmap(NULL,sizeof(int)*10,PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1,0);
    for (int i =0; i < 10; i++)
        childIndex[i] = 0;

    childrenCounter = mmap(NULL,4,PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS,-1,0);

    globalCount = mmap(NULL,4,PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS,-1,0);

    directory *dirContainer = (directory*)mmap(NULL,sizeof(directory)*10000,PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    overwritePipe = mmap(NULL,4,PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS,-1,0);
    // childrenCounter = 0;
    // int *childrenCounter = mmap(NULL,4,PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS,-1,0);
    // *childrenCounter = 0;


    childrenList = (children*)mmap(NULL,sizeof(children)*10,PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    for (int i = 0; i<10; i++)
    {
        pipe(fd[i]);
    }
    
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
        // printf("\n");
        // printf("calc = %i\n",calc);
        fflush(0);
        dup2(save_stdin,STDIN_FILENO); //restore user input
        read(STDIN_FILENO,fileName,1000);


        printf("User Input = %s \n",fileName);
        // close(fd[0][0]);
        // close(fd[0][1]);
        

        // printf("stringlength file name = %d\n",strlen(fileName));
        // printf("Input2 %s = \n",fileName+strlen(fileName)+1);
        // for(int i =0; i < calc; i++)
        //     printf("'%c' ",fileName[i]);
        

     


        if (strncmp("list",fileName,4) == 0)
        {
            printf("------------------------------------------------------\n");
            printf("Listing!!! \n");
            // if (*childrenCounter == 0)
            // {
            //     printf("No children! \n");
            // }
            int noChild = 0;
            for (int i = 0; i < 10; i++)
            {
                if (childrenList[i].active == 1)
                {
                    printf("\n");
                    printf("Child %i:",i);
                    printf("            Task %s\n",childrenList[i].task);
                    printf("                    PID = %i\n",childrenList[i].pid);
                    printf("                    Active = %i\n", childrenList[i].active);
                    noChild = 1;
                }
            }
            printf("\n");
            if (noChild == 0)
            {
                printf("No children! They are all dead! \n");
            }
        }
    //HANDLE CASE KILL!
        if(strncmp("kill",fileName,4) == 0)
        {
            int childNumber = atoi(fileName + 5);
            printf("killing\n");
            printf("fileName number = %i\n",childNumber);
            if (childrenList[childNumber].active == 1)
            {
                printf("Killed child %i\n",childNumber);
                printf("Child %i PID = %i\n",childNumber,childrenList[childNumber].pid);
                childIndex[childNumber] = 0;
                childrenList[childNumber].active = 0;
                kill(childrenList[childNumber].pid,SIGKILL);
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
            srand(time(NULL));
            int lowerLimit = 5, upperLimit = 20;
            int times =  lowerLimit + rand() % (upperLimit - lowerLimit);   
            time_t T= time(NULL);      
            struct tm tm = *localtime(&T);
            int hr, min, sec, hr2, min2, sec2;
            hr = tm.tm_hour;
            min = tm.tm_min;
            sec = tm.tm_sec;
            // printf("hr = %i, min = %i, sec = %i\n", hr, min, sec);
            
            int forkYes = 0;
            // *globalCount = *childrenCounter;
            for (int i =0; i < 10; i++)
            {

                if (childIndex[i] == 0)
                {
                    currentCounter = i;
                    childIndex[i] = 1;
                    forkYes = 1;
                    i = 10;
                }
                    
            }
            printf("Time for child %i = %i\n",currentCounter,times);


            if (forkYes == 1)
            {
            // printf("About to fork and piping %i!\n",currentCounter);
            pipe(fd[currentCounter]);

            // printf("Outside fork fd[currentCounter][0] = %i \n",fd[*overwritePipe][0]);
            // printf("Outside fork fd[currentCounter][1] = %i \n",fd[*overwritePipe][1]); 
                if (fork() == 0)
                {
                    // printf("Childrencounter for child %i",)
                    char bufferOutput [2000];
                    // printf("Childrencounter = %d\n",currentCounter + 1);

                    // printf("forked just now \n");
                    char cwdf [1024];
                    char swdf [1024];
                    getcwd(cwdf,sizeof(cwdf));   
                    children child;
                    strcpy(child.task,fileName);
                    child.pid = getpid();
                    child.active = 1;
                    childrenList[currentCounter] = child;
                    // itoa(currentCounter,buffer,10);
                    char inputBuffer[1000];
                    strcpy(inputBuffer,fileName+5);
                    strtok(inputBuffer," ");
                    // printf("inputBuffer = %s\n",inputBuffer);

                    // sleep(5); //finding stuff

                    if (strstr(fileName,"-s"))
                    {
                        int found = 0;
                        // printf("finding with -s\n");

                        // while (1)
                        // {
                            getcwd(cwdf,sizeof(cwdf));
                            // printf("Current Directory = %s\n",cwdf);
                            dir = opendir(".");

                            
                            while ((dent = readdir(dir)) != NULL)
                            {
                                // printf("dent->d_name == %s\n",dent->d_name);
                                // printf("fileName+5 = %s\n",fileName+5);
                                int lenDent = strlen(dent->d_name);
                                int lenInput = strlen(inputBuffer);
                                int max = lenDent;
                                if (lenDent > lenInput)
                                    max = lenDent;
                                if (lenDent < lenInput)
                                    max = lenInput;
                            
                                stat(dent->d_name, &sb);
                        
                                if ((strncmp(dent->d_name, inputBuffer,max) == 0 ) && !(S_ISDIR(sb.st_mode)) )
                                {
                                    // printf("Found dent-> %s\n",dent->d_name);
                                    char childNumber[10];
                                    itoa(currentCounter,childNumber,10);
                                    strcat(bufferOutput,"\nChild ");
                                    strcat(bufferOutput,childNumber);
                                    strcat(bufferOutput," reporting: \n");
                                    strncat(bufferOutput,fileName+5,strlen(dent->d_name));
                                    strcat(bufferOutput," was found in directory ");
                                    strcat(bufferOutput,cwdf);
                                    strcat(bufferOutput,"\n");
                                    // printf("%s was found in directory %s\n",fileName, cwdf);
                                    found = 1;
                                }

                                if ((strcmp(dent->d_name, ".") != 0) && (strcmp(dent->d_name, "..") != 0) && S_ISDIR(sb.st_mode))
                                {
                                    // printf("Dent->d_name = %s\n",dent->d_name);
                                    reaDir(dir,dent->d_name, cwdf, bufferOutput,inputBuffer, currentCounter);
                                }
                                
                                
                            }     
                            close(dir);
                            getcwd(swdf,sizeof(swdf));
                            // printf("Sub directory = %s\n",swdf);

                            if (found == 0)
                            {
                                    char childNumber[10];
                                    itoa(currentCounter,childNumber,10);
                                    strcat(bufferOutput,"\nChild ");
                                    strcat(bufferOutput,childNumber);
                                    strcat(bufferOutput," reporting: \n");
                                    strcat(bufferOutput,"The file ");
                                    strcat(bufferOutput,fileName+5);
                                    strcat(bufferOutput,"was not found \n");
                            }


                        // printf("--------------------------------\n");
                        // printf("Buffer Output = %s\n",bufferOutput);
                    }
                        
                    // }

    


                    else
                    {
                        // printf("Finding!\n");
                        dir = opendir(".");
                        int found = 0;

                        while ((dent = readdir(dir)) != NULL)
                        {
                            // printf("dent->d_name == %s\n",dent->d_name);
                            // printf("fileName+5 = %s\n",fileName+5);
                            // printf("length of fileName + 5 = %d\n",strlen(fileName+5) - 1);
                            // printf("length of dent->dname = %d\n",strlen(dent->d_name));
                            // printf("Comparing d name to file name + 5 = %d\n",strncmp(dent->d_name, fileName+5,strlen(fileName+5) - 1));
                            int lenDent = strlen(dent->d_name);
                            int lenInput = strlen(fileName +5) - 1;
                            int max = lenDent;
                            if (lenDent > lenInput)
                                max = lenDent;
                            if (lenDent < lenInput)
                                max = lenInput;
                            
                            stat(dent->d_name, &sb);
                        
                            if ((strncmp(dent->d_name, fileName+5,max) == 0 ) && !(S_ISDIR(sb.st_mode)) )
                            {
                                char childNumber[10];
                                itoa(currentCounter,childNumber,10);
                                strcat(bufferOutput,"\nChild ");
                                strcat(bufferOutput,childNumber);
                                strcat(bufferOutput," reporting: \n");
                                strncat(bufferOutput,fileName+5,strlen(dent->d_name));
                                strcat(bufferOutput," was found in directory ");
                                strcat(bufferOutput,cwdf);
                                strcat(bufferOutput,"\n");
                                // printf("%s was found in directory %s\n",fileName, cwdf);
                                found = 1;
                            }
                            // printf("found = %i\n",found);
                            
                        }
                        if (found == 0)
                        {
                                char childNumber[10];
                                itoa(currentCounter,childNumber,10);
                                strcat(bufferOutput,"\nChild ");
                                strcat(bufferOutput,childNumber);
                                strcat(bufferOutput," reporting: \n");
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
                    // printf("Buffer Output = %s\n",bufferOutput);
                    // printf("fd %d\n",fd[currentCounter][1]);
                    // printf("Current counter = %i\n",currentCounter);
                    // printf("fd[currentCounter][0] = %i \n",fd[currentCounter][0]);
                    // printf("fd[currentCounter][1] = %i \n",fd[currentCounter][1]);
                    sleep(times);
                    T = time(NULL);      
                    tm = *localtime(&T);
                    hr2 = tm.tm_hour;
                    min2 = tm.tm_min;
                    sec2 = tm.tm_sec;
                    // printf("line 454 hr2 = %i, min2 = %i, sec2 = %i\n", tm.tm_hour, tm.tm_min, tm.tm_sec);
                    // printf("455?");
                    char hrCharResult[10];
                    char minCharResult[10];
                    char secCharResult[10];
                    int hrResult = hr2 - hr;
                    int minResult = min2 - min;
                    int secResult = sec2 - sec;
                    if (hrResult < 0)
                        hrResult = hr2 + 60 - hr;
                    if (minResult < 0)
                        minResult = min2 + 60 - min;
                    if (secResult < 0)
                        secResult = sec2 + 60 - sec;
                    if (minResult > 0 && (sec2 < sec))
                        minResult = minResult - 1;
                    if (hrResult > 0 && (min2 < min))
                        hrResult = hrResult - 1;
                    

                    // itoa(hrResult,hrCharResult,10);
                    // itoa(minResult,minCharResult,10);
                    // itoa(secResult,secCharResult,10);
                    // printf("464");
                    // char timeTotal [100];
                    // strcat(timeTotal,"That took a total of ");
                    // printf("464");
                    // strcat(timeTotal,hrCharResult);
                    // strcat(timeTotal,"::");
                    // printf("464");
                    // strcat(timeTotal,minCharResult);
                    // strcat(timeTotal,"::");
                    // strcat(timeTotal,secCharResult);
                    // strcat(timeTotal,"\n");                   
                    // strcat(bufferOutput,timeTotal);
                    // printf("473 here! ");
                    // printf("timeTotal = %s\n",timeTotal);

                    // printf("Child %i bufferOutput = %s\n",currentCounter,bufferOutput);


                    write(fd[currentCounter][1],bufferOutput,1024);
                    // close(fd[0][0]);
                    // close(fd[0][1]);
                    close(fd[currentCounter][0]);
                    close(fd[currentCounter][1]);

                    *overwritePipe = currentCounter;

                    kill(parentPid,SIGUSR1);
                    printf("\nTotal time it took for child%i= %i hour::%i min::%i sec\n",currentCounter,hrResult,minResult,secResult);

                    // close(fd[currentCounter][1]);
                    childrenList[currentCounter].active = 0; 
                    
                    childIndex[currentCounter] = 0;

                    printf("\n");
                    return 9;
                }   

                // printf("after child process %i\n",*childrenCounter);
            }
    

        }
    }


}

void reaDir(DIR *dir, char *directTarget, char *homeDirect, char bufferOutput[2000], char inputBuffer[1000], int currentCounter)
{
    // printf("going inside reaDir \n");
    struct stat sb;
    struct dirent *dent;
    char cwdf[1024];

    
    chdir(directTarget);
    getcwd(cwdf,sizeof(cwdf));

    // printf("directTarget = %s\n",directTarget);
    // getcwd(cwdf,sizeof(cwdf));
    // printf("Current Directory recursive = %s\n",cwdf);
    // chdir(homeDirect);
    

        int found = 0;

    
        dir = opendir(".");

        
        while ((dent = readdir(dir)) != NULL)
        {
            // printf("dent->d_name == %s\n",dent->d_name);
            // printf("fileName+5 = %s\n",fileName+5);
            int lenDent = strlen(dent->d_name);
            int lenInput = strlen(inputBuffer);
            int max = lenDent;
            if (lenDent > lenInput)
                max = lenDent;
            if (lenDent < lenInput)
                max = lenInput;
        
            stat(dent->d_name, &sb);
    
            if ((strncmp(dent->d_name, inputBuffer,max) == 0 ) && !(S_ISDIR(sb.st_mode)) )
            {
                // printf("Inside readir: found dent-> %s\n",dent->d_name);
                char childNumber[10];
                itoa(currentCounter,childNumber,10);
                strcat(bufferOutput,"\nChild ");
                strcat(bufferOutput,childNumber);
                strcat(bufferOutput," reporting: \n");
                strncat(bufferOutput,inputBuffer,strlen(dent->d_name));
                strcat(bufferOutput," was found in directory ");
                strcat(bufferOutput,cwdf);
                strcat(bufferOutput,"\n");
                // printf("Inside readir: %s was found in directory %s\n",dent->d_name, cwdf);
                found = 1;
            }

            if ((strcmp(dent->d_name, ".") != 0) && (strcmp(dent->d_name, "..") != 0) && S_ISDIR(sb.st_mode))
            {
                // printf("Dent->d_name = %s\n",dent->d_name);
                reaDir(dir,dent->d_name, cwdf, bufferOutput,inputBuffer, currentCounter);
            }
            
            
        }     
        chdir(homeDirect);
        close(dir);
    

}

