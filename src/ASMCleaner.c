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
   rewind(f);
   char buf[555];
   FILE* out = fopen("cleaned.asm", "w");
   bool dataParsing = false;
   
   while(fgets(buf, 555, f))
   {
      if(buf[0] == '\n')
      {
         continue;
      }
      char* firstWord = calloc(256, sizeof(char));

      sscanf(buf, "%s", firstWord);

      if(strncmp(firstWord, ".data", 5) == 0)
      {
         dataParsing = true;
         fputs(buf, out);
         continue;
      }
      else if(strncmp(firstWord, ".text", 5) == 0)
      {
         dataParsing = false;
         fputs(buf, out);
         continue;
      }

      if(!dataParsing)
      {
         if(firstWord[0] != '#' && firstWord[0] != '\0')
         {
                  printf("BBB: %s\n", firstWord);
            if(true)
            {

               printf("\tFAS: %s\n", firstWord);
               if ( strncmp(firstWord, "la\0", 3) == 0)
               {
                  printf("\t\tFAS: %s\n", firstWord);
                  char* arg1 = calloc(33, sizeof(char));
                  char* arg2 = calloc(33, sizeof(char));
                  sscanf(buf, "%*s %s %s", arg1, arg2);
                  printf("\t%s %s %s\n", firstWord, arg1, arg2);
                  
                  fprintf(out, "\t addi %s $zero, %s\n", arg1, arg2);
                  
                  free(arg1);
                  free(arg2);
               }
               else if ( strncmp(firstWord, "li", 2) == 0)
               {
                  char* arg1 = calloc(33, sizeof(char));
                  char* arg2 = calloc(33, sizeof(char));
                  sscanf(buf, "%*s %s %s", arg1, arg2);
                  printf("\t%s %s %s\n", firstWord, arg1, arg2);
                  
                  fprintf(out, "\t addiu %s $zero, %s\n", arg1, arg2);
                  
                  free(arg1);
                  free(arg2);
               }
               else if( strncmp(firstWord, "nop", 3) == 0)
               {
                  fprintf(out, "\t sll $zero, $zero, 0\n");
               }
               else if(strcmp(firstWord, "lw") == 0)
               {
                  bool hasLabel = true;
                  for(int i = 0; i < 256; i++)
                  {
                     if(buf[i] == '(')
                     {
                        hasLabel = false;
                     }
                  }
                  if(hasLabel)
                  {
                     char* reg = calloc(10, sizeof(char));
                     char* label = calloc(33, sizeof(char));
                     sscanf(buf, "%*s %s %s", reg, label);
                     fprintf(out, "\tlw %s %s($zero)\n", reg, label);
                     free(reg);
                     free(label);
                  }
                  else
                  {
                     fputs(buf, out);
                  }
               }
               else if(strcmp(firstWord, "move") == 0)
               {
                  char* arg1 = calloc(33, sizeof(char));
                  char* arg2 = calloc(33, sizeof(char));
                  sscanf(buf, "%*s %s %s", arg1, arg2);
                  printf("\t%s %s %s\n", firstWord, arg1, arg2);
                  
                  fprintf(out, "\t addu %s $zero, %s\n", arg1, arg2);
                  
                  free(arg1);
                  free(arg2);

               }
               else if(strcmp(firstWord, "blt") == 0)
               {
                  //slt $at, $rs, $rt
                  //bne $at, $zero, offset
                  char rs[33];
                  char rt[33];
                  char offset[33];
                  sscanf(buf, "%*s %s %s %s", rs, rt, offset);
                  printf("\t%s %s %s\n", firstWord, rs, rt);
                  
                  strtok(rt, ",");
                  fprintf(out, "\t slt $at, %s %s\n", rs, rt);
                  fprintf(out, "\t bne $at, $zero, %s\n", offset);

               }
               else 
               {
                  fputs(buf, out);
               }
            }
            else 
            {
               fputs(buf, out);
            }
         }

         free(firstWord);
      }
      else
      {
         bool blankLine = true;
         bool writing = true;

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
               printf("Not blank: %s\n", buf);
            }
            
            if( writing && !blankLine )
            {
               fputc( buf[i], out);
            }

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
         printf("%s\n", temp);
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
               int relativeAddr = (tempSym->address - (curAddr+4))/4;
               printf("\t%s %s %d %d %d\n",mnem, label, curAddr, tempSym->address, relativeAddr);
               fprintf(out, "\t %s %s %s %d\n", mnem, reg1, reg2, relativeAddr);
            }
            else if( strcmp(temp, "blez") == 0 || strcmp(temp, "bgtz") == 0)
            {
               char mnem[8];
               char reg1[8];
               char label[33];
               //printf("\tLABL: %s\n", temp);
               sscanf(buf, "%s %s %s", mnem, reg1, label);
               //reg1 = strtok(reg1, ",");
               //reg2 = strtok(reg2, ",");
               Symbol* tempSym = getSymbol(label, sym);
               int relativeAddr = (tempSym->address - (curAddr+4))/4;
               printf("\t%s %s %d %d %d\n",mnem, label, curAddr, tempSym->address, relativeAddr);
               fprintf(out, "\t %s %s %d\n", mnem, reg1, relativeAddr);
            }
            else if( strcmp(temp, "addi") == 0)
            {
               char mnem[8];
               char reg1[8];
               char reg2[8];
               char label[33];
               //printf("\tLABL: %s\n", temp);
               sscanf(buf, "%s %s %s %s", mnem, reg1, reg2, label);
               //reg1 = strtok(reg1, ",");
               //reg2 = strtok(reg2, ",");
               int lblAddr = getLabelAddress(label, sym);
               if(lblAddr != -1)
               {
                  printf("\t %s %s %s %d\n", mnem, reg1, reg2, lblAddr);
                  fprintf(out, "\t %s %s %s %d\n", mnem, reg1, reg2, lblAddr);
               }
               else
               {
                  fprintf(out, "%s", buf);
               }
            }
            else if(strcmp(temp, "j") == 0)
            {
               char mnem[8];
               char label[33];
               //printf("\tLABL: %s\n", temp);
               sscanf(buf, "%s %s", mnem, label);
               //reg1 = strtok(reg1, ",");
               //reg2 = strtok(reg2, ",");
               Symbol* tempSym = getSymbol(label, sym);
               printSymbol(tempSym);
               
               printf("\t%s %s %d %d\n",mnem, label, curAddr, (tempSym->address)>>2);
               fprintf(out, "\t%s %d\n", mnem, (tempSym->address)>>2);

               printf("\t%s %d\n", mnem, (tempSym->address)>>2);
            }
            else if(strcmp(temp, "lw") == 0)
            {
               char mnem[8];
               char reg1[8];
               char offs[64];
               //printf("\tLABL: %s\n", temp);
               sscanf(buf, "%s %s %s", mnem, reg1, offs);
               //reg1 = strtok(reg1, ",");
               //reg2 = strtok(reg2, ",");
               char* label = strtok(offs, "(");
               char* offset = strtok(NULL, ")");
               printf("GGG: %s %s\n", label, offset);
               int lblAddr = getLabelAddress(label, sym);
               if(lblAddr != -1)
               {
                  printf("\t %s %s %d(%s)\n", mnem, reg1, lblAddr, offset);
                  fprintf(out, "\t %s %s %d(%s)\n", mnem, reg1, lblAddr, offset);
               }
               else
               {
                  fprintf(out, "%s", buf);
               }
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
               fprintf(out, "\t %s %s %d\n", mnem, reg, lblAddr);
               //Output the machine instruction
               //printf("%s", buf);
            }

         }
         else
         {
            fprintf(out, "%s", buf);
         }

         if(isInstruction(temp))
         {
            curAddr+=4;
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
