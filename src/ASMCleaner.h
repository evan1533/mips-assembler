#ifndef ASMCLEANER_H
#define ASMCLEANER_H

void removeComments(FILE* f);
void replaceSymbols(FILE* f, Symbol* sym);

#endif
