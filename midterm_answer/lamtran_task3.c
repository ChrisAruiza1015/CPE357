#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <string.h>
#include <time.h>


typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef unsigned int LONG;

typedef struct tagBITMAPFILEHEADER
{
    WORD bfType; //specifies the file type
    DWORD bfSize; //specifies the size in bytes of the bitmap file
    WORD bfReserved1; //reserved; must be 0
    WORD bfReserved2; //reserved; must be 0
    DWORD bfOffBits; //species the offset in bytes from the bitmapfileheader
}tagBITMAPFILEHEADER;
typedef struct tagBITMAPINFOHEADER
{
    DWORD biSize; //specifies the number of bytes required by the struct
    LONG biWidth; //specifies width in pixels
    LONG biHeight; //species height in pixels
    WORD biPlanes; //specifies the number of color planes, must be 1
    WORD biBitCount; //specifies the number of bit per pixel
    DWORD biCompression;//spcifies the type of compression
    DWORD biSizeImage; //size of image in bytes
    LONG biXPelsPerMeter; //number of pixels per meter in x axis
    LONG biYPelsPerMeter; //number of pixels per meter in y axis
    DWORD biClrUsed; //number of colors used by th ebitmap
    DWORD biClrImportant; //number of colors that are important
}tagBITMAPINFOHEADER;
unsigned char getColor(unsigned char *data,int width, int height, int x, int y, int offset);
unsigned char *removeRedBlue (unsigned char *data1, tagBITMAPFILEHEADER fh, tagBITMAPINFOHEADER fih);


int main(int argc, const char * argv[]) {

    tagBITMAPFILEHEADER fh1;
    tagBITMAPINFOHEADER fih1;
    FILE *image1;

    image1 = fopen(argv[1],"rb");
    if (image1 == NULL)
    {
        printf("File not found! \n");
        return 0;
    }

    fread(&fh1.bfType,1,sizeof(WORD),image1);
    fread(&fh1.bfSize,1,sizeof(DWORD),image1);
    fread(&fh1.bfReserved1,1,sizeof(WORD),image1);
    fread(&fh1.bfReserved2,1,sizeof(WORD),image1);
    fread(&fh1.bfOffBits,1,sizeof(DWORD),image1);

    printf("done reading and storing to fileheader ");

  
    fread(&fih1.biSize,1,sizeof(DWORD),image1);
    fread(&fih1.biWidth,1, sizeof(LONG),image1);
    fread(&fih1.biHeight,1,sizeof(LONG),image1);
    fread(&fih1.biPlanes,1,sizeof(WORD),image1);
    fread(&fih1.biBitCount,1,sizeof(WORD),image1);
    fread(&fih1.biCompression,1,sizeof(DWORD),image1);
    fread(&fih1.biSizeImage,1,sizeof(DWORD),image1);
    fread(&fih1.biXPelsPerMeter,1,sizeof(LONG),image1);
    fread(&fih1.biYPelsPerMeter,1,sizeof(LONG),image1);
    fread(&fih1.biClrUsed,1,sizeof(DWORD),image1);
    fread(&fih1.biClrImportant,1,sizeof(DWORD),image1);

    fseek(image1, fh1.bfOffBits,0);


    unsigned char *data = (unsigned char*)malloc(fih1.biSizeImage);
    fread(data,1,fih1.biSizeImage,image1);

    unsigned char *dataNew = (unsigned char*)malloc(fih1.biSizeImage);
    dataNew = removeRedBlue(data,fh1,fih1);

    fclose(image1);

    image1 = fopen(argv[2], "wb");
    fwrite(&fh1.bfType, 1, sizeof(WORD), image1);

    fwrite(&fh1.bfSize, 1, sizeof(DWORD), image1);
    fwrite(&fh1.bfReserved1, 1, sizeof(WORD), image1);
    fwrite(&fh1.bfReserved2, 1, sizeof(WORD), image1);
    fwrite(&fh1.bfOffBits,1,sizeof(DWORD),image1);
    fwrite(&fih1,1,sizeof(tagBITMAPINFOHEADER),image1);
    fwrite(dataNew,1,fih1.biSizeImage,image1);
    fclose(image1);


    

}

unsigned char *removeRedBlue (unsigned char *data1, tagBITMAPFILEHEADER fh, tagBITMAPINFOHEADER fih)
{


    unsigned char *dataNew = (unsigned char*)malloc(fih.biSizeImage);
    unsigned char red, blue, green;
    unsigned char redResult, blueResult, greenResult;

    for (int x =0; x < fih.biWidth; x++)
    {
        for (int y = 0; y < fih.biHeight; y++)
        {
            red = getColor(data1,fih.biWidth,fih.biHeight,x,y,2);
            blue = getColor(data1,fih.biWidth,fih.biHeight,x,y,3);
            green = getColor(data1,fih.biWidth,fih.biHeight,x,y,1);

            redResult = red - red;
            blueResult = blue - blue;
            greenResult = green;

            int row = y;
            int row_padding = fih.biWidth * 3;
            int padding_row = 0;
            if ((row_padding % 4) != 0)
            {
                padding_row = 4 - (row_padding % 4);
            }


            dataNew[(x+y*fih.biWidth)*3+2 + padding_row*y] = redResult;
            dataNew[(x+y*fih.biWidth)*3+3 + padding_row*y] = blueResult;
            dataNew[(x+y*fih.biWidth)*3+1 + padding_row*y] = greenResult;

        }
    }
    return dataNew;

}


unsigned char getColor(unsigned char *data,int width, int height, int x, int y, int offset)
{   
    int row = y;
    int row_padding = width * 3;
    int padding_row = 0;
    if ((row_padding % 4) != 0)
    {
        padding_row = 4 - (row_padding % 4);
    }
    return data[(x+y*width)*3+offset + padding_row*y];
}