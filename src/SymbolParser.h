#ifndef SYMBOLPARSER_H
#define SYMBOLPARSER_H

typedef enum {WORD, ASCIIZ} DataType;

struct _Symbol {
   char* Label;
   char* Type;
   char* data;
   char* raw;
   struct _Symbol* next;
};

typedef struct _Symbol Symbol;


void cleanSymbols(Symbol* sym);
Symbol* parseSymbols(FILE* f);

#endif
