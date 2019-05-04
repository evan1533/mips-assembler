import sys


class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'
    BR_YELLOW = '\033[93m'


if len(sys.argv) == 1:
    print("Please specify a file")
    exit(1)

infile = open(sys.argv[1])

lines = infile.readlines()
address = 0
for line in lines:
    if len(line) == 1 and line[0] == '\n':
        address = 0x2000
        print()
        continue
    print((bcolors.BR_YELLOW + "0x%08X: " + bcolors.ENDC + "%s") % (address, line), end='')
    address += 4


print(bcolors.ENDC)
