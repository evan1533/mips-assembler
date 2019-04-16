# the compiler: gcc for C program, define as g++ for C++
CC = gcc

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS = -ggdb3 -Wall -std=c11

CFILES = driver.c ASMParser.c ParseResult.c
HFILES = ASMParser.h ParseResult.h

# the build target executable:
TARGET = assemble

all: $(TARGET)
	$(CC) $(CFLAGS) -o $(TARGET) $(CFILES) $(HFILES)

clean:
	$(RM) $(TARGET)
