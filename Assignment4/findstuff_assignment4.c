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
int get_argument(char* line, int argn, char* result);

void reaDir(DIR *dir, char *directTarget, char *homeDirect, char bufferOutput[20000],char inputBuffer[1000], int currentCounter);
void dirText(DIR *dir, char *directTarget, char*homeDirect, char bufferOutput[20000], char bufferNoQuotation[1000], int currentCounter);
int dirTextSpecific(DIR *dir, char *directTarget, char*homeDirect, char bufferOutput[20000], char bufferNoQuotation[1000], int currentCounter, char *specific);

int main()
{

    char test1[10] = {'t','r','a',0,'n','h','a','a',0};


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




    
    // printf("Parent PID = %d\n",parentPid);

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
            char quotation[1] = {'"'};

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
                    char bufferOutput [20000];
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
                    char bufferNoQuotation[1000];
                    char inputBuffer[1000];
                    char inputBufferQuotation[1000];
                    char thirdBuffer[1000];
                    memset(bufferNoQuotation, 0, 1000);
                    memset(inputBufferQuotation, 0, 1000);
                    memset(thirdBuffer, 0, 1000);
                    int success = get_argument(fileName,1,inputBufferQuotation);
                    int success2 = get_argument(fileName,2,thirdBuffer);
                    // printf("thirdbuffer = %s size %i\n",thirdBuffer+3, strlen(thirdBuffer+3));
                    // printf("inputBufferQuotation = %s\n",inputBufferQuotation);
                    // printf("strlen(inputBufferQuotation)-2 = %i\n",strlen(inputBufferQuotation)-2);
                    if (strstr(fileName,"-s"))
                    {
                        
                        strncpy(bufferNoQuotation,inputBufferQuotation+1,strlen(inputBufferQuotation)-2);
                    }
                    else //this is this a test
                    {
                        if (strstr(fileName,"-f:"))
                            strncpy(bufferNoQuotation,inputBufferQuotation+1,strlen(inputBufferQuotation)-2);
                        else
                            strncpy(bufferNoQuotation,inputBufferQuotation+1,strlen(inputBufferQuotation)-3);


                    }
                    // printf("inputBufferQuotation = %s %i\n",bufferNoQuotation,strlen(bufferNoQuotation));
                    strcpy(inputBuffer,fileName+5);
                    strtok(inputBuffer," ");
                    // printf("inputBuffer = %s\n",inputBuffer);

                    // sleep(5); //finding stuff



                    //CASE HANDLING TEXT!
                    if ((strncmp(fileName+5,quotation,1) == 0))
                    {
                        // printf("okay finding text\n");

                        //FINDING TEXT WITH -s
// i dont know but it is rare huh// to test!
                        if (strstr(fileName,"-s"))
                        {
                            // printf("Finding Text With subdirectories!\n");
                            int found = 0;
                            int foundAtAll = 0;
                            int onlyFileActivate = 0;
                            char *onlyFile;
                            char line[1024];
                            // memset(line,0,1024);
                            FILE *file;
                            // chdir("..");
                            getcwd(cwdf,sizeof(cwdf));
                            dir = opendir(".");
                            // printf("cwdf = %s",cwdf);
                            //ONLY SEARCH FOR THESE FILES
                            if (strncmp(thirdBuffer,"-f",2) == 0)
                            {
                                // printf("activate onlyfile\n");
                                char *onlyFile = thirdBuffer + 3;
                                onlyFileActivate = 1;
                            }


                            //SEARCHING FOR ALL TYPE OF FILE! SINCE ONLYFILE ACTIVATE IS 0
                            if (onlyFileActivate == 0)
                            {
                                while ((dent = readdir(dir)) != NULL)
                                {
                                    found = 0;
                                    stat(dent->d_name, &sb);
                                    if (S_ISREG(sb.st_mode) && (strcmp(dent->d_name,".DS_Store") != 0))
                                    {
                                        file = fopen(dent->d_name,"r+");
                                        if (file == NULL)
                                        {
                                            // printf("Not valid file\n");
                                            break;
                                        }
                                        while (fgets(line,1024,file))
                                        {
                                            // printf("file %s has %s\n",dent->d_name,line);
                                            // printf("strstr %s \n",strstr(line,bufferNoQuotation));
                                            for (char *word = strtok (line, " "); word; word = strtok (NULL, " "))
                                            {
                                                // printf("word = %s and size = %i\n",word, strlen(word));
                                                if (strcmp(word,bufferNoQuotation) == 0)
                                                {
                                                    // printf("dent->d_name = %s\n",dent->d_name);
                                                    found = 1;
                                                    foundAtAll =1;
                                                    break;
                                                }
                                            }
                                            
                                        }
                                        if (found == 1)
                                        {
                                            char childNumber[10];
                                            itoa(currentCounter,childNumber,10);
                                            strcat(bufferOutput,"\nChild ");
                                            strcat(bufferOutput,childNumber);
                                            strcat(bufferOutput," reporting: \n");
                                            strcat(bufferOutput,"The text '");
                                            strcat(bufferOutput,bufferNoQuotation);
                                            strcat(bufferOutput,"' was found in the file ~");
                                            strcat(bufferOutput,(dent->d_name));
                                            strcat(bufferOutput,"~ and was found in directory ");
                                            strcat(bufferOutput,cwdf);
                                            strcat(bufferOutput,"\n");
                                        }
                                    }

                                    if ((strcmp(dent->d_name, ".") != 0) && (strcmp(dent->d_name, "..") != 0) && S_ISDIR(sb.st_mode))
                                    {
                                        dirText(dir,dent->d_name,cwdf,bufferOutput,bufferNoQuotation,currentCounter);

                                    }

                                }
                            }

                            //SEARCHING WITH ONLY SPECIFIC FILE TYPE! with subdirectories
                            else
                            {
                                printf("Searching with only SPECIFIC file type\n");
                                while ((dent = readdir(dir)) != NULL)
                                {
                                    found = 0;
                                    stat(dent->d_name, &sb);
                                    // printf("dent->d_name = %s\n",dent->d_name);
                                    char typeOfFile[100];
                                    char fileEnding[100];
                                    memset(typeOfFile,0,100);
                                    memset(fileEnding,0,100);
                                    // printf("strlen(thirdbuffer+3) = %i",strlen(thirdBuffer+3));
                                    strncpy(typeOfFile,thirdBuffer+3,strlen(thirdBuffer+3));
                                    strcpy(fileEnding,dent->d_name + strlen(dent->d_name) - strlen(typeOfFile));



                                    if (S_ISREG(sb.st_mode) && (strcmp(dent->d_name,".DS_Store") != 0) && (strstr(typeOfFile,fileEnding)))
                                    {
                                        // printf("Inside = dent->d_name = %s\n",dent->d_name);
                                        file = fopen(dent->d_name,"r+");
                                        if (file == NULL)
                                        {
                                            // printf("Not valid file\n");
                                            break;
                                        }
                                        while (fgets(line,1024,file))
                                        {
                                            // printf("file %s has %s\n",dent->d_name,line);
                                            // printf("strstr %s \n",strstr(line,bufferNoQuotation));
                                            for (char *word = strtok (line, " "); word; word = strtok (NULL, " "))
                                            {
                                                // printf("word = %s and size = %i\n",word, strlen(word));
                                                if (strcmp(word,bufferNoQuotation) == 0)
                                                {
                                                    // printf("dent->d_name = %s\n",dent->d_name);
                                                    found = 1;
                                                    foundAtAll = 1;
                                                    break;
                                                }
                                            }
                                            
                                        }
                                        if (found == 1)
                                        {
                                            char childNumber[10];
                                            itoa(currentCounter,childNumber,10);
                                            strcat(bufferOutput,"\nChild ");
                                            strcat(bufferOutput,childNumber);
                                            strcat(bufferOutput," reporting: \n");
                                            strcat(bufferOutput,"The text '");
                                            strcat(bufferOutput,bufferNoQuotation);
                                            strcat(bufferOutput,"' was found in the file ~");
                                            strcat(bufferOutput,(dent->d_name));
                                            strcat(bufferOutput,"~ and was found in directory ");
                                            strcat(bufferOutput,cwdf);
                                            strcat(bufferOutput,"\n");
                                        }
                                    }

                                    if ((strcmp(dent->d_name, ".") != 0) && (strcmp(dent->d_name, "..") != 0) && S_ISDIR(sb.st_mode))
                                    {
                                        foundAtAll = dirTextSpecific(dir,dent->d_name,cwdf,bufferOutput,bufferNoQuotation,currentCounter,thirdBuffer+3);
                                        printf("foundAtAll = %i in directory %s\n",foundAtAll,dent->d_name);
                                    }

                                }
                            }

                            
                            close(dir);
                            if (foundAtAll == 0)
                            {
                                    char childNumber[10];
                                    itoa(currentCounter,childNumber,10);
                                    strcat(bufferOutput,"\nChild ");
                                    strcat(bufferOutput,childNumber);
                                    strcat(bufferOutput," reporting: \n");
                                    strcat(bufferOutput,"The text '");
                                    strcat(bufferOutput,bufferNoQuotation);
                                    strcat(bufferOutput,"' was not found \n");
                            }
                            
                        }

                        //FINDING TEXT WITHOUT -s
                        else
                        {
                            // printf("Finding Text Without subdirectories!\n");
                            int found = 0;
                            int foundAtAll = 0;
                            int onlyFileActivate = 0;
                            char *onlyFile;
                            char line[1024];
                            // memset(line,0,1024);
                            FILE *file;
                            // chdir("here");
                            getcwd(cwdf,sizeof(cwdf));
                            dir = opendir(".");
                            // printf("cwdf = %s",cwdf);
                            //ONLY SEARCH FOR THESE FILES
                            if (strncmp(thirdBuffer,"-f",2) == 0)
                            {
                                // printf("activate onlyfile\n");
                                char *onlyFile = thirdBuffer + 3;
                                onlyFileActivate = 1;
                            }


                            //SEARCHING FOR ALL TYPE OF FILE! SINCE ONLYFILE ACTIVATE IS 0
                            if (onlyFileActivate == 0)
                            {
                                while ((dent = readdir(dir)) != NULL)
                                {
                                    found = 0;
                                    stat(dent->d_name, &sb);
                                    if (S_ISREG(sb.st_mode) && (strcmp(dent->d_name,".DS_Store") != 0))
                                    {
                                        file = fopen(dent->d_name,"r+");
                                        if (file == NULL)
                                        {
                                            // printf("Not valid file\n");
                                            break;
                                        }
                                        while (fgets(line,1024,file))
                                        {
                                            // printf("file %s has %s\n",dent->d_name,line);
                                            // printf("strstr %s \n",strstr(line,bufferNoQuotation));
                                            for (char *word = strtok (line, " "); word; word = strtok (NULL, " "))
                                            {
                                                // printf("word = %s and size = %i\n",word, strlen(word));
                                                if (strcmp(word,bufferNoQuotation) == 0)
                                                {
                                                    // printf("dent->d_name = %s\n",dent->d_name);
                                                    found = 1;
                                                    foundAtAll = 1;
                                                    break;
                                                }
                                            }
                                            
                                        }
                                        if (found == 1)
                                        {
                                            char childNumber[10];
                                            itoa(currentCounter,childNumber,10);
                                            strcat(bufferOutput,"\nChild ");
                                            strcat(bufferOutput,childNumber);
                                            strcat(bufferOutput," reporting: \n");
                                            strcat(bufferOutput,"The text '");
                                            strcat(bufferOutput,bufferNoQuotation);
                                            strcat(bufferOutput,"' was found in the file ~");
                                            strcat(bufferOutput,(dent->d_name));
                                            strcat(bufferOutput,"~ and was found in directory ");
                                            strcat(bufferOutput,cwdf);
                                            strcat(bufferOutput,"\n");
                                        }
                                    }
                                }
                            }

                            //SEARCHING WITH ONLY SPECIFIC FILE TYPE! with no sub directores
                            else
                            {
                                printf("Searching with only SPECIFIC file type\n");
                                while ((dent = readdir(dir)) != NULL)
                                {
                                    found = 0;
                                    stat(dent->d_name, &sb);
                                    // printf("dent->d_name = %s\n",dent->d_name);
                                    char typeOfFile[100];
                                    memset(typeOfFile,0,100);
                                    // printf("strlen(thirdbuffer+3) = %i",strlen(thirdBuffer+3));
                                    strncpy(typeOfFile,thirdBuffer+3,strlen(thirdBuffer+3)-1);
                                    printf("type of file = %s with size %i",typeOfFile,strlen(typeOfFile));


                                    if (S_ISREG(sb.st_mode) && (strcmp(dent->d_name,".DS_Store") != 0) && strstr(dent->d_name,typeOfFile))
                                    {
                                        // printf("Inside = dent->d_name = %s\n",dent->d_name);
                                        file = fopen(dent->d_name,"r+");
                                        if (file == NULL)
                                        {
                                            // printf("Not valid file\n");
                                            break;
                                        }
                                        while (fgets(line,1024,file))
                                        {
                                            // printf("file %s has %s\n",dent->d_name,line);
                                            // printf("strstr %s \n",strstr(line,bufferNoQuotation));
                                            for (char *word = strtok (line, " "); word; word = strtok (NULL, " "))
                                            {
                                                // printf("word = %s and size = %i\n",word, strlen(word));
                                                if (strcmp(word,bufferNoQuotation) == 0)
                                                {
                                                    // printf("found the word dent->d_name = %s\n",dent->d_name);
                                                    found = 1;
                                                    foundAtAll = 1;
                                                    break;
                                                }
                                            }
                                            
                                        }
                                        if (found == 1)
                                        {
                                            char childNumber[10];
                                            itoa(currentCounter,childNumber,10);
                                            strcat(bufferOutput,"\nChild ");
                                            strcat(bufferOutput,childNumber);
                                            strcat(bufferOutput," reporting: \n");
                                            strcat(bufferOutput,"The text '");
                                            strcat(bufferOutput,bufferNoQuotation);
                                            strcat(bufferOutput,"' was found in the file ~");
                                            strcat(bufferOutput,(dent->d_name));
                                            strcat(bufferOutput,"~ and was found in directory ");
                                            strcat(bufferOutput,cwdf);
                                            strcat(bufferOutput,"\n");
                                        }
                                    }

                        

                                }
                            }

                            
                            close(dir);//
                            if (foundAtAll == 0)
                            {
                                    char childNumber[10];
                                    itoa(currentCounter,childNumber,10);
                                    strcat(bufferOutput,"\nChild ");
                                    strcat(bufferOutput,childNumber);
                                    strcat(bufferOutput," reporting: \n");
                                    strcat(bufferOutput,"The text '");
                                    strcat(bufferOutput,bufferNoQuotation);
                                    strcat(bufferOutput,"' was not found \n");
                            }
                        }
                    }




//HANDLING WITHOUT FINDING TEXT!!! JUST FILEEEEEEEEEEEEEEEEEEEEEE
                    else
                    {
                        // printf("okay no text\n");

///////////////////////////////////FINDING FILE WITH -s
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

        

////////////////////////FINDING FILE WITHOUT -s
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
                    // sleep(times);
                    T = time(NULL);      
                    tm = *localtime(&T);
                    hr2 = tm.tm_hour;
                    min2 = tm.tm_min;
                    sec2 = tm.tm_sec;
                    // printf("line 454 hr2 = %i, min2 = %i, sec2 = %i\n", tm.tm_hour, tm.tm_min, tm.tm_sec);
                    // printf("455?");clea
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


                    write(fd[currentCounter][1],bufferOutput,20000);
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

void reaDir(DIR *dir, char *directTarget, char *homeDirect, char bufferOutput[20000], char inputBuffer[1000], int currentCounter)
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

int dirTextSpecific(DIR *dir, char *directTarget, char*homeDirect, char bufferOutput[20000], char bufferNoQuotation[1000], int currentCounter, char *specific)
{
    struct stat sb;
    struct dirent *dent;
    char cwdf[1024];
    FILE *file;
    char line[1024];
    chdir(directTarget);
    getcwd(cwdf,sizeof(cwdf));
    int foundAtAll;
    int found;
    dir = opendir(".");


    while ((dent = readdir(dir)) != NULL)
    {
        found = 0;
        stat(dent->d_name, &sb);
            char fileEnding[100];
            memset(fileEnding,0,100);
            // printf("strlen(thirdbuffer+3) = %i",strlen(thirdBuffer+3));
            strcpy(fileEnding,dent->d_name + strlen(dent->d_name) - strlen(specific));
        if (S_ISREG(sb.st_mode) && (strcmp(dent->d_name,".DS_Store") != 0) && strstr(fileEnding,specific))
        {
            file = fopen(dent->d_name,"r+");
            if (file == NULL)
            {
                // printf("Not valid file\n");
                break;
            }
            while (fgets(line,1024,file))
            {
                // printf("file %s has %s\n",dent->d_name,line);
                // printf("strstr %s \n",strstr(line,bufferNoQuotation));
                for (char *word = strtok (line, " "); word; word = strtok (NULL, " "))
                {
                    // printf("word = %s",word);
                    if (strcmp(word,bufferNoQuotation) == 0)
                    {
                        // printf("dent->d_name = %s\n",dent->d_name);
                        found = 1;
                        foundAtAll = 1;
                        break;
                    }
                }
                
            }
            if (found == 1)
            {
                char childNumber[10];
                itoa(currentCounter,childNumber,10);
                strcat(bufferOutput,"\nChild ");
                strcat(bufferOutput,childNumber);
                strcat(bufferOutput," reporting: \n");
                strcat(bufferOutput,"The text '");
                strcat(bufferOutput,bufferNoQuotation);
                strcat(bufferOutput,"' was found in the file ~");
                strcat(bufferOutput,(dent->d_name));
                strcat(bufferOutput,"~ and was found in directory ");
                strcat(bufferOutput,cwdf);
                strcat(bufferOutput,"\n");
            }
        }

        if ((strcmp(dent->d_name, ".") != 0) && (strcmp(dent->d_name, "..") != 0) && S_ISDIR(sb.st_mode))
        {
            dirTextSpecific(dir,dent->d_name,cwdf,bufferOutput,bufferNoQuotation,currentCounter,specific);

        }
    }
    chdir(homeDirect);
    close(dir);
    return foundAtAll;
    
}

void dirText(DIR *dir, char *directTarget, char*homeDirect, char bufferOutput[20000], char bufferNoQuotation[1000], int currentCounter)
{

    struct stat sb;
    struct dirent *dent;
    char cwdf[1024];
    FILE *file;
    char line[1024];
    chdir(directTarget);
    getcwd(cwdf,sizeof(cwdf));

    int found;
    dir = opendir(".");


    while ((dent = readdir(dir)) != NULL)
    {
        found = 0;
        stat(dent->d_name, &sb);
        if (S_ISREG(sb.st_mode) && (strcmp(dent->d_name,".DS_Store") != 0))
        {
            file = fopen(dent->d_name,"r+");
            if (file == NULL)
            {
                // printf("Not valid file\n");
                break;
            }
            while (fgets(line,1024,file))
            {
                // printf("file %s has %s\n",dent->d_name,line);
                // printf("strstr %s \n",strstr(line,bufferNoQuotation));
                for (char *word = strtok (line, " "); word; word = strtok (NULL, " "))
                {
                    // printf("word = %s",word);
                    if (strcmp(word,bufferNoQuotation) == 0)
                    {
                        // printf("dent->d_name = %s\n",dent->d_name);
                        found = 1;
                        break;
                    }
                }
                
            }
            if (found == 1)
            {
                char childNumber[10];
                itoa(currentCounter,childNumber,10);
                strcat(bufferOutput,"\nChild ");
                strcat(bufferOutput,childNumber);
                strcat(bufferOutput," reporting: \n");
                strcat(bufferOutput,"The text '");
                strcat(bufferOutput,bufferNoQuotation);
                strcat(bufferOutput,"' was found in the file ~");
                strcat(bufferOutput,(dent->d_name));
                strcat(bufferOutput,"~ and was found in directory ");
                strcat(bufferOutput,cwdf);
                strcat(bufferOutput,"\n");
            }
        }

        if ((strcmp(dent->d_name, ".") != 0) && (strcmp(dent->d_name, "..") != 0) && S_ISDIR(sb.st_mode))
        {
            dirText(dir,dent->d_name,cwdf,bufferOutput,bufferNoQuotation,currentCounter);

        }
    }
    chdir(homeDirect);
    close(dir);


}


int get_argument(char* line, int argn, char* result)
	{
	//firstly: remove all spaces at the front
	char temp[1000];
	int start_space = 1;
	for (int i = 0, u = 0; i <= strlen(line); i++)
		if (line[i] == ' ' && start_space == 1) continue;
		else 
			{
			temp[u++] = line[i]; 
			start_space = 0;
			}
	//now remove an double or tripple spaces
	char temp2[1000];
	int space_on = 0;
	for (int i = 0, u = 0; i <= strlen(temp); i++)
		{
		if (space_on == 1 && temp[i] == ' ') continue;
		temp2[u++] = temp[i];
		if (temp[i] == ' ') space_on = 1;
		else space_on = 0;
		}
	//finally extract the arguments
	int start, end;
	start = end = 0;
	int count = 0;
	int quote = 0;
	for (int i = 0; i <= strlen(temp2); i++)
		if (temp2[i] == '\"') quote = !quote;
		else if (quote == 0 &&(temp2[i] == ' ' || temp2[i] == 0))
			{
			end = i;
			if (argn == count)
				{
				int length = end - start;
				strncpy(result, temp2 + start, length);
				result[length] = 0;
				return 1;
				}
			start = end + 1;
			count++;
			}
	return 0;
	}

