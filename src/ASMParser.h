#ifndef ASMPARSER_H
#define ASMPARSER_H
#include "ParseResult.h"
#include <stdbool.h>

#define NUM_REGISTERS 32
#define NUM_INSTRUCTIONS 24

typedef struct _MIPSInstruction {
   char* mnemonic;
   char* opcode;
   char* funct;
} MIPSInstruction;

static char* registerTable[NUM_REGISTERS] = {
	"zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3",
	 "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7",
	 "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7",
	 "$t8", "$t9", "$k0", "$k1", "$gp", "$sp", "$fp", "$ra"};
/*
static char* instructionTable[NUM_INSTRUCTIONS] = {"add", "and", "sub", "addi", "andi", "lui", "lw", "sw",
                                                  "addu", "addiu", "mul", "nop", "nor", "sll", "slt", "slti",
                                                   "sra", "srav", '"sub", "beq", "blez", "bgtz", "bne", "j",
                                                   "syscall"};
static char* opcodeTable[NUM_INSTRUCTIONS] = {"000000", "000000", "000000", "001000", "001100", "001111", "100011", "101011", 
                                              "000000", "001001", "000000", "000000", "000000", "000000", "000000", "001010",
                                              "000000"};
static char* functionTable[NUM_INSTRUCTIONS] = {"100000", "100100", "100010",  NULL,     NULL,     NULL,     NULL,   NULL,
                                                "100001",   NULL,   "011000", "000000", "100111", "000000", "101010", NULL,
                                                "000011"};
*/    
static MIPSInstruction mipsTable[NUM_INSTRUCTIONS] = {
   {"add",    "000000", "100000"},
   {"and",    "000000", "100100"},
   { "sub",   "000000", "100010"}, 
   {"addi",   "001000",   NULL  },
   {"andi",   "001100",   NULL  },
   {"lui",    "001111",   NULL  },
   {"lw",     "100011",   NULL  },
   {"sw",     "101011",   NULL  },
   {"addu",   "000000", "100001"},
   {"addiu",  "001001",   NULL  },
   {"mul",    "000000", "011000"},
   {"nop",    "000000", "000000"},
   {"nor",    "000000", "100111"},
   {"sll",    "000000", "000000"},
   {"slt",    "000000", "101010"},
   {"slti",   "001010",   NULL  },
   {"sra",    "000000", "000011"},
   {"srav",   "000000", "000111"},
   {"beq",    "000100",   NULL  },
   {"blez",   "000110",   NULL  },
   {"bgtz",   "000111",   NULL  },
   {"bne",    "000101",   NULL  },
   {"j",      "000010",   NULL  },
   {"syscall","000000", "001100"} };

/**  Breaks up given MIPS32 assembly instruction and creates a proper 
 *   ParseResult object storing information about that instruction.
 * 
 *   Pre:  pASM points to an array holding the bits (as chars) of a
 *         syntactically valid assembly instruction, whose mnemonic is
 *         one of the following:
 *             add  addi  and  andi  lui  lw  or  ori  sub
 * 
 *   Returns:
 *         A pointer to a proper ParseResult object whose fields have been
 *         correctly initialized to correspond to the target of pASM.
 */
ParseResult* parseASM(const char* const pASM);

bool isInstruction(char* s);



#endif
