#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#include "ASMParser.h" 
#include "SymbolParser.h"

static int getLabelAddress(char* label, Symbol* sym);
static Symbol* getSymbol(char* label, Symbol* sym);

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
   int curAddr = 0;
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
         if(isInstruction(temp))
         {
            curAddr+=4;
         }
         if(isLabelInstruction(temp))
         {
            if( strcmp(temp, "beq") == 0 || strcmp(temp, "bne") == 0)
            {
               char mnem[8];
               char reg1[8];
               char reg2[8];
               char label[33];
               //printf("\tLABL: %s\n", temp);
               sscanf(buf, "%s %s %s %s", mnem, reg1, reg2, label);
               //reg1 = strtok(reg1, ",");
               //reg2 = strtok(reg2, ",");
               Symbol* tempSym = getSymbol(label, sym);
               int relativeAddr = (tempSym->address - curAddr)/4;
               //printf("\t%s %s %d %d\n",mnem, label, curAddr, relativeAddr);
               fprintf(out, "%s %s %s %d\n", mnem, reg1, reg2, relativeAddr);
            }
            else
            {
               char mnem[8];
               char reg[8];
               char label[33];
               //printf("\tLABL: %s\n", temp);
               sscanf(buf, "%s %s %s", mnem, reg, label);
               //reg = strtok(reg, ",");
               int lblAddr = getLabelAddress(label, sym);
               fprintf(out, "%s %s %d\n", mnem, reg, lblAddr);
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

static Symbol* getSymbol(char* label, Symbol* sym)
{
   sym = sym->next;
   while( sym != NULL )
   {
      if( strcmp(sym->Label, label) == 0)
      {
         return sym;
      }
      sym = sym->next;
   }
   return NULL;
}
