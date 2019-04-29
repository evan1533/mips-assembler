.data
num01:  .word  3029391
.text
main:
        j     tst1
loop1:
        andi  $s0, $s1, 6165
tst1:
        beq   $t0, $t1, loop1
        blez  $t0, end
        j     tst2
loop2:
        slti  $s6, $s7, -199
        nop
        j     tst3
loop3:
        lui   $t0, 4095
tst3:
        bgtz  $s6, loop3
tst2:
        bne   $t2, $t3, loop2
end:
        j     main
