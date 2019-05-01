// On my honor:
//
// - I have not discussed the C language code in my program with
// anyone other than my instructor or the teaching assistants
// assigned to this course.
//
// - I have not used C language code obtained from another student,
// the Internet, or any other unauthorized source, either modified
// or unmodified.
//
// - If any C language code or documentation used in my program
// was obtained from an authorized source, such as a text book or
// course notes, that has been clearly noted with a proper citation
// in the comments of my program.
//
// - I have not designed this program in such a way as to defeat or
// interfere with the normal operation of the Curator System.
//
//
// Evan Ott
// evan1533

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "SymbolParser.h"
#include "ASMParser.h"
#include "ParseResult.h"
#include "ASMCleaner.h"

void parseInstructions(FILE* f, FILE* out, Symbol* sym);
void writeSymbols(FILE* out, Symbol* sym);
void createSymbolTable(FILE* out, Symbol* sym);

int main(int argc, char** argv)
{
   char* inFile;
   char* outFile;
   char* flag;
   bool genTable = false;

   if ( argc > 2 )
   {
      inFile = argv[1];
      outFile = argv[2];
      if(argc == 4)
      {
         flag = argv[3];
         
         if(strncmp(flag, "-symbols", 8) == 0)
         {
            genTable = true;
         }
         else
         {
            printf("Unrecognized option %s\n", flag);
            return 1;
         }
      }
   }
   else
   {
      printf("invocation: assemble <infile> <outfile> [-symbols]\n");
      return 1;
   }
   
   //printf("%s\n", inFile);
   FILE *in = fopen(inFile, "r");
   printf("Removing comments...\n");
   removeComments(in);
   FILE* clean = fopen("cleaned.asm", "r");
   fclose(in);

   FILE *out = fopen(outFile, "w");
   printf("Parsing symbols...\n");
   Symbol* res = parseSymbols(clean);
     /*Symbol* temp = res->next;
     while(temp != NULL)
     {
         printSymbol(temp);
         printf("\n");
         temp = temp->next;
      }*/

   if(genTable)
   {
      createSymbolTable(out, res);
   }
   else
   {
      printf("Rewriting symbols...\n");
      replaceSymbols(clean, res);
      
      printf("Parsing instructions...\n");
      FILE* repfile = fopen("symreplaced.txt", "r");
      parseInstructions(repfile, out, res);
      
      printf("Writing symbols...\n");  
      writeSymbols(out, res); 
      fclose(repfile);
      /*Symbol* temp = res->next;
      while(temp != NULL)
      {
         printSymbol(temp);
         printf("\n");
         temp = temp->next;
      }*/
   }

   fclose(clean); 
   printf("Freeing symbol list...\n");
   cleanSymbols(res);
   fclose(out);
   printf("Done!\n");
   return 0;
}



void parseInstructions(FILE* f, FILE* out, Symbol* sym)
{
   rewind(f);
   //FILE fp = fopen("nocomments.txt", "r");
   char buf[555];
   //FILE* out = fopen("nocomments.txt", "w");
   bool parsing = false;
   //printf("\tPASINg TIME\n"); 
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
         if(isInstruction(temp))
         {
            ParseResult* res = parseASM(buf);
            //Output the machine instruction
            fprintf(out, "%s\n", res->Machine);
            printf("\t%s %s\n", res->Mnemonic, res->Machine);
            clearResult(res);
            free(res);
         }
      }
      free(temp);
   }
   //printf("%s\n", buf);
   fprintf(out, "\n");
}

void createSymbolTable(FILE* out, Symbol* sym)
{
   Symbol* temp;
   temp = sym->next;
   while(temp!=NULL)
   {
      fprintf(out, "0x%08X:  %s\n", temp->address, temp->Label);
      temp = temp->next;
   }
}

void writeSymbols(FILE* out, Symbol* sym)
{
   Symbol* temp;
   temp = sym->next;
   while(temp!=NULL && out)
   {
      if(strcmp(temp->Type, ".text") != 0)
      {
         fprintf(out, "%s", temp->raw);
      }
      temp = temp->next;
   }
}
