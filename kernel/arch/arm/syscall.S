/* ARM syscall implementation for LughOS
 * Handles syscalls from user mode via SVC #0
 */

    .section .text
    .global syscall_entry
    .type syscall_entry, %function
    .arch armv7-a
    .syntax unified
    .thumb

syscall_entry:
    /* Save context */
    push {r4-r11, lr}       /* Save callee-saved registers */
    
    /* Call syscall handler with arguments from registers:
     * r7 = syscall number
     * r0 = arg1
     * r1 = arg2
     * r2 = arg3
     */
    mov r3, r2              /* Arg 3 */
    mov r2, r1              /* Arg 2 */
    mov r1, r0              /* Arg 1 */
    mov r0, r7              /* Syscall number */
    bl syscall_handler
    
    /* Restore context and return */
    pop {r4-r11, lr}
    bx lr                   /* Return from SVC */

/* User-callable syscall function */
    .section .text
    .global syscall
    .type syscall, %function
    .thumb

syscall:
    /* Arguments already in r0-r2, move to: 
     * r7 = syscall number (from r0)
     * r0 = arg1 (from r1)
     * r1 = arg2 (from r2)
     * r2 = arg3 (from r3)
     */
    push {r4-r7, lr}        /* Save callee-saved registers */
    mov r7, r0              /* Move syscall number to r7 */
    mov r0, r1              /* Move arg1 to r0 */
    mov r1, r2              /* Move arg2 to r1 */
    mov r2, r3              /* Move arg3 to r2 */
    
    svc #0                  /* Trigger syscall */
    
    pop {r4-r7, pc}         /* Restore and return */
