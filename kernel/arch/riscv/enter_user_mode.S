/* RISC-V User Mode Switching Code for LughOS */

.section .text
.global switch_to_user_mode

switch_to_user_mode:
    /* a0 = user program entry point, a1 = user stack pointer */
    
    /* Set USER mode and enable interrupts */
    li t0, 0x100      /* SPIE (allow interrupts in user mode) */
    csrw sstatus, t0
    
    /* Set the user PC */
    csrw sepc, a0
    
    /* Load the user stack */
    mv sp, a1
    
    /* Return to user mode (sret) */
    sret
