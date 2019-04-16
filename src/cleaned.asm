








.text
main:
        add   $s3, $s5, $s7
        add   $s4, $s4, $s0
        nor   $s6, $v0, $v1
        nor   $v0, $s3, $s3
        nor   $s3, $s3, $s3

        addi  $s1, $s0, 0
        addi  $s3, $s3, -32768
        addi  $s7, $s6, 32767

        slti  $s5, $s3, 0
        slti  $s3, $v1, -16384
        slti  $s6, $s1, 16323

        syscall
