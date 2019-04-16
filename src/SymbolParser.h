#ifndef SYMBOLPARSER_H
#define SYMBOLPARSER_H

typedef enum {WORD, ASCIIZ} DataType;

typedef struct {
   char* Label;
   char* Type;
   char* data;
   char* raw;
   Symbol* next;
} Symbol;


void cleanSymbols(Symbol* sym);
Symbol* parseSymbols(FILE* f);

#endif
