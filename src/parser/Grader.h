#ifndef GRADER_H
#define GRADER_H
#include <inttypes.h>
#include <stdio.h>
#include "ParseResult.h"

#define PTS_PER_TEST  60

uint32_t scoreResult(FILE* fp, const ParseResult* const stu, const char* const pASM);

#endif
