#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <string.h>


#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h> 
#define KBLU  "\x1B[34m"
#define RESET "\x1B[0m"

void sigintHandlerC(int sig)
{
    signal(SIGINT, sigintHandlerC); 
    fflush(stdout); 
}

void sigintHandlerZ(int sig)
{
    signal(SIGTSTP, sigintHandlerZ); 
    fflush(stdout); 
}


int main()
{
    struct stat sb;
    DIR *dir;
    struct dirent *dent;
    int counter;
    
    int f1, g, childPID;

  
    char directContainer [1000][1000];
    char fileName[1000];    

    
    

    int *active = mmap(NULL, 4, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    *active = 0;


    signal(SIGINT, sigintHandlerC);
    signal(SIGTSTP, sigintHandlerZ);
    f1 = fork();

    if (f1 == 0)
    {

        childPID = getpid();
        printf("Child pid = %d\n",childPID);


        while (1)
        {



            char cwd [1024];
            getcwd(cwd,sizeof(cwd));    
            printf(KBLU);
            printf("LamTranAssignment3Program %s", cwd );
            printf(RESET);
            printf("$ ");
            scanf("%s",fileName);

            printf("\n");



            if (strcmp(fileName,"list") == 0)
            {
                *active = 1;

                dir = opendir(".");
                if (dir == NULL)
                {
                    printf("Can't find anything\n");
                }
                else
                {
                    while ((dent = readdir(dir)) != NULL)
                    {
                        printf("%s",dent->d_name);
                        stat(dent->d_name, &sb);
                        if (S_ISDIR(sb.st_mode))
                            printf(" is directory ---- ");
                        printf("\n");
                    }
                }
                closedir(dir);

            }

            else if (strcmp(fileName,"q") == 0)
            {
                *active = -1;
                printf("ENDING PROGRAM!!!! \n");
                return 3;
            }

            else if (strcmp(fileName,"..") == 0)
            {
                *active = 1;
                chdir("..");
            }

            else if ('/'==fileName[0])
            {
                *active = 1;
                if (chdir(fileName+1) == -1)
                {
                    printf("Folder doesn't exist\n");
                }
                chdir(fileName+1);           
            }


            // else if (stat(fileName, &sb) == -1)
            // {
            //     *active = 1;
            //     printf("Invalid filename!!! \n");
            // }
            else
            {
                *active = 1;
                if (stat(fileName, &sb) == -1)
                {
                    *active = 1;
                    printf("Invalid filename!!! \n");
                }
                else
                {

                
                printf("ID of containing device:  [%lx,%lx]\n",
                (long) major(sb.st_dev), (long) minor(sb.st_dev));

                printf("File type:                ");

                switch (sb.st_mode & S_IFMT) {
                case S_IFBLK:  printf("block device\n");            break;
                case S_IFCHR:  printf("character device\n");        break;
                case S_IFDIR:  printf("directory\n");               break;
                case S_IFIFO:  printf("FIFO/pipe\n");               break;
                case S_IFLNK:  printf("symlink\n");                 break;
                case S_IFREG:  printf("regular file\n");            break;
                case S_IFSOCK: printf("socket\n");                  break;
                default:       printf("unknown?\n");                break;
                }

                printf("Inode number:             %ld\n", (long) sb.st_ino);

                printf("Mode:                     %lo (octal)\n",
                        (unsigned long) sb.st_mode);

                printf("Link number:              %ld\n", (long) sb.st_nlink);
                printf("ID OWNER:                 UID=%ld   GID=%ld\n",
                        (long) sb.st_uid, (long) sb.st_gid);

                printf("Preferred I/O block size: %ld bytes\n",
                        (long) sb.st_blksize);
                printf("File size:                %lld bytes\n",
                        (long long) sb.st_size);
                printf("Blocks allocated:         %lld\n",
                        (long long) sb.st_blocks);

                printf("Last status change:       %s", ctime(&sb.st_ctime));
                printf("Last file access:         %s", ctime(&sb.st_atime));
                printf("Last file modification:   %s", ctime(&sb.st_mtime));
                }


            }
            printf("\n");
        }
        return 0;

    }
    else /////PARENT PROCESS!!
    {

        int flag10 = 0;
        int start_sec;


        while (1)
        {

            time_t T= time(NULL);
            struct tm tm = *localtime(&T);

            if (*active == 1)
            {
                time_t T= time(NULL);

                struct tm tm = *localtime(&T);
                *active = 0;
                flag10 = 0;
            }

            if (*active == -1)
            {
                break;
            }


            if (flag10 == 0 && *active == 0)
            {
                start_sec = tm.tm_sec;
                flag10 = 1;
            }
        // printf("%i", tm.tm_sec);
            if (tm.tm_sec == ((start_sec + 10) % 60))
            {
                printf("Now 10 seconds is passed.");
                start_sec = start_sec + 10;
                flag10 = 0;
                if (*active == 0)
                {
                    printf("Killing child!\n");
                    kill(childPID, SIGKILL);

                }
          
                
            }
        }
    }


    
    

    return 0;   
}