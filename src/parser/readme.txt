After unpacking the posted tar file, you should find the following files,
organized in two subdirectories:

   dev:
      readme.txt       - this file
      ASMParser.c      - C files for implementing your solution
      ParseResult.c
      driver.c         - C source for the test driver
      ASMParser.h      - supplied C header file 
      ParseResult.h    - supplied C header file
      Grade.h          - declaration for grading function
      Grade.o          - 64-bit CentOS binary for grading function 
      Generate.h       - declaration for test data generator
      Generate.o       - 64-bit CentOS binary for test data generator
      
   autograder:
      gradeC02.sh      - bash script file
      C02Grader.tar    - grading code, including:
         driver.c      - C source for the test driver
         ASMParser.h   - supplied C header file 
         ParseResult.h - supplied C header file
         Grade.h       - declaration for grading function
         Grade.o       - 64-bit CentOS binary for grading function 
         Generate.h    - declaration for test data generator
         Generate.o    - 64-bit CentOS binary for test data generator

Development and Manual Testing in ./dev
----------------------------------------------------------------------------
For development and manual testing (especially if you want to use gdb):
    - work in the dev directory
    - edit your versions of ASMParser.c and ParseResult.c in the dev
      directory, along with any additional C code files you write
    - compile your files with the grading harness:
        gcc -o driver -std=c11 -Wall -W -ggdb3 driver.c ASMParser.c ParseResult.c Generate.o Grade.o
    - execute the test driver:  
        ./driver <name for test data file> <name for results file>

This will create new test data and graded results.  See the results file
for details.  If you want to reuse the last test cases, run the driver
this way:
        ./driver <name for test data file> <name for results file> -repeat


Automated Grading in  ./autograder
----------------------------------------------------------------------------
To use the automated grading package:
    - work in the autograder directory
    - if necessary, run chmod a+x to make the script gradeC02.sh executable
    - create a tar file containing your ASMParser.c and ParseResult.c
      files (and all additional .c and .h files you've created); 
      copy that tar file into the autograder directory
    - execute one of the following commands:
         gradeC02.sh <name of your tar file>
         gradeC02.sh <name of your tar file> -repeat

The grading script will extract the first token from the name of your zip
file and use that in naming the generated report file.  We recommend that
you name your tar file PID.tar, where PID is your VT email PID, since 
that is the naming convention used when you submit your solution to us
for grading.    

Watch the terminal window for any error messages from the script.  Check
the file PID.txt for grading results; if that file was not created (due
to some irrecoverable error), check the other text files created by the
grading process.

If you get an error message about the build failing, then your tar file 
is defective or your source code is defective.  Return to the dev directory
to fix the relevant issues, then try the autograder again.
