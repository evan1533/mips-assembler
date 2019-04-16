#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "ASMParser.h"
#include "ParseResult.h"

void removeComments(FILE* f);
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

   printf("%s\n", inFile);
   FILE *in = fopen(inFile, "r");
   removeComments(in);
   FILE* clean = fopen("cleaned.asm", "r");
   fclose(in);
   FILE *out = fopen(outFile, "w");
   parseInstructions(clean, out);  
   fclose(clean); 
   return 0;
}


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
   printf("%s\n", buf);
   fprintf(out, "\n");
   fclose(out);
}

