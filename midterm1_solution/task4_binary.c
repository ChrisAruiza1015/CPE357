#include <stdio.h>
#include <stdlib.h>

typedef struct datafile
{
    char type; //
    int size;//size of the data part in bytes
}datafile;



int main(int argc, const char * argv[]) {


    datafile data1;
    FILE *file1;

    file1 = fopen("file.bin","rb");
    if(file1 == NULL) {
        printf("failed to open\n");
    } else {
        printf("success!\n");
     } 

//1st::::::: Read the data frm the file and put it in the type

    fread(&data1.type,1,sizeof(char),file1);
    fread(&data1.size,1,sizeof(int),file1);

//2nd::::::: Create an array of data with the size of data1.size

    unsigned char * alldata = (unsigned char *) malloc((data1.size));
// Change all the data according to the instruction
    fread(alldata,1,(data1.size),file1);
    int i;
    if(data1.type == 'U') {
        for(i = 0; i < data1.size; i++) {
        alldata[i] = alldata[i] + 4;
        }
    } else if(data1.type == 'T') {
        for(i = 0; i < data1.size; i++) {
        alldata[i] = alldata[i] - 2;
        }
    } else if(data1.type == 'M') {
        for(i = 0; i < data1.size; i++) {
        alldata[i] = alldata[i] + 9;
        }
    

  }
    fclose(file1);
    
//Rewrite it back into the file

    file1 = fopen("result2.txt","wb+");
    fwrite(alldata,1,(data1.size),file1);
    fclose(file1);



    //Now print all the elements but how?



}