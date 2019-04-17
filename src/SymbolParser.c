#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "SymbolParser.h"
#include "ASMParser.h"
 
#define BASE_ADDRESS 0x0000200

Symbol* initSymbol(char* pLabel, char* pType, char* pData, char* pAddress, int addr);
char* stripData(char* const buf);
void makeDataRaw(Symbol* sym);
static char* toBinary(int num, int size);
Symbol* parseDataSymbols(FILE* f, Symbol* tail);
Symbol* parseTextSymbols(FILE* f, Symbol* tail);

void cleanSymbols(Symbol* sym)
{
   while(sym)
   {
      Symbol* next = sym->next;
      sym->size = 0;
      free(sym->Label);
      free(sym->Type);
      free(sym->data);
      free(sym->raw);
      free(sym->Address);
      free(sym);
      sym = next;
   }
}

Symbol* initSymbol(char* pLabel, char* pType, char* pData, char* pAddress, int addr)
{
   Symbol* res = calloc(1, sizeof(Symbol));
   /*res->Label = calloc(50, sizeof(char));
   res->Type = calloc(9, sizeof(char));
   res->data = calloc(500, sizeof(char));*/
   res->Label = pLabel;
   res->Type = pType;
   res->data = pData;
   res->Address = pAddress;
   res->address = addr;
   res->size = 0;
   res->raw = NULL;
   res->next = NULL;

   //strncpy(res->Label, pLabel, 50);
   //strncpy(res->Type, pType, 9);
   //strncpy(res->data, pData, 500);

   return res;
}

Symbol* parseSymbols(FILE* f)
{
   Symbol* head = calloc(1, sizeof(Symbol));
   Symbol* tail = head;
   head->size = 0;
   head->Label = calloc(5, sizeof(char));
   strcpy(head->Label, "HEAD");
   rewind(f);
   
   printf("\tParsing data symbols...\n");
   tail = parseDataSymbols(f, tail);
   
   printf("\tParsing text symbols...\n");
   tail = parseTextSymbols(f, tail);

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

Symbol* parseTextSymbols(FILE* f, Symbol* tail)
{  
   static uint32_t textAddr = 0x0000;
   bool textParsing = false;
   rewind(f);
   char buf[555];

   while(fgets(buf, 555, f))
   { 
      char* temp = calloc(100, sizeof(char));
      sscanf(buf, "%s", temp);
      if(strncmp(".data", temp, 5) == 0)
      {
         textParsing = false;
         free(temp);
         continue;
      }
      else if(strncmp(".text", temp, 5) == 0)
      {
         textParsing = true;
         free(temp);
         continue;
      }
      if(textParsing)
      {
         if(strlen(buf) > 1)
         {
            Symbol* cur;
            char* label = calloc(50, sizeof(char));
            char* type = calloc(9, sizeof(char));
            strcpy(type, ".text");
            printf("\tScanning label...\n");
            sscanf(buf, "%s", label);
   
            int bufLen = strlen(buf)-1;
            if(buf[bufLen-1] == ':')
            {
               printf("\tLBL: %s\n", label);
               char* address = toBinary(textAddr, 32);
               label = strtok(label, ":");
               cur = initSymbol(label, type, NULL, address, textAddr);
               tail->next = cur;
               tail = cur;
               printf("%s", buf);
            }
            else if (bufLen > 0)
            {
               printf("\t%X -> %s\n", textAddr, buf);
               textAddr+=4;
               free(label);
               free(type);
            }
         }
      }

      free(temp);
   }

   return tail;
}

Symbol* parseDataSymbols(FILE* f, Symbol* tail)
{
   static uint32_t dataAddr = 0x2000;
   bool dataParsing = false;
   rewind(f);
   char buf[555];
   
   while(fgets(buf, 555, f))
   { 
      char* temp = calloc(100, sizeof(char));
      sscanf(buf, "%s", temp);
      if(strncmp(".data", temp, 5) == 0)
      {
         dataParsing = true;
         free(temp);
         continue;
      }
      else if(strncmp(".text", temp, 5) == 0)
      {
         dataParsing = false;
         free(temp);
         continue;
      }
      if(dataParsing)
      {
         if(strlen(buf) > 1)
         {
            Symbol* cur;
            char* label = calloc(50, sizeof(char));
            char* type = calloc(9, sizeof(char));
            char* data;
            char* address = toBinary(dataAddr, 32);
            
            sscanf(buf, "%s %s", label, type);
            data = stripData(buf);
            label = strtok(label, ":");
            cur = initSymbol(label, type, data, address, dataAddr);
            
            makeDataRaw(cur);
            printf("%X %d %x\n", dataAddr, cur->size, cur->size*4);
            dataAddr += (cur->size)*4;
            
            tail->next = cur;
            tail = cur;
            printf("%s", buf);
            //printSymbol(cur);
         }
      } 
      free(temp);
   }

   return tail;

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
   char* raw;// = calloc(32, sizeof(char));
   if (strncmp(".asciiz", sym->Type, 7) == 0)
   {
      char* temp = sym->data;
      int size = strlen(temp)-2;
      int rows = ((size*8)/32)+1;
      //rows = (rows == 0) ? 1 : rows;
      int rawlength = (rows*33)+1;
      raw = calloc(rawlength, sizeof(char));
      int endIndex = -1;
      for(int i = 1; i < size; i++)
      {
         char* binRes;
         if( i == size || temp[i] == '\n' || temp[i] == '\"')
         {
            binRes = "00000000";
            strcat(raw, binRes);
         }
         else
         {
            binRes = toBinary((int)temp[i], 8);
            strcat(raw, binRes);
            free(binRes);
         }

         //printf("%c -> %s\n",temp[i], binRes);
         if(i%4==0)
         {
            strcat(raw, "\n");
         }
         endIndex = i;
      }
      //Pad the remaining data 
      for(int i = endIndex; i % 4 != 0; i++)
      {
         if(i%4==3)
         {
            strcat(raw, "00000000\n");
         }
         else
         {
            strcat(raw, "00000000");
         }
      }
      sym->size = rows;
   }
   else if (strncmp(".word", sym->Type, 5) == 0)
   {
      char* token;
      int* values = calloc(50, sizeof(int));
      char* temp = calloc(500, sizeof(char));
      strncpy(temp, sym->data, 500);
      
      token = strtok(temp, ", ");
      //printf("TOK %s\n", token);
      int count = 0;
      while(token != NULL)
      {
         values[count] = atoi(token);
         token = strtok(NULL, ", ");
         count++;
      }
      
      raw = calloc(count*34, sizeof(char));
      for(int i = 0; i < count; i++)
      {
         char* binRes = toBinary(values[i], 32);
         strcat(raw, binRes);
         strcat(raw, "\n");
         free(binRes);
      }
      sym->size = count;
      free(temp);
      free(values);
   }
   sym->raw = raw;
}


void printSymbol(Symbol* sym)
{
   printf("%s %s %08X -> %s\n%s\n", sym->Label, sym->Type, sym->address, sym->Address, sym->data);//, sym->raw);
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
