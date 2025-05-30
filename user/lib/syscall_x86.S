/* x86 syscall implementation for user space
 * 
 * Handles the syscall interface for user programs
 */

    .section .text
    .global syscall
    .type syscall, @function

syscall:
    /* Get arguments from stack:
     * [esp+4]  = syscall number
     * [esp+8]  = arg1
     * [esp+12] = arg2
     * [esp+16] = arg3
     */
    push %ebx               /* Save callee-saved registers */
    
    movl 8(%esp), %eax      /* syscall number */
    movl 12(%esp), %ebx     /* arg1 */
    movl 16(%esp), %ecx     /* arg2 */
    movl 20(%esp), %edx     /* arg3 */
    
    int $0x80              /* Trigger syscall */
    
    pop %ebx               /* Restore callee-saved registers */
    ret                    /* Return to caller */
