.data
str01:  .asciiz  "abc"
values: .word    1, 1, 2, 3, 5, 8, 13, 21, 34
lol_values: .word 15:8
num_vales: .word 9
val01:  .word    839293
str02:  .asciiz  "backward"
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
