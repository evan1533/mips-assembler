#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void removeComments(FILE* f)
{
   char buf[555];
   FILE* out = fopen("cleaned.asm", "w");
   printf("Removing comments\n");
   
   while(fgets(buf, 555, f))
   {
      bool writing = true;
      for(int i = 0; i < 555 && buf[i] != '\0'; i++)
      {
         if( buf[i] == '#' )
         {
            writing = false;
            fputc( '\n', out);
         }
         else if (buf[i] == EOF)
         {
            fputc( EOF, out);
            break;
         }
         if( writing )
         {
            fputc( buf[i], out);
         }

      }
   }
   fclose(out);
}
