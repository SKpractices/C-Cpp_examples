#include <stdio.h>

int main () 
{

   /* a pointer to an int */
   
   int arr[3][4];
   for (int i = 0; i < 3; i++ ) 
   {
      for (int j = 0 ; j< 4; j++)
      {
        arr[i][j] = i*j;
      //printf( "r[%d][%d]: %d\n", i,j,*((*(p +i) + j));
      }
   }

for (int i = 0; i < 3; i++ ) 
   {
      for (int j = 0 ; j< 4; j++)
      {
      printf("%d",*((*(arr +i) + j)));
      }
   }
   return 0;
}
