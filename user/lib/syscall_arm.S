/* ARM syscall implementation for user space
 *
 * Handles the syscall interface for user programs
 */

    .section .text
    .global syscall
    .type syscall, %function
    .thumb
    .syntax unified

syscall:
    /* Get arguments from registers:
     * r0 = syscall number
     * r1 = arg1
     * r2 = arg2
     * r3 = arg3
     */
    push {r4-r7, lr}     /* Save callee-saved registers */
    
    /* Move syscall number to r7 (per ARM EABI) */
    movs r7, r0
    movs r0, r1           /* arg1 */
    movs r1, r2           /* arg2 */
    movs r2, r3           /* arg3 */
    
    svc #0               /* Trigger syscall */
    
    pop {r4-r7, pc}      /* Restore registers and return */
