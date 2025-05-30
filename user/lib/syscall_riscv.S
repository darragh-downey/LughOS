/* RISC-V System Call Interface for LughOS User Mode Programs */

.section .text
.global syscall_0
.global syscall_1
.global syscall_2
.global syscall_3

syscall_0:
    li a7, 0
    ecall
    ret

syscall_1:
    li a7, 1
    ecall
    ret

syscall_2:
    li a7, 2
    ecall
    ret

syscall_3:
    li a7, 3
    ecall
    ret
