.data
val01:  .word  65535
val02:  .word  -4134137
.text
main:
        add  $s0, $s1, $s2
        and  $s3, $s4, $s5
        nor  $s6, $s7, $v0
        addu $v1, $t0, $t1
        slt  $t2, $t3, $t4
        sub  $t5, $t6, $t7
        mul  $at, $k0, $k1
        sra  $t8, $t9, 15
        sll  $a0, $a1, 23
