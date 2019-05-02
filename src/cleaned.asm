.data
x01:    .word    14324143
s01:    .asciiz  "a"
x02:    .word    -323434
s02:    .asciiz  "ab"
x03:    .word    873124
s03:    .asciiz  "abc"
x04:    .word    -1343241
s04:    .asciiz  "abcd"
arr01:  .word    23:6
s05:    .asciiz  "How did that one go?"
arr02:  .word    6, 28, 496, 8128
.text
main:
	 addi $t0, $zero, arr01
	lw $t1, x01($zero)
        sw     $t1, 4($t1)         # store value of x01 into arr01[1]
        lui    $t1, -1             # load 0xFFFF to top half of $t1
label1:
        add    $t4, $t2, $t3
        addi   $s0, $s0, 31214
        addu   $s1, $s1, $s2
        addiu  $s2, $s2, -31214
        and    $s3, $s4, $s5
        andi   $s3, $s4, 8731
label2:
        mul    $t4, $t7, $t9
	 sll $zero, $zero, 0
        nor    $t3, $t6, $t8
        sll    $s2, $s2, 31
        slt    $t3, $s2, $s3
        slti   $t3, $s2, -21497
label3:
        sra    $s0, $s1, 17
        srav   $s7, $s2, $s0
        sub    $t0, $zero, $s1
        beq    $t1, $a0, label1
        blez   $t3, label2
        bgtz   $s7, label3
        bne    $s5, $zero, label2
        j      label1
label4:
	 addu $t3, $zero, $s3
	 slt $at, $t0, $t1
	 bne $at, $zero, label4
	 addiu $t3, $zero, -4123
        syscall
	 addi $t3, $zero, x02
	 addi $t3, $zero, x03
	 addi $t3, $zero, x04
	 addi $t3, $zero, s05
	 addi $t3, $zero, arr02
