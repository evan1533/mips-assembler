.data
str01:  .asciiz  "abc"
str02:  .asciiz  "back"
str03:  .asciiz  "to"
str04:  .asciiz  "end"
str05:  .asciiz  "Assembling machine code is fun, and good for you!"
.text
main:
	 addi $t0, $zero, str01
        la    $t0, str01
        lw    $s0, 0($t0)
