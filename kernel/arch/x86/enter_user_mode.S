/* 
 * x86 user mode entry trampoline for LughOS
 * Switches from kernel (ring 0) to user mode (ring 3) using iret
 * Arguments (stack-based):
 *   [esp+4]  = user_eip (user entry point, must be valid)
 *   [esp+8]  = user_esp (user stack pointer, must be valid)
 * Assumes GDT selectors:
 *   0x18 = user code segment (ring 3)
 *   0x20 = user data segment (ring 3)
 * Caller must validate arguments (SEI CERT EXP34-C, JPL Rule 15)
 */

    .section .text
    .global enter_user_mode
    .type enter_user_mode, @function

enter_user_mode:
    /* Load arguments */
    mov 8(%esp), %eax        /* user_esp */
    mov 4(%esp), %edx        /* user_eip */

    /* Set user data segment selectors (SEI CERT MSC15-C) */
    mov $0x20, %ax           /* user data selector (ring 3) */
    mov %ax, %ds             /* data segment */
    mov %ax, %es             /* extra segment */
    mov %ax, %fs             /* FS segment */
    mov %ax, %gs             /* GS segment */

    /* Set up iret stack frame (JPL Rule 1: predictable execution) */
    pushl $0x20              /* SS: user data segment */
    pushl %eax               /* ESP: user stack pointer */
    pushfl                   /* EFLAGS */
    popl %ecx                /* Modify EFLAGS */
    or $0x200, %ecx          /* Enable interrupts (IF=1) */
    pushl %ecx               /* Push modified EFLAGS */
    pushl $0x18              /* CS: user code segment */
    pushl %edx               /* EIP: user entry point */

    /* Transition to user mode (ring 3) */
    iret                     /* No instructions after iret (SEI CERT MSC15-C) */
