#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "SymbolParser.h"

void cleanSymbols(Symbol* sym)
{
   while(sym)
   {
      Symbol* next = sym->next;
      free(sym->Label);
      free(sym->Type);
      free(sym->data);
      free(sym->raw);
      free(sym);
      sym = next;
   }
}

Symbol* parseSymbols(FILE* f)
{
   Symbol* head = calloc(1, sizeof(Symbol));
   rewind(f);
   //FILE fp = fopen("nocomments.txt", "r");
   char buf[555];
   //FILE* out = fopen("nocomments.txt", "w");
   bool parsing = false; 
   while(fgets(buf, 555, f))
   { 
      char* temp = calloc(100, sizeof(char));
      sscanf(buf, "%s", temp);
      if(strncmp(".data", temp, 5) == 0)
      {
         parsing = true;
         free(temp);
         continue;
      }
      if(parsing)
      {
         if(strlen(buf) > 4)
         {
            printf("%s", buf);
         }
      }
      free(temp);
   }
   //printf("%s\n", buf);
   //fprintf(out, "\n");
   //fclose(out);

   return head;
}
