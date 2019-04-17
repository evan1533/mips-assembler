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

void parseInstructions(FILE* f, FILE* out);

int main(int argc, char** argv)
{
   char* inFile;
   char* outFile;

   if ( argc > 2 )
   {
      inFile = argv[1];
      outFile = argv[2];
   }
   else
   {
      printf("invocation: assemble <infile> <outfile> [-symbols]\n");
      return 1;
   }

   //printf("%s\n", inFile);
   FILE *in = fopen(inFile, "r");
   removeComments(in);
   FILE* clean = fopen("cleaned.asm", "r");
   fclose(in);

   FILE *out = fopen(outFile, "w");
   Symbol* res = parseSymbols(clean);
   
   parseInstructions(clean, out);  
   fclose(clean); 
   cleanSymbols(res);
   return 0;
}



void parseInstructions(FILE* f, FILE* out)
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
         if(isInstruction(temp))
         {
            ParseResult* res = parseASM(buf);
            //Output the machine instruction
            fprintf(out, "%s\n", res->Machine);
            //printf("%s", buf);
            clearResult(res);
            free(res);
         }
      }
      free(temp);
   }
   //printf("%s\n", buf);
   fprintf(out, "\n");
   fclose(out);
}

