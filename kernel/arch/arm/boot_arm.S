.section .text
.global _start

_start:
    ldr sp, =0x100000  @ set stack pointer
    bl kmain          @ call kmain
    b .               @ infinite loop

