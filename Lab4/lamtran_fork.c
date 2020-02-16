//
//  main.c
//  Program1_ImgBlend
//
//  Created by Lam Tran on 1/15/20.
//  Copyright © 2020 Lam Tran. All rights reserved.
//

#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
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

unsigned char* brightenImage(unsigned char *data1, unsigned char *dataResult, tagBITMAPFILEHEADER fh, tagBITMAPINFOHEADER fih, float brightness, int y_start, int y_end);
unsigned char brightenColor(unsigned char color, float brightness);
unsigned char getColor(unsigned char *data,int width, int height, int x, int y, int offset);

int main(int argc, const char * argv[]) {
    char imageName1[1000];
    int forkEnable;
    float brightness = atof(argv[2]);
    clock_t start_t, end_t;
    double cpu_time_used;

    FILE *image1;
    FILE *imageBright;
    tagBITMAPFILEHEADER fh1;
    tagBITMAPINFOHEADER fih1;
    tagBITMAPFILEHEADER fhBright;
    tagBITMAPINFOHEADER fihBright;

    // printf("Type in the first image: \n");
    // scanf("%s",imageName1);
    // printf("Type in the brightness ratio: \n");
    // scanf("%f",&brightness);
    // printf("Wanna fork?\n");
    // scanf("%i", &forkEnable);
    start_t = clock();


    image1 = fopen(argv[1],"rb");
    // image1 = fopen(imageName1, "rb");
    if (image1 == NULL)
    {
        printf("file not found");
        return 0;
    }


    fread(&fh1.bfType,1,sizeof(WORD),image1);
    fread(&fh1.bfSize,1,sizeof(DWORD),image1);
    fread(&fh1.bfReserved1,1,sizeof(WORD),image1);
    fread(&fh1.bfReserved2,1,sizeof(WORD),image1);
    fread(&fh1.bfOffBits,1,sizeof(DWORD),image1);
    fread(&fih1,1,sizeof(tagBITMAPINFOHEADER),image1);
    
    fseek(image1, fh1.bfOffBits,0);

    unsigned char *data = (unsigned char*)mmap(NULL, fih1.biSizeImage, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);;
    unsigned char *data3 = (unsigned char*)mmap(NULL, fih1.biSizeImage, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

   

    fread(data,1,fih1.biSizeImage,image1);

    fclose(image1);
    
    if (atof(argv[3]) == 1)
    {
        if (fork() == 0)
        {
            // Child, then just pass in the whole data but only work with the top half of the pixel
            printf("child");
            data3 = brightenImage(data, data3, fh1, fih1, brightness, 0, fih1.biHeight/2);
            return 0;
        }
        else
        {
            //parent
            data3 = brightenImage(data, data3, fh1, fih1, brightness, fih1.biHeight/2 , fih1.biHeight);
            int i;
            wait(&i);
            image1 = fopen(argv[4],"wb");

            fwrite(&fh1.bfType, 1, sizeof(WORD), image1);

            fwrite(&fh1.bfSize, 1, sizeof(DWORD), image1);
            fwrite(&fh1.bfReserved1, 1, sizeof(WORD), image1);
            fwrite(&fh1.bfReserved2, 1, sizeof(WORD), image1);
            fwrite(&fh1.bfOffBits,1,sizeof(DWORD),image1);
            fwrite(&fih1,1,sizeof(tagBITMAPINFOHEADER),image1);
            

            printf("Forking!!!! \n");
            fwrite(data3,1,fih1.biSizeImage,image1);

            
            fclose(image1);

            end_t = clock();
            cpu_time_used = ((double) (end_t - start_t)) / CLOCKS_PER_SEC;
            printf("start_t = %ld\n", start_t);
            printf("end_t = %ld\n", end_t);
            printf("Totaltime with fork = %f second", cpu_time_used);
            return 0;
        }

    }


    // NO FORK
    else
    {
    printf("No fork then \n");
    image1 = fopen(argv[4],"wb");
    data3 = brightenImage(data,data3, fh1, fih1, brightness, 0, fih1.biHeight);


    fwrite(&fh1.bfType, 1, sizeof(WORD), image1);

    fwrite(&fh1.bfSize, 1, sizeof(DWORD), image1);
    fwrite(&fh1.bfReserved1, 1, sizeof(WORD), image1);
    fwrite(&fh1.bfReserved2, 1, sizeof(WORD), image1);
    fwrite(&fh1.bfOffBits,1,sizeof(DWORD),image1);
    fwrite(&fih1,1,sizeof(tagBITMAPINFOHEADER),image1);

    fwrite(data3,1,fih1.biSizeImage,image1);

    
    fclose(image1);
    end_t = clock();
    cpu_time_used = ((double) (end_t - start_t)) / CLOCKS_PER_SEC;
    printf("start_t = %ld\n", start_t);
    printf("end_t = %ld\n", end_t);
    printf("Totaltime without fork = %f second", cpu_time_used);
    return 0;
    }
   

    // Done createing Image
    // unsigned char *data3 = (unsigned char*)malloc(fih1.biSizeImage);


    // data3 = brightenImage(data,fh1, fih1, brightness);
    // data3 = data;



    // imageBright = fopen(argv[3],"wb");
   



}

unsigned char brightenColor(unsigned char color, float brightness)
{
    // float brightNess255 = (int)(brightness*255);
    int addBrightInt = (int)color + brightness*255;
    unsigned char addBright = (unsigned char)(brightness*255);
    unsigned char result = color + addBright;
    if (addBrightInt >= 255)
    {
        return 255;
    }
    return result;
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

unsigned char* brightenImage(unsigned char *data1, unsigned char *dataResult, tagBITMAPFILEHEADER fh, tagBITMAPINFOHEADER fih, float brightness, int y_start, int y_end)
{
    
    unsigned char *dataBright = dataResult;
    unsigned char red,blue,green;
    unsigned char redBright;
    unsigned char blueBright;
    unsigned char greenBright;
    for (int x = 0; x < fih.biWidth; x++)
    {
        for (int y = y_start; y < y_end; y++)
        {

            red = getColor(data1,fih.biWidth,fih.biHeight,x,y,0);
            blue = getColor(data1,fih.biWidth,fih.biHeight,x,y,1);
            green = getColor(data1,fih.biWidth,fih.biHeight,x,y,2);

            redBright = brightenColor(red, brightness);
            blueBright = brightenColor(blue, brightness);
            greenBright = brightenColor(green, brightness);

            int row = y;
            int row_padding = fih.biWidth * 3;
            int padding_row = 0;
            if ((row_padding % 4) != 0)
            {
                padding_row = 4 - (row_padding % 4);
            }

            dataBright[(x+y*fih.biWidth)*3+0 + padding_row*y] = redBright;
            dataBright[(x+y*fih.biWidth)*3+1 + padding_row*y] = blueBright;
            dataBright[(x+y*fih.biWidth)*3+2 + padding_row*y] = greenBright;

        }
    }
    return dataBright;
}
