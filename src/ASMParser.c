// On my honor:
//
// - I have not discussed the C language code in my program with
// anyone other than my instructor or the teaching assistants
// assigned to this course.
//
// - I have not used C language code obtained from another student,
// the Internet, or any other unauthorized source, either modified
// or unmodified.
//
// - If any C language code or documentation used in my program
// was obtained from an authorized source, such as a text book or
// course notes, that has been clearly noted with a proper citation
// in the comments of my program.
//
// - I have not designed this program in such a way as to defeat or
// interfere with the normal operation of the Curator System.
//
//
// Evan Ott
// evan1533

#include <inttypes.h>  // I used functions from these Standard modules in
#include <string.h>    // my solution; you may or may not need all of them,
#include <stdlib.h>    // and you might need additional ones, depending on
#include <stdio.h>     // your approach to the problem.
#include <assert.h>

#include "ASMParser.h"

#define NUM_REGISTERS 32
#define NUM_INSTRUCTIONS 26

static uint8_t findRegister(char* rName);
static char* findOpcode(char* inst);
static char* findFunct(char* inst);
static ParseResult* parseRType(const char* const pASM);
static ParseResult* parseIType(const char* const pASM);
static ParseResult* parseJump(const char* const pASM);
static char* toBinary(int num, int size); 

static char* registerTable[NUM_REGISTERS] = {
	"$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3",
	 "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7",
	 "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7",
	 "$t8", "$t9", "$k0", "$k1", "$gp", "$sp", "$fp", "$ra"};

static MIPSInstruction mipsTable[NUM_INSTRUCTIONS] = {
{"add",    "000000", "100000", false },
{"and",    "000000", "100100", false },
{"sub",    "000000", "100010", false }, 
{"addi",   "001000",   NULL  , true },
{"andi",   "001100",   NULL  , false },
{"lui",    "001111",   NULL  , false },
{"lw",     "100011",   NULL  , false },
{"sw",     "101011",   NULL  , false },
{"addu",   "000000", "100001", false },
{"addiu",  "001001",   NULL  , false },
{"mul",    "011100", "000010", false },
{"nop",    "000000", "000000", false },
{"nor",    "000000", "100111", false },
{"sll",    "000000", "000000", false },
{"slt",    "000000", "101010", false },
{"slti",   "001010",   NULL  , false },
{"sra",    "000000", "000011", false },
{"srav",   "000000", "000111", false },
{"beq",    "000100",   NULL  , true  },
{"blez",   "000110",   NULL  , true  },
{"bgtz",   "000111",   NULL  , true  },
{"bne",    "000101",   NULL  , true  },
{"j",      "000010",   NULL  , true  },
{"syscall","000000", "001100", false },
{"addiu",  "001001",   NULL  , false } };

/** Breaks up given the MIPS32 assembly instruction and creates a proper
 * ParseResult object storing information about that instruction.
 *
 * Pre: pASM points to an array holding the representation of a
 * syntactically valid assembly instruction, whose mnemonic is
 * one of the following:
 *
 * add addi and andi lui lw sub
 *
 * The instruction will be formatted as follows:
 *
 * <mnemonic><ws><operand1>,<ws><operand2>,<ws>...
 *
 * where <ws> is an arbitrary mixture of space and tab characters.
 *
 * Returns:
 * A pointer to a proper ParseResult object whose fields have been
 * correctly initialized to correspond to the target of pASM.
 */	
ParseResult* parseASM(const char* const pASM) {
   char* mnem = calloc(8, sizeof(char));
	char* temp = calloc(50, sizeof(char));
	strcpy(temp, pASM);
	sscanf(temp,"%s", mnem);
	char* opcode = findOpcode(mnem);
   printf("%s\n", pASM); 
   free(mnem);
   free(temp);

	if(strcmp(opcode,"000000") == 0 || strcmp(opcode, "011100") == 0)
	{
		return parseRType(pASM);
	}
   else if(strcmp(opcode, "000010") == 0)
   {
      return parseJump(pASM);
   }
	else 
	{
		return parseIType(pASM);
	}	
	return NULL;
}


static ParseResult* parseRType(const char* const pASM)
{
	ParseResult* res = malloc(sizeof(ParseResult));
	res->ASMInstruction = calloc(50, sizeof(char));
	strcpy(res->ASMInstruction, pASM);
	char* mnem = calloc(8,sizeof(char));
	char* arg1 = calloc(10,sizeof(char));
	char* arg2 = calloc(10,sizeof(char));
	char* arg3 = calloc(10,sizeof(char));

	res->Mnemonic = calloc(8, sizeof(char));
	res->rdName = calloc(7, sizeof(char));
	res->rsName = calloc(7, sizeof(char));
	res->rtName = calloc(7, sizeof(char));
	res->Imm = 0;
	res->rd = 255;
	res->rs = 255;
	res->rt = 255;
   res->shamt = 255;
	res->Opcode = calloc(7, sizeof(char));
	res->Funct = calloc(7, sizeof(char));
   res->Shamt = calloc(7, sizeof(char));
	res->RD = calloc(6, sizeof(char));
	res->RS = calloc(6, sizeof(char));
	res->RT = calloc(6, sizeof(char));
	res->IMM = NULL;
   res->Machine = calloc(33, sizeof(char));
	
	sscanf(pASM,"%s %s %s %s", mnem, arg1, arg2, arg3);
   arg1 = strtok(arg1, ",");
   arg2 = strtok(arg2, ",");
   
   if( strncmp(mnem, "syscall", 7) == 0)
   {
      strcpy(res->Mnemonic, mnem);
      strcpy(res->Opcode, "000000");
      strcpy(res->Funct, findFunct(mnem));
      strcpy(res->Shamt, "00000");
      strcpy(res->RS, "00000");
      strcpy(res->RT, "00000");
      strcpy(res->RD, "00000");
   }
   else if( strncmp(mnem, "sll", 3) == 0)
   {
      strcpy(res->Mnemonic, mnem);
      strcpy(res->Opcode, findOpcode(mnem));
      strcpy(res->Funct, findFunct(mnem));
      strcpy(res->rdName, arg1);
      strcpy(res->rtName, arg2);
      res->rd = findRegister(arg1);
      res->rt = findRegister(arg2);

      char* shamBin = toBinary(atoi(arg3), 5);
      strncpy(res->Shamt, shamBin, 5);
      printf("\t %d %d %d\n", res->rs, res->rt, res->rd);
	
      char* rdBin = toBinary(res->rd, 5);
      strcpy(res->RD, rdBin);
      free(rdBin);

      strcpy(res->RS, "00000");

      char* rtBin = toBinary(res->rt, 5);
      strcpy(res->RT, rtBin);
      free(rtBin);
   }
   else
   {
      strcpy(res->Mnemonic, mnem);
      strcpy(res->Opcode, findOpcode(mnem));
      strcpy(res->Funct, findFunct(mnem));
      strcpy(res->Shamt, "00000");
      strcpy(res->rdName, arg1);
      strcpy(res->rsName, arg2);
      strcpy(res->rtName, arg3);
      res->rd = findRegister(arg1);
      res->rs = findRegister(arg2);
      res->rt = findRegister(arg3);
	
      char* rdBin = toBinary(res->rd, 5);
      strcpy(res->RD, rdBin);
      free(rdBin);

      char* rsBin = toBinary(res->rs, 5);
      strcpy(res->RS, rsBin);
      free(rsBin);

      char* rtBin = toBinary(res->rt, 5);
      strcpy(res->RT, rtBin);
      free(rtBin);

   }

   //Build the machine code instruction
   char* machine = calloc(33, sizeof(char));
   strncat(machine, res->Opcode, 6);
   strncat(machine, res->RS, 5); 
   strncat(machine, res->RT, 5);
   strncat(machine, res->RD, 5); 
   strncat(machine, res->Shamt, 5);
   strncat(machine, res->Funct, 6);
   strncpy(res->Machine, machine, 32);

   free(machine);
	free(arg1);
	free(arg2);
	free(arg3);
	free(mnem);
	return res;
}

static ParseResult* parseIType(const char* const pASM)
{
	ParseResult* res = malloc(sizeof(ParseResult));

	res->ASMInstruction = calloc(50, sizeof(char));
   strcpy(res->ASMInstruction, pASM);
	char* mnem = calloc(6,sizeof(char));
	char* arg1 = calloc(10,sizeof(char));
	int16_t imm = 0;
	sscanf(pASM, "%s %s ", mnem, arg1);
   printf("\t%s\n", arg1);
   arg1 = strtok(arg1, ",");

	//Set all fields to default unused value, then fill in the fields that we use as
	//we go along, thus in the end only the unused fields will still have the default value
	res->Mnemonic = calloc(6, sizeof(char));
	res->rdName = NULL;
	res->rsName = NULL;
	res->rtName = calloc(6, sizeof(char));
	res->Imm = 0;
	res->rd = 255;
	res->rs = 255;
	res->rt = 255;
   res->shamt = 255;
	res->Opcode = calloc(7, sizeof(char));
	res->Funct = NULL;
   res->Shamt = NULL;
	res->RD = NULL;
	res->RS = NULL;
	res->RT = calloc(6, sizeof(char));
	res->IMM = calloc(17, sizeof(char));
   res->Machine = calloc(33, sizeof(char));

	strcpy(res->Mnemonic, mnem);
	strcpy(res->Opcode, findOpcode(mnem));

	strcpy(res->rtName, arg1);
	res->rt = findRegister(arg1);
	char* rtBin = toBinary(res->rt, 5);
	strncpy(res->RT, rtBin, 5);
	free(rtBin);


   if(strcmp(mnem, "lui") == 0)
	{
		sscanf(res->ASMInstruction, "%*3s %*4s %"SCNd16"", &imm);
		
		res->Imm = imm;
		char* immBin = toBinary(imm, 16);
		strcpy(res->IMM, immBin);
		free(immBin);
		
		res->rs = 0;
		res->RS = calloc(7, sizeof(char));
		strcpy(res->RS, "00000");
	}
	else if(strcmp(mnem, "lw") == 0 || strcmp(mnem, "sw") == 0)
	{
	   char* arg2 = calloc(5,sizeof(char));
      char* temp = calloc(55, sizeof(char));
      strcpy(temp, pASM);
		sscanf(temp, "%*2s %*4s %"SCNd16"(%3s)", &imm, arg2);
	
		res->rsName = calloc(5, sizeof(char));
		strcpy(res->rsName, arg2);
		res->rs = findRegister(res->rsName);

		char* rsBin = toBinary(res->rs, 5);
		res->RS = calloc(6, sizeof(char));
		strcpy(res->RS, rsBin);
		free(rsBin);	

		res->Imm = imm;
		char* immBin = toBinary(imm, 16);
		strcpy(res->IMM, immBin);
		free(immBin);

		free(arg2);
      free(temp);
	}
   /*else if( strcmp(mnem, "la") == 0 )
   {
      sscanf(res->ASMInstruction, "%*3s %*4s %"SCNd16"", &imm);
		
		res->Imm = imm;
		char* immBin = toBinary(imm, 16);
		strcpy(res->IMM, immBin);
		free(immBin);
		
		res->rs = 0;
		res->RS = calloc(7, sizeof(char));
		strcpy(res->RS, "00000");
   }*/
	//if(strcmp(mnem, "addi") == 0 || strcmp(mnem, "andi") == 0)
   else if(strcmp(mnem, "beq") == 0 || strcmp(mnem, "bne") == 0)
	{
	   char* arg2 = calloc(8,sizeof(char));
      char* temp = calloc(55, sizeof(char));
      strcpy(temp, pASM);
		sscanf(temp, "%*s %*s %s %"SCNd16"", arg2, &imm);
      arg2 = strtok(arg2, ",");
      


      res->rsName = calloc(7, sizeof(char));
		strncpy(res->rsName, res->rtName, 5);
		res->rs = res->rt;
      res->RS = calloc(6, sizeof(char));
      strncpy(res->RS, res->RT, 5);

		/*char* rsBin = toBinary(res->rs, 5);
		res->RS = calloc(6, sizeof(char));
		strncpy(res->RS, rsBin, 5);
		free(rsBin);*/


      strncpy(res->rtName, arg2, 5);
   	res->rt = findRegister(arg2);
   	char* rtBin = toBinary(res->rt, 5);
   	strncpy(res->RT, rtBin, 5);
   	free(rtBin);


		res->Imm = imm;
		char* immBin = toBinary(imm, 16);
		strcpy(res->IMM, immBin);
		free(immBin);

		free(arg2);
      free(temp);
	}
   else if(strcmp(mnem, "blez") == 0 || strcmp(mnem, "bgtz") == 0)
   {
		sscanf(res->ASMInstruction, "%*4s %*4s %"SCNd16"", &imm);
		
		res->Imm = imm;
		char* immBin = toBinary(imm, 16);
		strcpy(res->IMM, immBin);
		free(immBin);
		
      res->rsName = calloc(7, sizeof(char));
		strncpy(res->rsName, res->rtName, 5);
		res->rs = res->rt;
      res->RS = calloc(6, sizeof(char));
      strncpy(res->RS, res->RT, 5);

		res->rt = 0;
		strcpy(res->RT, "00000");

      printf("%s %s %s\n", mnem, res->RT, res->IMM);

   }
   else
   {
	   char* arg2 = calloc(5,sizeof(char));
      char* temp = calloc(55, sizeof(char));
      strcpy(temp, pASM);
		sscanf(temp, "%*s %*s %s %"SCNd16"", arg2, &imm);
      printf("INST: %s\n", temp);
      arg2 = strtok(arg2, ",");

		res->rsName = calloc(5, sizeof(char));
		strcpy(res->rsName, arg2);
		res->rs = findRegister(res->rsName);

		char* rsBin = toBinary(res->rs, 5);
		res->RS = calloc(6, sizeof(char));
		strncpy(res->RS, rsBin, 5);
		free(rsBin);

		res->Imm = imm;
		char* immBin = toBinary(imm, 16);
		strcpy(res->IMM, immBin);
      printf("%d -> %s\n", imm, immBin);
		free(immBin);

		free(arg2);
      free(temp);
   }

   //Build the machine code instruction
   char* machine = calloc(33, sizeof(char));
   strncat(machine, res->Opcode, 6);
   strncat(machine, res->RS, 5); 
   strncat(machine, res->RT, 5);
   strncat(machine, res->IMM, 16);
   strncpy(res->Machine, machine, 32);

   free(machine);
	free(mnem);
	free(arg1);

	return res;
}

static ParseResult* parseJump(const char* const pASM)
{
	ParseResult* res = malloc(sizeof(ParseResult));

	res->ASMInstruction = calloc(50, sizeof(char));
   strcpy(res->ASMInstruction, pASM);
	char* mnem = calloc(6,sizeof(char));
	char* target = calloc(27,sizeof(char));
	sscanf(pASM, "%s %s ", mnem, target);
   printf("\t%s %s\n", mnem, target);

	//Set all fields to default unused value, then fill in the fields that we use as
	//we go along, thus in the end only the unused fields will still have the default value
	res->Mnemonic = calloc(6, sizeof(char));
	res->rdName = NULL;
	res->rsName = NULL;
	res->rtName = NULL;
	res->Imm = 0;
	res->rd = 255;
	res->rs = 255;
	res->rt = 255;
   res->shamt = 255;
	res->Opcode = calloc(7, sizeof(char));
	res->Funct = NULL;
   res->Shamt = NULL;
	res->RD = NULL;
	res->RS = NULL;
	res->RT = NULL;
	res->IMM = NULL;
   res->Machine = calloc(33, sizeof(char));

	strcpy(res->Mnemonic, mnem);
	strcpy(res->Opcode, findOpcode(mnem));

   char* targetBin = toBinary(atoi(target), 26);

   //Build the machine code instruction
   char* machine = calloc(33, sizeof(char));
   strncat(machine, res->Opcode, 6);
   strncat(machine, targetBin, 26);

   strncpy(res->Machine, machine, 32);

   free(machine);
	free(mnem);
	free(target);

   return res;
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

static uint8_t findRegister(char* rName)
{
	for(uint8_t i = 0; i < NUM_REGISTERS; i++)
	{
		char* cur = *(registerTable+i);
		if(strcmp(rName, cur) == 0)
		{
			return i;
		}
	}

	return 255;
}

static char* findOpcode(char* inst)
{
	for(int i = 0; i < NUM_INSTRUCTIONS; i++)
	{
		char* cur = (*(mipsTable+i)).mnemonic;
		if(strcmp(inst, cur) == 0)
		{
			return (*(mipsTable+i)).opcode;
		}
	}

	return NULL;
}

static char* findFunct(char* inst)
{
	for(int i = 0; i < NUM_INSTRUCTIONS; i++)
	{
		char* cur = (*(mipsTable+i)).mnemonic;
		if(strcmp(inst, cur) == 0)
		{
			return (*(mipsTable+i)).funct;
		}
	}

	return NULL;
}

bool isInstruction(char* s)
{
	for(int i = 0; i < NUM_INSTRUCTIONS; i++)
	{
		char* cur = (*(mipsTable+i)).mnemonic;
      if(cur)
      {
         if(strcmp(s, cur) == 0)
         {
            return true;
         }
      }
	}
	return false;
}

bool isLabelInstruction(char* s)
{
   for(int i = 0; i < NUM_INSTRUCTIONS; i++)
	{
		char* cur = (*(mipsTable+i)).mnemonic;
      bool curHasLabel = (*(mipsTable+i)).hasLabel;

      if(cur)
      {
         if(strcmp(s, cur) == 0)
         {
            return curHasLabel;
         }
      }
	}

   return false;
}
