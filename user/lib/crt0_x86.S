/* x86 C runtime initialization for LughOS user programs
 * This is the entry point for all user mode programs
 */

    .section .text
    .global _start
    .type _start, @function

_start:
    /* Clear frame pointer for stack traces */
    xorl %ebp, %ebp
    
    /* Set up the stack - argc, argv would normally go here */
    movl $0, %esi          /* argc = 0 */
    movl $0, %edi          /* argv = NULL */
    
    /* Prepare arguments and call main */
    push %edi              /* argv */
    push %esi              /* argc */
    call main
    
    /* Exit the program with main's return value */
    pushl $0               /* arg3 = 0 */
    pushl $0               /* arg2 = 0 */
    pushl %eax             /* arg1 = exit code (main's return value) */
    pushl $3               /* SYS_EXIT */
    call syscall
    
    /* Should never reach here, but just in case */
1:  hlt
    jmp 1b
