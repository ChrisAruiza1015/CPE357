#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#define MATRIX_DIMENSION_XY 10



//SEARCH FOR TODO


//************************************************************************************************************************
// sets one element of the matrix
void set_matrix_elem(float *M,int x,int y,float f)
{
M[x + y*MATRIX_DIMENSION_XY] = f;
}
//************************************************************************************************************************
// lets see it both are the same
int quadratic_matrix_compare(float *A,float *B)
{
for(int a = 0;a<MATRIX_DIMENSION_XY;a++)
    for(int b = 0;b<MATRIX_DIMENSION_XY;b++)
       if(A[a +b * MATRIX_DIMENSION_XY]!=B[a +b * MATRIX_DIMENSION_XY]) 
        return 0;
   
return 1;
}
//************************************************************************************************************************
//print a matrix
void quadratic_matrix_print(float *C)
{
    printf("\n");
for(int a = 0;a<MATRIX_DIMENSION_XY;a++)
    {
    printf("\n");
    for(int b = 0;b<MATRIX_DIMENSION_XY;b++)
        printf("%.2f,",C[a + b* MATRIX_DIMENSION_XY]);
    }
printf("\n");
}
//************************************************************************************************************************
// multiply two matrices
void quadratic_matrix_multiplication(float *A,float *B,float *C)
{
	//nullify the result matrix first
for(int a = 0;a<MATRIX_DIMENSION_XY;a++)
    for(int b = 0;b<MATRIX_DIMENSION_XY;b++)
        C[a + b*MATRIX_DIMENSION_XY] = 0.0;
//multiply
for(int a = 0;a<MATRIX_DIMENSION_XY;a++) // over all cols a
    for(int b = 0;b<MATRIX_DIMENSION_XY;b++) // over all rows b
        for(int c = 0;c<MATRIX_DIMENSION_XY;c++) // over all rows/cols left
            {
                C[a + b*MATRIX_DIMENSION_XY] += A[c + b*MATRIX_DIMENSION_XY] * B[a + c*MATRIX_DIMENSION_XY]; 
            }
}
//************************************************************************************************************************
void synch(int par_id,int par_count,int *ready)  //idea: every synch step waits for another value (1,2,3,4,5,..)
{
int synchid = ready[par_count]+1; //synchid is progressing with each synch - I know, one from section 1 and 3 had a similar idea.
ready[par_id]=synchid;
int breakout = 0;
while(1)
    {
    breakout=1;    
    for(int i=0;i<par_count;i++) 
        {
        if(ready[i]<synchid) //"less than" is super important here. Do you know why? Because one process could run ahead and increment its ready[par_is] var already.
            {breakout = 0;break;}
        }
    if(breakout==1)
        {
        ready[par_count] = synchid; //and here we increment the additional variable
        break;
        }
    }
}

//************************************************************************************************************************
int main(int argc, char *argv[])
{
clock_t start_t, end_t;
double cpu_time_used;
int par_id = 0; // the parallel ID of this process
int par_count = 1; // the amount of processes

float *A,*B,*C; //matrices A,B and C
int *ready; //needed for synch
if(argc!=3){printf("no shared\n");}
else
    {
    par_id= atoi(argv[1]);
    par_count= atoi(argv[2]);
   // strcpy(shared_mem_matrix,argv[3]);
    }
if(par_count==1){printf("only one process\n");}

int fd[4];
int work = 10/par_count;
int start = par_id * work;
int end = start + work;

if(par_id==0)
    {

    start_t = clock();
    //TODO: init the shared memory for A,B,C, ready. shm_open with C_CREAT here! then ftruncate! then mmap
    fd[0] = shm_open("matrixA", O_RDWR|O_CREAT, 0777);
    ftruncate(fd[0],sizeof(float)*10);
    A = mmap(0,sizeof(float)*100,PROT_READ|PROT_WRITE,MAP_SHARED,fd[0],0);

    fd[1] = shm_open("matrixB", O_RDWR|O_CREAT, 0777);
    ftruncate(fd[1],sizeof(float)*10);
    B = mmap(0,sizeof(float)*100,PROT_READ|PROT_WRITE,MAP_SHARED,fd[1],0);

    fd[2] = shm_open("matrixC", O_RDWR|O_CREAT, 0777);
    ftruncate(fd[2],sizeof(float)*10);
    C = mmap(0,sizeof(float)*100,PROT_READ|PROT_WRITE,MAP_SHARED,fd[2],0);

    fd[3] = shm_open("synchobject", O_RDWR|O_CREAT, 0777);
    ftruncate(fd[3],sizeof(int)*11);
    ready = mmap(0,sizeof(int)*11,PROT_READ|PROT_WRITE,MAP_SHARED,fd[3],0);
    }
else
    {
	//TODO: init the shared memory for A,B,C, ready. shm_open withOUT C_CREAT here! NO ftruncate! but yes to mmap
    sleep(2); //needed for initalizing synch

    fd[0] = shm_open("matrixA", O_RDWR, 0777);
    A = mmap(0,sizeof(float)*100,PROT_READ|PROT_WRITE,MAP_SHARED,fd[0],0);

    fd[1] = shm_open("matrixB", O_RDWR, 0777);
    B = mmap(0,sizeof(float)*100,PROT_READ|PROT_WRITE,MAP_SHARED,fd[1],0);

    fd[2] = shm_open("matrixC", O_RDWR, 0777);
    C = mmap(0,sizeof(float)*100,PROT_READ|PROT_WRITE,MAP_SHARED,fd[2],0);

    fd[3] = shm_open("synchobject", O_RDWR, 0777);
    ready = mmap(0,sizeof(int)*11,PROT_READ|PROT_WRITE,MAP_SHARED,fd[3],0);
    }


synch(par_id,par_count,ready);

if(par_id==0)
    {
	//TODO: initialize the matrices A and B
    for (int a = 0; a < 100; a++)
        A[a] = rand() % 10;
    for (int b = 0; b < 100; b++)
        B[b] = rand() % 10;
    // for (int c = 0; c < 100; c++)
    //     C[c] = 0.0;
    printf("Printing matrix A = \n");
    quadratic_matrix_print(A);
    printf("Printing matrix B = \n");
    quadratic_matrix_print(B);
    }

synch(par_id,par_count,ready);
//TODO: quadratic_matrix_multiplication_parallel(par_id, par_count,A,B,C, ...);

if (par_id != (par_count - 1))
{
for(int a = 0;a<MATRIX_DIMENSION_XY;a++)
    for(int b = start;b<end;b++)
        C[a + b*MATRIX_DIMENSION_XY] = 0.0;

for(int a = 0;a<MATRIX_DIMENSION_XY;a++) // over all cols a
    for(int b = start;b<end;b++) // over all rows b
        for(int c = 0;c<MATRIX_DIMENSION_XY;c++) // over all rows/cols left
            {
                C[a + b*MATRIX_DIMENSION_XY] += A[c + b*MATRIX_DIMENSION_XY] * B[a + c*MATRIX_DIMENSION_XY]; 
            }
}
else{
for(int a = 0;a<MATRIX_DIMENSION_XY;a++)
    for(int b = start;b<MATRIX_DIMENSION_XY;b++)
        C[a + b*MATRIX_DIMENSION_XY] = 0.0;

for(int a = 0;a<MATRIX_DIMENSION_XY;a++) // over all cols a
    for(int b = start;b<MATRIX_DIMENSION_XY;b++) // over all rows b
        for(int c = 0;c<MATRIX_DIMENSION_XY;c++) // over all rows/cols left
            {
                C[a + b*MATRIX_DIMENSION_XY] += A[c + b*MATRIX_DIMENSION_XY] * B[a + c*MATRIX_DIMENSION_XY]; 
            }
}

	
synch(par_id,par_count,ready);



if(par_id==0)
{
    printf("Printing matrix C! \n");
    quadratic_matrix_print(C);
}
synch(par_id, par_count, ready);

close (fd[0]);
close (fd[1]);
close (fd[2]);
close (fd[3]);
shm_unlink("matrixA");
shm_unlink("matrixB");
shm_unlink("matrixC");
shm_unlink("synchobject");

//lets test the result:
float M[MATRIX_DIMENSION_XY * MATRIX_DIMENSION_XY];
quadratic_matrix_multiplication(A, B, M);
if (par_id == 0)
{
    end_t = clock();
    cpu_time_used = ((double) (end_t - start_t)) / CLOCKS_PER_SEC;
    printf("start_t = %ld\n", start_t);
    printf("end_t = %ld\n", end_t);
    printf("Totaltime of multiplication ~= %f second\n", cpu_time_used);
}
if (quadratic_matrix_compare(C, M))
printf("full points!\n");
else
printf("buuug!\n");


return 0;    
}