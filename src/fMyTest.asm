# Spring 2019 Full Assembler
# Test file 7
# Tests:  - la and lw
#         - mixed integers and strings in data segment
#         - some alignment padding
# Maximum points:  22.0

.data
str01:  .asciiz  "abc"
values: .word    1, 1, 2, 3, 5, 8, 13, 21, 34
lol_values: .word 15:8
num_vales: .word 9
val01:  .word    839293
str02:  .asciiz  "backward" #this is to test how data comments are handled
val02:  .word    -839293
str03:  .asciiz  "to"
val03:  .word    903923
str04:  .asciiz  "end"
str05:  .asciiz  "Pointers are fun!"
val04:  .word    3137814

.text
main:
         addiu $t0, $t1, 4500
         
         add $t3, $s0, $s1
#        la    $t0, val01
#        la    $t1, val02
         la    $t2, val03
#
         lw    $s0, 0($t0)
#        lw    $s1, 4($t1)
         
#        sw    $s0, -40($t3)
