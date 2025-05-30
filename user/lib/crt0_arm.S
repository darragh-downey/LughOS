/* ARM C runtime initialization for LughOS user programs
 * This is the entry point for all user mode programs
 */

    .section .text
    .global _start
    .type _start, %function
    .arch armv7-a
    .syntax unified
    .thumb

_start:
    /* Clear frame pointer for stack traces */
    mov r7, #0
    
    /* Set up the stack - argc, argv would normally go here */
    mov r0, #0              /* argc = 0 */
    mov r1, #0              /* argv = NULL */
    
    /* Call main */
    bl main
    
    /* Exit the program with main's return value */
    mov r1, r0              /* Move return value to arg1 */
    mov r2, #0              /* arg2 = 0 */
    mov r3, #0              /* arg3 = 0 */
    mov r0, #3              /* SYS_EXIT */
    bl syscall
    
    /* Should never reach here, but just in case */
1:  wfi
    b 1b
