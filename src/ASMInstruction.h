#ifndef ASMINSTRUCTION_H
#define ASMINSTRUCTION_H
#include <stdio.h>

struct _ASMInstruction {
   char* Mnemonic;
   char* Opcode;
   char* Funct;
};

typedef struct _ASMInstruction ASMInstruction;
#endif
