#ifndef SYMBOLPARSER_H
#define SYMBOLPARSER_H
#include <inttypes.h>

typedef enum {WORD, ASCIIZ, TEXT} SymbolType;

struct _Symbol {
   char* Label;
   SymbolType Type;
   char* Address;
   int address;
   int size;
   int numRows;
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
