#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "ASMParser.h"
#include "ParseResult.h"

FILE* removeComments(FILE* f);
void parseInstructions(FILE* f);
void printRes(FILE* out, ParseResult* res);

int main(int argc, char** argv)
{
   char* inFile, outFile;

   if ( argc >= 3 )
   {
      inFile = argv[1];
      //outFile = argv[2];
   }
   else
   {
      return 1;
   }

   printf("%s\n", inFile);
   FILE *in = fopen(inFile, "r");
   FILE* nocIn = removeComments(in);
   fclose(in);
   
   parseInstructions(nocIn);  
   fclose(nocIn); 
   return 0;
}


FILE* removeComments(FILE* f)
{
   char* buf = calloc(555, sizeof(char));
   FILE* out = fopen("nocomments.txt", "rw");
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
      free(buf);
      buf = calloc(555, sizeof(char));
   }
   return out;
}

void parseInstructions(FILE* f)
{
   FILE* out = fopen("myAns.o", "w");
   rewind(f);
   //FILE fp = fopen("nocomments.txt", "r");
   char* buf = calloc(555, sizeof(char));
   //FILE* out = fopen("nocomments.txt", "w");
   bool parsing = false; 
   while(fgets(buf, 555, f))
   { 
      char* temp = calloc(100, sizeof(char));
      sscanf(buf, "%s", temp);
      if(strncmp(".text", temp, 6) == 0)
      {
         parsing = true;
         continue;
      }
      if(parsing)
      {
         if(isInstruction(temp))
         {
            ParseResult* res = parseASM(buf);
            printRes(out, res);
            //printf("%s", buf);
         }
      }
      free(buf);
      buf = calloc(555, sizeof(char));
   }
   printf("%s\n", buf);
   fclose(out);
}

void printRes(FILE* out, ParseResult* pPR)
{
   fprintf(out, "%s%s%s%s%s%s\n", pPR->Opcode, pPR->RS, pPR->RT, pPR->RD, "00000", pPR->Funct);
/*      printf( "%s\n", pPR->ASMInstruction);
      printf( "   %s   %s\n", pPR->Opcode, pPR->Mnemonic);
      printf( "   %2"PRIu8"   %s", pPR->rd, pPR->rdName);
      if ( pPR->RD != NULL ) {
			printf( "   %s", pPR->RD);
		}
		printf( "\n");
      printf( "   %2"PRIu8"   %s", pPR->rs, pPR->rsName);
      if ( pPR->RS != NULL ) {
			printf( "   %s", pPR->RS);
		}
		printf( "\n");
      printf( "   %2"PRIu8"   %s", pPR->rt, pPR->rtName);
      if ( pPR->RT != NULL ) {
			printf( "   %s", pPR->RT);
		}
		printf( "\n");
      printf( "   %s\n", pPR->Funct);
      printf( "   %"PRId16"\n", pPR->Imm);
      if ( pPR->IMM != NULL ) {
			printf( "   %s", pPR->IMM);
		}
		printf( "\n");
		printf( "\n");*/
}
