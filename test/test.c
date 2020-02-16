#include <stdio.h> 
#include <sys/types.h> 
#include <unistd.h> 

int main () {
   int i;
   int f1,f2,f3,f4,f5;
   f1 = fork();
   if (f1 == 0)
   {
      f2 = fork();
      if (f2 == 0)
      {
         printf("1");
         return 0;
      }
      else
      {
         wait(&i);
         f3 = fork();
         if (f3 == 0)
         {
            printf("2");
            return 0;
         }
         else
         {
            wait(&i);
            printf("3");
         }
      }
      return 0;
   }
   else
   {
      wait(&i);
      f4 = fork();
      if (f4 == 0)
      {
         printf("4");
         return 0;
      }
      else{
         wait(&i);
         printf("5");

      }
   }

   return 0;
}