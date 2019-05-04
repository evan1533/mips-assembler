# adder.asm
#
# The following program computes the sum of all elements in an array.
# The program then prints the sum of all the entries in the array.

.data
message:    .asciiz "The sum of the numbers in the array is: "
values:     .word    2, 3, 5, 7, 11, 13, 17, 19, 23, 29  # array of 10 words
num_values: .word    10    # size of array
sum:        .word    0     # running total

.text
main:
		la    $s0, values       # load address of the array;
										# $s0 will point to the current element
		la    $s1, num_values   # load address of num_values
		lw    $s1, 0($s1)       # load num_values
		sll   $s1, $s1, 2       # compute size of array in bytes
		add   $s1, $s0, $s1     # compute one-past-end address
		li    $s2, 0            # set running total to 0

		j     chk               # check for empty array
loop:
		lw    $s3, 0($s0)       # fetch current data element
		add   $s2, $s2, $s3     # update running total
		addi  $s0, $s0, 4       # step pointer to next array element
chk:
		sub   $t0, $s0, $s1     # compute distance beyond one-past-end
		blez  $t0, loop         # if ( distance <= 0 ) then loop

		# done processing array elements; store total to memory
		la    $s4, sum          # get address for sum
		sw    $s2, 0($s4)       # write sum to memory

		# report results to user
		li    $v0, 4            # system call to print string
		la    $a0, message      # load address of message into arg register
		syscall                 # make call

		li    $v0, 1            # system call to print an integer
		and   $a0, $s2, $s2     # load value to print into arg register
		syscall                 # make call

		li    $v0, 10           # system call to terminate program
		syscall                 # make call
