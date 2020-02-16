//
//  main.c
//  Program1_ImgBlend
//
//  Created by Lam Tran on 1/15/20.
//  Copyright © 2020 Lam Tran. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
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

unsigned char* blendImage(unsigned char *data1, unsigned char *data2, unsigned char *data3, tagBITMAPFILEHEADER fhBig, tagBITMAPINFOHEADER fihBig, tagBITMAPFILEHEADER fhSmall, tagBITMAPINFOHEADER fihSmall, float ratio);
unsigned char mixColor(unsigned char color1, unsigned char color2, float ratio);
unsigned char getColor(unsigned char *data,int width, int height, int x, int y, int offset);
int getMax(int a, int b);
unsigned char get_color_bilinear(unsigned char *imagedata,int imagewidth, int imageheight,float x,float y, int offset);

int main(int argc, const char * argv[]) {

    // insert code here...
    char imageName1[1000];
    char imageName2[1000];
    float ratio = atof(argv[3]);
    FILE *image1;
    FILE *image2;
    FILE *blendedImage;
    tagBITMAPFILEHEADER fh1;
    tagBITMAPINFOHEADER fih1;
    tagBITMAPFILEHEADER fh2;
    tagBITMAPINFOHEADER fih2;
    tagBITMAPFILEHEADER fh3;
    tagBITMAPINFOHEADER fih3;
    // printf("Type in the first image: \n");
    // scanf("%s",imageName1);
    // printf("Type in the second image: \n");
    // scanf("%s",imageName2);
    // printf("Ratio for image1 to image2: \n");
    // scanf("%f",&ratio);

    printf("image1 = %s, image2 = %s, ratio = %f",imageName1,imageName2,ratio);
    image1 = fopen(argv[1],"rb");
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

    printf("done reading and storing to fileheader ");

    fread(&fih1,1,sizeof(tagBITMAPINFOHEADER),image1);
    
    fseek(image1, fh1.bfOffBits,0);

    unsigned char *data = (unsigned char*)malloc(fih1.biSizeImage);
    fread(data,1,fih1.biSizeImage,image1);
    printf("done reading and storing to fileInfoheader\n");
    printf("Image 1 size = %d", fih1.biSizeImage);
    fclose(image1);
    
    
    image1 = fopen("result_image1.bmp", "wb");
    fwrite(&fh1.bfType, 1, sizeof(WORD), image1);

    fwrite(&fh1.bfSize, 1, sizeof(DWORD), image1);
    fwrite(&fh1.bfReserved1, 1, sizeof(WORD), image1);
    fwrite(&fh1.bfReserved2, 1, sizeof(WORD), image1);
    fwrite(&fh1.bfOffBits,1,sizeof(DWORD),image1);
    fwrite(&fih1,1,sizeof(tagBITMAPINFOHEADER),image1);
    fwrite(data,1,fih1.biSizeImage,image1);
    fclose(image1);
    // Done createing Image1


    image2 = fopen(argv[2],"rb");
    if (image2 == NULL)
    {   
        printf("file not found");
        return 0;
    }



    fread(&fh2.bfType,1,sizeof(WORD),image2);
    fread(&fh2.bfSize,1,sizeof(DWORD),image2);
    fread(&fh2.bfReserved1,1,sizeof(WORD),image2);
    fread(&fh2.bfReserved2,1,sizeof(WORD),image2);
    fread(&fh2.bfOffBits,1,sizeof(DWORD),image2);

    printf("done reading and storing to fileheader ");

    fread(&fih2,1,sizeof(tagBITMAPINFOHEADER),image2);
    
    fseek(image2, fh2.bfOffBits,0);

    unsigned char *data2= (unsigned char*)malloc(fih2.biSizeImage);
    fread(data2,1,fih2.biSizeImage,image2);
    printf("done reading and storing to fileInfoheader ");

    fclose(image2);
    
    
    image2 = fopen("result_image2.bmp", "wb");
    fwrite(&fh2.bfType, 1, sizeof(WORD), image2);

    fwrite(&fh2.bfSize, 1, sizeof(DWORD), image2);
    fwrite(&fh2.bfReserved1, 1, sizeof(WORD), image2);
    fwrite(&fh2.bfReserved2, 1, sizeof(WORD), image2);
    fwrite(&fh2.bfOffBits,1,sizeof(DWORD),image2);
    fwrite(&fih2,1,sizeof(tagBITMAPINFOHEADER),image2);
    fwrite(data2,1,fih2.biSizeImage,image2);
    printf("\nImage 2 size = %d", fih2.biSizeImage);

    fclose(image2);
    printf("done writing\n");
    //done creating image 3

    
    unsigned char *data3 = (unsigned char*)malloc(getMax(fih1.biSizeImage,fih2.biSizeImage));

    if (fih1.biSizeImage > fih2.biSizeImage)
    {
        printf("\n Fih1 has bigger size so blending with that size");
        data3 = blendImage(data, data2, data3, fh1, fih1, fh2, fih2, ratio);
        fh3 = fh1;
        fih3 = fih1;
    }
    else if (fih1.biSizeImage < fih2.biSizeImage)
    {
        printf("\n Fih2 has bigger size so blending with that size");

        data3 = blendImage(data2, data, data3, fh2, fih2, fh1, fih1, ratio);
        fh3 = fh2;
        fih3 = fih2;
    }
    else
    {
        data3 = blendImage(data, data2, data3, fh1, fih1, fh2, fih2, ratio);
        fh3 = fh1;
        fih3 = fih1;
    }

    blendedImage = fopen(argv[4],"wb");
    fwrite(&fh3.bfType, 1, sizeof(WORD), blendedImage);

    fwrite(&fh3.bfSize, 1, sizeof(DWORD), blendedImage);
    fwrite(&fh3.bfReserved1, 1, sizeof(WORD), blendedImage);
    fwrite(&fh3.bfReserved2, 1, sizeof(WORD), blendedImage);
    fwrite(&fh3.bfOffBits,1,sizeof(DWORD),blendedImage);
    fwrite(&fih3,1,sizeof(tagBITMAPINFOHEADER),blendedImage);
    fwrite(data3,1,fih3.biSizeImage,blendedImage);
    printf("\nBlended Size = %d", fih3.biSizeImage);
    fclose(blendedImage);

}

unsigned char get_color_bilinear(unsigned char *imagedata,int imagewidth, int imageheight,float x,float y, int offset)
{
    
    int x1 = (int)x;
    int x2 = x1 + 1;
    int y1 = (int)y;
    int y2 = y1 + 1;
    if(x2>=imagewidth)return 0;
    if(y2>=imageheight)return 0;
    float dx = x - (float)x1;
    float dy = y - (float)y1;

    unsigned char color_left, color_right, color_result;

    unsigned char color_upper_left, color_upper_right, color_lower_left, color_lower_right;

    color_upper_left = getColor(imagedata, imagewidth, imageheight, x1,y2,offset);
    color_upper_right = getColor(imagedata, imagewidth, imageheight, x2,y2,offset);
    color_lower_left = getColor(imagedata, imagewidth, imageheight, x1,y1,offset);
    color_lower_right = getColor(imagedata, imagewidth, imageheight, x2,y1,offset);

    color_left = color_upper_left*(1-dy) + color_lower_left*(dy);
    color_right = color_upper_right*(1-dy) + color_lower_right*(dy);
    color_result = color_left*(1-dx) + color_right*dx;

    return color_result;
}

int getMax(int a, int b)
{
    if (a > b)
    {
        return a;
    }
    if (a < b)
    {
        return b;
    }
    return a;
}

unsigned char mixColor (unsigned char color1, unsigned char color2, float ratio)
{
    unsigned char blendedColor;
    blendedColor = (color1*ratio) + (color2)*(1 - ratio);
    return blendedColor;
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
unsigned char* blendImage(unsigned char *data1, unsigned char *data2, unsigned char *data3, tagBITMAPFILEHEADER fhBig, tagBITMAPINFOHEADER fihBig, tagBITMAPFILEHEADER fhSmall, tagBITMAPINFOHEADER fihSmall, float ratio)
{
    
    data3 = (unsigned char*)malloc(getMax(fihSmall.biSizeImage,fihBig.biSizeImage));
    unsigned char red1, red2;
    unsigned char blue1, blue2;
    unsigned char green1, green2;
    unsigned char mixedRed, mixedBlue, mixedGreen;
    int smallX, smallY;

    for (int x =0; x < fihBig.biWidth; x++)
    {
        for (int y = 0; y < fihBig.biHeight; y++)
        {

            red1 = getColor(data1,fihBig.biWidth,fihBig.biHeight,x,y,0);
            blue1 = getColor(data1,fihBig.biWidth,fihBig.biHeight,x,y,1);
            green1 = getColor(data1,fihBig.biWidth,fihBig.biHeight,x,y,2);

            smallX = x * ((float)(fihSmall.biWidth)/(float)(fihBig.biWidth));
            smallY = y * ((float)(fihSmall.biHeight)/(float)(fihBig.biHeight));
            if (smallX >= fihSmall.biWidth)continue;
            if (smallY >= fihSmall.biHeight)continue;

            red2 = get_color_bilinear(data2,fihSmall.biWidth,fihSmall.biHeight,smallX,smallY,0);
            blue2 = get_color_bilinear(data2,fihSmall.biWidth,fihSmall.biHeight,smallX,smallY,1);
            green2 = get_color_bilinear(data2,fihSmall.biWidth,fihSmall.biHeight,smallX,smallY,2);


            mixedRed = mixColor(red1,red2,ratio);
            mixedBlue = mixColor(blue1,blue2,ratio);
            mixedGreen = mixColor(green1,green2,ratio);

            int row = y;
            int row_padding = fihBig.biWidth * 3;
            int padding_row = 0;
            if ((row_padding % 4) != 0)
            {
                padding_row = 4 - (row_padding % 4);
            }


            data3[(x+y*fihBig.biWidth)*3+0 + padding_row*y] = mixedRed;
            data3[(x+y*fihBig.biWidth)*3+1 + padding_row*y] = mixedBlue;
            data3[(x+y*fihBig.biWidth)*3+2 + padding_row*y] = mixedGreen;
        }
    }
    return data3;
}
