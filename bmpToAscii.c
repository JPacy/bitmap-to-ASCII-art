#include <stdio.h>
#include <stdlib.h>
#include "lenaArray.h"
#include "eindopdracht.h"

int main()
{
   char bmpFileName[31];
   
   programLena();
   
   printf("Input bmp filename (max 30 characters): \n");
   scanf(" %30s", bmpFileName);
   BitmapToAsciiArt(bmpFileName);
}
