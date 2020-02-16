/* Task 4 CPE 357-03*
   Christian Eckhardt
   Midterm 1
*/
#include <stdio.h>

typedef struct datafile {
  char type;
  int size; // size of the data part in bytes
}datafile;

datafile myDataFile;
FILE* file;
int main() {

  file = fopen("file.bin", "rb");
  int i;
  if(file == NULL) {
    printf("failed to open\n");
  } else {
    printf("success!\n");
  }

  fread(&myDataFile.type, 1, sizeof(char), file);
  fread(&myDataFile.size, 1, sizeof(int), file);

  unsigned char* charData = (unsigned char*)malloc(myDataFile.size);
  fread(charData, 1, myDataFile.size, file);
  if(myDataFile.type == 'U') {
    for(i = 0; i < myDataFile.size; i++) {
      charData[i] = charData[i] + 4;
    }
  } else if(myDataFile.type == 'T') {
    for(i = 0; i < myDataFile.size; i++) {
      charData[i] = charData[i] - 2;
    }
  } else if(myDataFile.type == 'M') {
    for(i = 0; i < myDataFile.size; i++) {
      charData[i] = charData[i] + 9;
    }
  }

  file = fopen("result2.txt", "w");
  fwrite(charData, 1, myDataFile.size, file);
  fclose(file);


  return 0;
}
