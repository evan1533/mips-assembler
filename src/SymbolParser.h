#ifndef SYMBOLPARSER_H
#define SYMBOLPARSER_H
#include <inttypes.h>

typedef enum {WORD, ASCIIZ} DataType;

struct _Symbol {
   char* Label;
   char* Type;
   char* Address;
   int address;
   int size;
   char* data;
   int dataLength;
   int lastRowLength;
   char* raw;
   struct _Symbol* next;
};

typedef struct _Symbol Symbol;


void cleanSymbols(Symbol* sym);
Symbol* parseSymbols(FILE* f);
void printSymbol(Symbol* sym);

#endif
