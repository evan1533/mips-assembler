#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

void removeComments(FILE* f)
{
   char buf[555];
   FILE* out = fopen("cleaned.asm", "w");
   
   while(fgets(buf, 555, f))
   {
      bool writing = true;
      if(buf[0] == '\n')
      {
         continue;
      }
      bool blankLine = true;
      for(int i = 0; i < 555 && buf[i] != '\0'; i++)
      {
         if(!isspace(buf[i]))
         {
            blankLine = false;
         }
         if( buf[i] == '#' )
         {
            writing = false;
            if ( i != 0)
            {
               fputc( '\n', out);
            }
         }
         else if (buf[i] == EOF)
         {
            fputc( EOF, out);
            break;
         }
         if( writing && !blankLine )
         {
            fputc( buf[i], out);
         }

      }
   }
   fclose(out);
}
