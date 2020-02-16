//midterm practice
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/mman.h>


typedef struct le{
   struct le *next,*prev;
   int number;
}le;
le *top = NULL;

typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef unsigned int LONG;
typedef struct Header
{
    WORD type; //specifies the file type
    DWORD size; //specifies the size in bytes of the bitmap file
    WORD reserved1; //reserved; must be 0
    WORD reserved2; //reserved; must be 0
    DWORD offset; //species the offset in bytes from the bitmapfileheader to the bitmap bits
}Header;
typedef struct Infoheader
{
    DWORD size; //specifies the number of bytes required by the struct
    LONG width; //specifies width in pixels
    LONG height; //species height in pixels
    WORD numPlanes; //specifies the number of color planes, must be 1
    WORD bitCount; //specifies the number of bit per pixel
    DWORD compression;//spcifies the type of compression
    DWORD sizeImage; //size of image in bytes
    LONG xres; //number of pixels per meter in x axis
    LONG yres; //number of pixels per meter in y axis
    DWORD numColors; //number of colors used by th ebitmap
    DWORD importantColors; //number of colors that are important
}Infoheader;

typedef struct datafile
{
   char type;
   int size;
   char type2;
   int offset;
   char text[10];
}datafile;

//------------fork problem------------//
/* should print:
1
2
3
4
5
*/
void forkproblem(){
   int f1 = fork();
   if (f1==0){
      int f2 = fork();
      if (f2==0){
         printf("1\n");
      }
      else{
         int i1;
         wait(&i1);
         int f3 = fork();
         if (f3 == 0){
            printf("2\n");
         }
         else{
            int i2;
            wait(&i2);
            printf("3\n");
         }
      }
   }
   else{
      int i3;
      wait(&i3);
      int f4 = fork();
      if (f4==0){
         printf("4\n");
      }
      else{
         int i4;
         wait(&i4);
         printf("5\n");
      }
   }
}

//------------double linked list problem------------//

void new_element(int num){
   // if you want to read from the command line instead of arg
   // int *numptr;
   // printf("enter a number\n");
   // scanf("%d",num);
   // int num = *numptr;
   le *new = (le*)malloc(sizeof(le));
   new->number = num;
   if (top!=NULL){
      new->next = top;
      new->prev = NULL;
      top->prev = new;
      top = new;

   }
   else{
      new->next = NULL;
      new->prev = NULL;
      top = new;
   }
}

void printlist(){
   printf("--------------\n");
   if (top==NULL){
      printf("list is empty\n");
      return;
   }
   int num = 0;
   for (le *curr = top;curr!=NULL;curr=curr->next){
      printf("%d|address: %p|number: %d|next: %p|prev: %p\n",num,curr,curr->number,curr->next,curr->prev);
      num++;
   }
}

void remove_element(int d){
   for (le *curr = top;curr!=NULL;curr=curr->next){
      if (curr->number == d){
         if (curr->prev!=NULL){
            curr->prev->next = curr->next;
         }
         if (curr->next!=NULL){
            curr->next->prev = curr->prev;
         }
         free(curr);
      }
   }
}

void delete_list(){
   le *curr = top;
   le *temp = curr;
   for(;curr!=NULL;curr=curr->next){
      temp = curr;
      free(temp);
   }
   free(curr);
   top = NULL;
}

void dublinklistproblem(){
   //this is just arbitrary testing
   new_element(1);
   new_element(2);
   new_element(1);
   new_element(4);
   new_element(1);
   new_element(6);
   printlist();

   remove_element(1);
   printlist();

   delete_list();
   printlist();

   /* prints something similar to the following
--------------
0|address: 0x7ff331401880|number: 6|next: 0x7ff331401860|prev: 0x0
1|address: 0x7ff331401860|number: 1|next: 0x7ff331401840|prev: 0x7ff331401880
2|address: 0x7ff331401840|number: 4|next: 0x7ff331401820|prev: 0x7ff331401860
3|address: 0x7ff331401820|number: 1|next: 0x7ff331401800|prev: 0x7ff331401840
4|address: 0x7ff331401800|number: 2|next: 0x7ff3314017e0|prev: 0x7ff331401820
5|address: 0x7ff3314017e0|number: 1|next: 0x0|prev: 0x7ff331401800
--------------
0|address: 0x7ff331401880|number: 6|next: 0x7ff331401840|prev: 0x0
1|address: 0x7ff331401840|number: 4|next: 0x7ff331401800|prev: 0x7ff331401880
2|address: 0x7ff331401800|number: 2|next: 0x0|prev: 0x7ff331401840
--------------
list is empty
   */
}

//------------read bitmap problem------------//
void bitmapproblem(){
   /*
   You could chose the file with command line args or just edit what you want below
   Make sure that file is in the directory

   Also sorry I know you probably have different names in your struct but
   I think you get it
   */
   FILE *infile = fopen("jar.bmp","rb");
   if (infile == NULL){
      printf("file not found\n");
      return;
   }
   Header headerbody;
   Infoheader infoheaderbody;
   Header *header = &headerbody;
   Infoheader *infoheader = &infoheaderbody;

   fread(&header->type,1,sizeof(WORD),infile);
   fread(&header->size,1,sizeof(DWORD),infile);
   fread(&header->reserved1,1,sizeof(WORD),infile);
   fread(&header->reserved2,1,sizeof(WORD),infile);
   fread(&header->offset,1,sizeof(DWORD),infile);
   fread(infoheader,1,sizeof(Infoheader),infile);

   unsigned char *datain = malloc(infoheader->sizeImage);
   unsigned char *dataout = malloc(infoheader->sizeImage);
   fread(datain,1,infoheader->sizeImage,infile);
   fclose(infile);

   int modwidth = infoheader->width;
   if (modwidth%4 != 0){
      modwidth = modwidth+4-modwidth%4;
   }

   for(int x=0;x<infoheader->width;x++){
      for(int y=0;y<infoheader->height;y++){
         for(int offset=0;offset<3;offset++){
            dataout[(x+y*infoheader->width)*3+offset] = 255-datain[(x+y*modwidth)*3+offset];
         }
      }
   }

   FILE *outfile = fopen("out.bmp","wb+");
   fwrite(&header->type,1,sizeof(WORD),outfile);
   fwrite(&header->size,1,sizeof(DWORD),outfile);
   fwrite(&header->reserved1,1,sizeof(WORD),outfile);
   fwrite(&header->reserved2,1,sizeof(WORD),outfile);
   fwrite(&header->offset,1,sizeof(DWORD),outfile);
   fwrite(infoheader,1,sizeof(Infoheader),outfile);
   fwrite(dataout,1,infoheader->sizeImage,outfile);
   fclose(outfile);
   free(datain);
   free(dataout);
}
//------------read binary file problem------------//
void binaryfileproblem(){
   char type;
   int size;
   char type2;
   int offset;
   char text[10];
   FILE *file = fopen("file.bin","r");
   datafile *dfile = (datafile*)malloc(sizeof(datafile));
   fread(&dfile->type,1,sizeof(char),file);
   fread(&dfile->size,1,sizeof(int),file);
   fread(&dfile->type2,1,sizeof(char),file);
   fread(&dfile->offset,1,sizeof(int),file);
   fread(&dfile->text,1,sizeof(char)*10,file);
   fclose(file);

   //these prints are not part of the question but just for testing
   printf("type: %c\n",dfile->type);
   printf("size: %d\n",dfile->size);
   printf("type2: %c\n",dfile->type2);
   printf("offset: %d\n",dfile->offset);
   printf("text: %s\n",dfile->text);

   /*should print (in the bin file I provided)
   a
   5
   b
   6
   sample
   */
}
//------------memory allocation problem------------//
void memoryallocationproblem(){
   //probably inefficient but it works LOL
   printf("enter a word to be added to the heap\n");
   printf("re-enter the word to have it deleted\n");
   printf("type \"print\" to print all words\n");
   printf("type \"stop\" to end the program\n");
   char *text[10] = {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
   char newText[1000];
   int stopFlag = 0;
   while (stopFlag == 0){
      printf("enter word: \n");
      scanf("%s",newText);
      if (strncmp(newText,"stop",4)==0){
         for(int i=0;i<10;i++){
            if (text[i]!=NULL){
               munmap(text[i],sizeof(char)*strlen(text[i]));
            }
         }
         stopFlag = 1;
      }
      else if(strncmp(newText,"print",5)==0){
         printf("---------\n");
         for (int i=0;i<10;i++){
            if (text[i]!=NULL){
               printf("%s\n",text[i]);
            }
         }
         printf("---------\n");
      }
      else{
         int openspot = -1;
         int duplicate = 0;
         int duplicatenum;
         for (int i=0;i<10;i++){
            if (text[i]!=NULL&&strncmp(newText,text[i],strlen(newText))==0){
               duplicate = 1;
               duplicatenum= i;
               break;
            }
            if (openspot==-1 && text[i]==NULL){
               openspot = i;
            }
         }
         if (duplicate == 0){
            if (openspot == -1){
               printf("word array is full. limit 10.\n");
            }
            else{
               char *textptr = (char*)mmap(NULL,strlen(newText),PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS,-1,0);
               strcpy(textptr,newText);
               text[openspot] = textptr;
            }
         }
         else{
            munmap(&text[duplicatenum],strlen(text[duplicatenum]));
            text[duplicatenum] = NULL;
         }
      }
   }
}

//-----------multi tasking problem------------//
void multitaskingproblem(){
   /*
   f1 child: 1
   f1 parent: 2
   f2 child: 3
   f2 parent: 4
   */
   int *arr = (int*)mmap(NULL,1000*sizeof(int),PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS,-1,0);
   int f1 = fork();
   int f2 = fork();
   if(f1==0&&f2==0){
      for(int a=0;a<250;a++){
         arr[a] = 1;
      }
   }
   else if(f1==0&&f2!=0){
      for(int b=250;b<500;b++){
         arr[b] = 2;
      }
   }
   else if (f1!=0&&f2==0){
      for(int c=500;c<750;c++){
         arr[c] = 3;
      }
   }
   else{
      for(int d=750;d<1000;d++){
         arr[d] = 4;
      }
   }
   if (f1==0){
      return;
   }
   else{
      int k;
      wait(&k);
   }
   if(f2==0){
      return;
   }
   else{
      int j;
      wait(&j);
   }
   for(int i=0;i<1000;i++){
      if (i%50==0){
         //an attempt to make the outcome somewhat readable
         printf("\n");
      }
      printf("%d",arr[i]);
   }
   printf("\n");
   munmap(arr,1000*sizeof(int));
}

int main(){
   // new_element(9);
   // new_element(3);
   // new_element(2);
   // new_element(5);
   // printlist();
   // remove_element(5);
   // printlist();
   
   // binaryfileproblem();
      //#1
   // forkproblem();
   //#2
   // dublinklistproblem();
   //#3
   // bitmapproblem();//make sure jar.bmp is in the directory, open out.bmp to see result
   //#4
   // binaryfileproblem();
   //#5
   memoryallocationproblem();
   //#6
   // multitaskingproblem();
   return 0;
}
