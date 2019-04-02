#include <inttypes.h>  // I used functions from these Standard modules in
#include <string.h>    // my solution; you may or may not need all of them,
#include <stdlib.h>    // and you might need additional ones, depending on
#include <stdio.h>     // your approach to the problem.
#include <assert.h>

#include "ASMInstruction.h"
#include "ASMParser.h"

#define NUM_REGISTERS 32
#define NUM_INSTRUCTIONS 7
 
static char* registerTable[NUM_REGISTERS] = {
	"zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3",
	 "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7",
	 "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7",
	 "$t8", "$t9", "$k0", "$k1", "$gp", "$sp", "$fp", "$ra"};
static char* instructionTable[NUM_INSTRUCTIONS] = {"add", "and", "sub", "addi", "andi", "lui", "lw"};
static char* opcodeTable[NUM_INSTRUCTIONS] = {
	"000000", "000000", "000000", "001000", "001100", "001111", "100011"};
static char* functionTable[NUM_INSTRUCTIONS] = {
	"100000", "100100", "100010", NULL, NULL, NULL};

static ASMInstruction instrTable[NUM_INSTRUCTIONS] = {
   {  "add", "000000", "100000"},
   {  "and", "000000", "100100"}, 
   {  "sub", "000000", "100010"},
   { "addi", "000000",     NULL},
   { "andi", "000000",     NULL},
   {  "lui", "000000",     NULL},
   {   "lw", "000000",     NULL} };

static uint8_t findRegister(char* rName);
static char* findOpcode(char* inst);
static char* findFunct(char* inst);
static ParseResult* parseRType(const char* const pASM);
static ParseResult* parseIType(const char* const pASM);
static char* toBinary(int num, int size);

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
   char* mnem = calloc(5, sizeof(char));
	char* temp = calloc(50, sizeof(char));
	strcpy(temp, pASM);
	sscanf(temp,"%s", mnem);
	char* opcode = findOpcode(mnem);

   free(mnem);
   free(temp);

	if(strcmp(opcode,"000000") == 0)
	{
		return parseRType(pASM);
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
	char* mnem = calloc(5,sizeof(char));
	char* arg1 = calloc(10,sizeof(char));
	char* arg2 = calloc(10,sizeof(char));
	char* arg3 = calloc(10,sizeof(char));

	res->Mnemonic = calloc(5, sizeof(char));
	res->rdName = calloc(4, sizeof(char*));
	res->rsName = calloc(4, sizeof(char*));
	res->rtName = calloc(4, sizeof(char*));
	res->Imm = 0;
	res->rd = 255;
	res->rs = 255;
	res->rt = 255;
	res->Opcode = calloc(7, sizeof(char*));
	res->Funct = calloc(7, sizeof(char*));
	res->RD = calloc(6, sizeof(char*));
	res->RS = calloc(6, sizeof(char*));
	res->RT = calloc(6, sizeof(char*));
	res->IMM = NULL;
	
	sscanf(pASM,"%3s %3s%*c %3s%*c %3s", mnem, arg1, arg2, arg3);

   strcpy(res->Mnemonic, mnem);
	strcpy(res->Opcode, "000000");
	strcpy(res->Funct, findFunct(mnem));
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
	char* mnem = calloc(5,sizeof(char));
	char* arg1 = calloc(10,sizeof(char));
	int16_t imm = 0;
	sscanf(pASM, "%s %3s", mnem, arg1);

	//Set all fields to default unused value, then fill in the fields that we use as
	//we go along, thus in the end only the unused fields will still have the default value
	res->Mnemonic = calloc(6, sizeof(char));
	res->rdName = NULL;
	res->rsName = NULL;
	res->rtName = calloc(5, sizeof(char));
	res->Imm = 0;
	res->rd = 255;
	res->rs = 255;
	res->rt = 255;
	res->Opcode = calloc(7, sizeof(char));
	res->Funct = NULL;
	res->RD = NULL;
	res->RS = NULL;
	res->RT = calloc(6, sizeof(char));
	res->IMM = calloc(17, sizeof(char));

	strcpy(res->Mnemonic, mnem);
	strcpy(res->Opcode, findOpcode(mnem));
	strcpy(res->rtName, arg1);
	res->rt = findRegister(arg1);
	char* rtBin = toBinary(res->rt, 5);
	strcpy(res->RT, rtBin);
	free(rtBin);

	if(strcmp(mnem, "addi") == 0 || strcmp(mnem, "andi") == 0)
	{
	   char* arg2 = calloc(5,sizeof(char));
      char* temp = calloc(55, sizeof(char));
      strcpy(temp, pASM);
		sscanf(temp, "%*4s %*4s %3s%*1c %"SCNd16"", arg2, &imm);

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
	else if(strcmp(mnem, "lui") == 0)
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
	else if(strcmp(mnem, "lw") == 0)
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

	free(mnem);
	free(arg1);

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
		char* cur = *(instructionTable+i);
		if(strcmp(inst, cur) == 0)
		{
			return *(opcodeTable+i);
		}
	}

	return NULL;
}

static char* findFunct(char* inst)
{
	for(int i = 0; i < NUM_INSTRUCTIONS; i++)
	{
		char* cur = *(instructionTable+i);
		if(strcmp(inst, cur) == 0)
		{
			return *(functionTable+i);
		}
	}

	return NULL;
}
