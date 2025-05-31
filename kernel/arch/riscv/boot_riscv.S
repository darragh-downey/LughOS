/* RISC-V Boot Code for LughOS */

.section .text
.global _start
.global _riscv_boot

/* External declaration for early debug function */
.extern riscv_early_boot_debug

_start:
    /* Setup the early stack pointer */
    la sp, _stack_top

    /* Clear BSS */
    la t0, __bss_start
    la t1, __bss_end
1:
    beq t0, t1, 2f
    sw zero, 0(t0)
    addi t0, t0, 4
    j 1b

2:
    /* Call early debug to help troubleshoot boot process */
    call riscv_early_boot_debug
    
    /* Jump to C kernel entry point */
    call kmain

    /* Should never return, but if it does, halt */
3:  wfi
    j 3b
