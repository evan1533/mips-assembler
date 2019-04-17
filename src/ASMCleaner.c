#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#include "ASMParser.h" 
#include "SymbolParser.h"

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
         
         if( buf[i] == '#' )
         {
            writing = false;
            if ( i != 0 && !blankLine)
            {
               fputc( '\n', out);
            }
         }
         else if (buf[i] == EOF)
         {
            fputc( EOF, out);
            break;
         }
         
         if(blankLine && !isspace(buf[i]))
         {
            blankLine = false;
         }
         
         if( writing && !blankLine )
         {
            fputc( buf[i], out);
         }

      }
   }
   fclose(out);
}


void replaceSymbols(FILE* f, Symbol* sym)
{
   rewind(f);
   //FILE fp = fopen("nocomments.txt", "r");
   char buf[555];
   //FILE* out = fopen("nocomments.txt", "w");
   bool parsing = false; 
   while(fgets(buf, 555, f))
   {
      char* temp = calloc(100, sizeof(char));
      sscanf(buf, "%s", temp);
      if(strncmp(".text", temp, 6) == 0)
      {
         parsing = true;
         free(temp);
         continue;
      }
      if(parsing)
      {
         if(isLabelInstruction(temp))
         {
            char mnem[3];
            char reg[4];
            char label[33];
            sscanf(buf, "%s %s %s", mnem, reg, label);
            fprintf(f, "%s %s %d\n", mnem, reg, 6969);
            //Output the machine instruction
            //printf("%s", buf);
         }
         else
         {
            fprintf(f, "%s", buf);
         }
      }
      else
      {
         fprintf(f, "%s", buf);
      }
      free(temp);
   }
   //printf("%s\n", buf);
   //fprintf(out, "\n");

}
