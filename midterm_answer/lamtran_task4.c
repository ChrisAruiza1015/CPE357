#include <stdio.h>
#include <stdlib.h>
typedef struct chunkdata
{
    char c;
    int offset;
}chunkdata;

int main(int argc, const char * argv[]) {


    chunkdata data1;
    FILE *file1;
    char c;

    file1 = fopen("crypt.bin","rb");
    if (file1 == NULL)
    {
        printf("File not found!\n");

        return 0;
    }



    while(1)
    {
        fread(&data1.c,1,sizeof(char),file1);
        fread(&data1.offset,1,sizeof(int),file1);
        if (data1.c == 0 && data1.offset == 0) break;
        c = (char)(data1.c - data1.offset);
        printf("%c",c);
    }



}