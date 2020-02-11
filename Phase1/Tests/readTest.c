#include <stdio.h>
#include <stdlib.h>

int main()
{
   char ch, file_name[25];
   FILE *fp;

   fp = fopen("Zoinks.txt", "r"); // read mode

   if (fp == NULL)
   {
      perror("Error while opening.\n");
      exit(EXIT_FAILURE);
   }

   while((ch = fgetc(fp)) != EOF)
   {
     printf("%c", ch);
   }

   fclose(fp);
   return 0;
}
