/* RISC-V System Call Handler for LughOS */

.section .text
.global init_syscall_riscv

/* Setup the system call handler */
init_syscall_riscv:
    /* Set the trap vector */
    la t0, trap_vector
    csrw stvec, t0
    ret

/* Trap vector for handling exceptions and interrupts */
.align 4
trap_vector:
    /* Save registers */
    addi sp, sp, -32*8
    sd ra, 0*8(sp)
    sd t0, 1*8(sp)
    sd t1, 2*8(sp)
    sd t2, 3*8(sp)
    sd a0, 4*8(sp)
    sd a1, 5*8(sp)
    sd a2, 6*8(sp)
    sd a3, 7*8(sp)
    sd a4, 8*8(sp)
    sd a5, 9*8(sp)
    sd a6, 10*8(sp)
    sd a7, 11*8(sp)
    
    /* Check if this is a system call (environment call from U-mode) */
    csrr t0, scause
    li t1, 8          /* Environment call from U-mode cause value */
    beq t0, t1, handle_syscall
    
    /* Handle other exceptions/interrupts */
    j other_trap
    
handle_syscall:
    /* Get syscall number from a7 and dispatch */
    ld a7, 11*8(sp)   /* Restore original a7 value (syscall number) */
    ld a0, 4*8(sp)    /* Restore original a0 value (arg1) */
    ld a1, 5*8(sp)    /* Restore original a1 value (arg2) */
    ld a2, 6*8(sp)    /* Restore original a2 value (arg3) */
    
    /* Call the C handler with args already in registers */
    mv a3, a2         /* Move arg3 to a3 */
    mv a2, a1         /* Move arg2 to a2 */
    mv a1, a0         /* Move arg1 to a1 */
    mv a0, a7         /* Move syscall number to a0 */
    
    call handle_syscall_c
    
    /* Store result back to a0 for user mode */
    sd a0, 4*8(sp)
    
    /* Increment the return address to skip the ecall instruction */
    csrr t0, sepc
    addi t0, t0, 4
    csrw sepc, t0
    
    j trap_exit

other_trap:
    /* Call appropriate handler based on scause */
    /* For now, we just return to user mode */
    
trap_exit:
    /* Restore registers */
    ld ra, 0*8(sp)
    ld t0, 1*8(sp)
    ld t1, 2*8(sp)
    ld t2, 3*8(sp)
    ld a0, 4*8(sp)
    ld a1, 5*8(sp)
    ld a2, 6*8(sp)
    ld a3, 7*8(sp)
    ld a4, 8*8(sp)
    ld a5, 9*8(sp)
    ld a6, 10*8(sp)
    ld a7, 11*8(sp)
    addi sp, sp, 32*8
    
    /* Return to where we came from */
    sret
