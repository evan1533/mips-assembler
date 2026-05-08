# the compiler: gcc for C program, define as g++ for C++
CC = gcc

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS = -ggdb3 -Wall -std=c99

# the build target executable:
TARGET = driver

all: $(TARGET)
	$(CC) $(CFLAGS) -o $(TARGET) *.c *.h *.o

types.vim: *.[ch]
	ctags --c-kinds=gstu -o- *.[ch] |\
		awk 'BEGIN{printf("syntax keyword Type\t")}\
			{printf("%s ", $$1)}END{print ""}' > $@

clean:
	$(RM) $(TARGET)
