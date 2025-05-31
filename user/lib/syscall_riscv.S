/* RISC-V System Call Interface for LughOS User Mode Programs */

.section .text
.global syscall
.global syscall_0
.global syscall_1
.global syscall_2
.global syscall_3

/* Main syscall function used by the user library */
syscall:
    /* Move arguments to the right registers for RISC-V syscall
     * a0 already contains first argument (syscall number)
     * a1 already contains second argument (arg1)
     * a2 already contains third argument (arg2)
     * a3 already contains fourth argument (arg3)
     * Just need to move syscall number to a7
     */
    mv a7, a0       /* Move syscall number to a7 */
    mv a0, a1       /* Move arg1 to a0 */
    mv a1, a2       /* Move arg2 to a1 */
    mv a2, a3       /* Move arg3 to a2 */
    
    ecall           /* Trigger syscall */
    
    ret             /* Return to caller */

/* Individual syscall handlers for direct use */
syscall_0:
    li a7, 0
    ecall
    ret

syscall_1:
    li a7, 1
    ecall
    ret

syscall_2:
    li a7, 2
    ecall
    ret

syscall_3:
    li a7, 3
    ecall
    ret
