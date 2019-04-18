#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#include "ASMParser.h" 
#include "SymbolParser.h"

static int getLabelAddress(char* label, Symbol* sym);

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
   FILE* out = fopen("symreplaced.txt", "w");
   bool parsing = false; 
   while(fgets(buf, 555, f))
   {
      char* temp = calloc(100, sizeof(char));
      sscanf(buf, "%s", temp);
      if(strncmp(".text", temp, 6) == 0)
      {
         parsing = true;
         fprintf(out, "%s", buf);
         free(temp);
         continue;
      }
      if(parsing)
      {
         if(isLabelInstruction(temp))
         {
            if( strcmp(temp, "beq") == 0 || strcmp(temp, "bne") == 0)
            {
               char mnem[6];
               char* reg1 = calloc(6, sizeof(char));
               char* reg2 = calloc(6, sizeof(char));
               char label[33];
               //printf("\tLABL: %s\n", temp);
               sscanf(buf, "%s %s %s %s", mnem, reg1, reg2, label);
               //reg1 = strtok(reg1, ",");
               //reg2 = strtok(reg2, ",");
               int lblAddr = getLabelAddress(label, sym);
               fprintf(out, "%s %s %s %d\n", mnem, reg1, reg2, lblAddr);
               free(reg1);
               free(reg2);
            }
            else
            {
               char mnem[6];
               char* reg = calloc(6, sizeof(char));
               char label[33];
               //printf("\tLABL: %s\n", temp);
               sscanf(buf, "%s %s %s", mnem, reg, label);
               //reg = strtok(reg, ",");
               int lblAddr = getLabelAddress(label, sym);
               fprintf(out, "%s %s %d\n", mnem, reg, lblAddr);
               free(reg);
               //Output the machine instruction
               //printf("%s", buf);
            }
         }
         else
         {
            fprintf(out, "%s", buf);
         }
      }
      else
      {
         fprintf(out, "%s", buf);
      }
      free(temp);
   }
   //printf("%s\n", buf);
   //fprintf(out, "\n");
   fclose(out);
}


static int getLabelAddress(char* label, Symbol* sym)
{
   sym = sym->next;
   while( sym != NULL )
   {
      if( strcmp(sym->Label, label) == 0)
      {
         return sym->address;
      }
      sym = sym->next;
   }
   return -1;
}

