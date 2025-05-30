/* RISC-V User Mode Program Entry Point for LughOS */

.section .text.entry
.global _start
_start:
    /* Initialize stack pointer */
    la sp, _stack_top
    
    /* Call C main function */
    call main
    
    /* Call exit syscall when main returns */
    li a0, 0        /* Exit code */
    li a7, 1        /* Exit syscall number */
    ecall
    
    /* Should not reach here, but in case we do, loop forever */
1:  j 1b
