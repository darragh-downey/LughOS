/* x86 syscall implementation for LughOS
 * Handles syscalls from user mode via int $0x80
 */

    .section .text
    .global syscall_entry
    .type syscall_entry, @function

syscall_entry:
    /* Save all registers */
    pushl %ebp
    movl %esp, %ebp
    pusha                   /* Push all general purpose registers */
    
    /* Call syscall handler with arguments from registers:
     * eax = syscall number
     * ebx = arg1
     * ecx = arg2 
     * edx = arg3
     */
    pushl %edx              /* Arg 3 */
    pushl %ecx              /* Arg 2 */
    pushl %ebx              /* Arg 1 */
    pushl %eax              /* Syscall number */
    call syscall_handler
    addl $16, %esp          /* Clean up stack after call */
    
    /* Restore registers and return */
    popa
    popl %ebp
    iret                    /* Return from interrupt to user mode */

/* User-callable syscall function */
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
    push %esi
    push %edi
    
    movl 16(%esp), %eax     /* syscall number */
    movl 20(%esp), %ebx     /* arg1 */
    movl 24(%esp), %ecx     /* arg2 */
    movl 28(%esp), %edx     /* arg3 (if present) */
    
    int $0x80               /* Trigger syscall */
    
    pop %edi                /* Restore callee-saved registers */
    pop %esi
    pop %ebx
    ret                     /* Return to caller */
