#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "SymbolParser.h"

Symbol* initSymbol(char* pLabel, char* pType, char* pData);
void printSymbol(Symbol* sym);
char* stripData(char* const buf);
void makeDataRaw(Symbol* sym);
static char* toBinary(int num, int size);

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

Symbol* initSymbol(char* pLabel, char* pType, char* pData)
{
   Symbol* res = calloc(1, sizeof(Symbol));
   res->Label = calloc(50, sizeof(char));
   res->Type = calloc(9, sizeof(char));
   res->data = calloc(500, sizeof(char));
   res->raw = NULL;
   res->next = NULL;

   strncpy(res->Label, pLabel, 50);
   strncpy(res->Type, pType, 9);
   strncpy(res->data, pData, 500);

   return res;
}

Symbol* parseSymbols(FILE* f)
{
   Symbol* head = calloc(1, sizeof(Symbol));
   Symbol* tail = head;
   rewind(f);
   char buf[555];
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
            Symbol* cur;
            char* label = calloc(50, sizeof(char));
            char* type = calloc(9, sizeof(char));
            char* data;
            
            sscanf(buf, "%s %s", label, type);
                
            data = stripData(buf);
            cur = initSymbol(label, type, data);
            makeDataRaw(cur);
            tail->next = cur;
            tail = cur;
            //printf("%s", buf);
            //printSymbol(cur);
         }
      }
      free(temp);
   }

   Symbol* temp = head;
   while(temp!=NULL)
   {
      printSymbol(temp);
      temp = temp->next;
      printf("\n");
   }
   //printf("%s\n", buf);
   //fprintf(out, "\n");
   //fclose(out);

   return head;
}


char* stripData(char* const buf)
{
   char* res = calloc(500, sizeof(char));
   char* temp = calloc(500, sizeof(char));
   strncpy(temp, buf, 500);
   
   char* delim = " ";
   char* token;

   token = strtok(temp, delim);
   int numStripped = 1;
   while (token != NULL)
   {
      if (numStripped > 2)
      {
         strcat(res, token);
         strcat(res, " ");
      }

      token = strtok(NULL, delim);
      numStripped++;
   }

   free(temp);
   return res;
}

void makeDataRaw(Symbol* sym)
{
   char* raw = calloc(32, sizeof(char));
   if (strncmp(".asciiz", sym->Type, 7) == 0)
   {
      char* temp = sym->data;
      for(int i = 1; temp[i] != '\"'; i++)
      {
         char* binRes = toBinary((int)temp[i], 8);
         printf("%c -> %s\n",temp[i], binRes);
      }
   }
}
void printSymbol(Symbol* sym)
{
   printf("%s %s %s %s\n", sym->Label, sym->Type, sym->data, sym->raw);
}

static char* toBinary(int num, int size)
{
	char* res = calloc(size + 1, sizeof(char));
	int temp = num;

	if(num < 0)
	{
		temp = (num*-1)-1;
	}

	for(int j = 0; j < size; j++)
	{
		*(res+j) = (num < 0) ? '1' : '0';
	}

	int i = 0;
	while(temp > 0)
	{
		*(res+(size-1-i)) = (num < 0) ? '1'-temp%2 : '0'+temp%2;
		temp = temp >> 1;
		i++;
	}
	return res;
}
