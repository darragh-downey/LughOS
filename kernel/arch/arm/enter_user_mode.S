/* ARM user mode entry trampoline for LughOS
 * Switches from Supervisor mode (SVC) to User mode using exception return
 * Arguments (per AAPCS):
 *   r0 = user_eip (user entry point, must be valid)
 *   r1 = user_esp (user stack pointer, must be valid)
 * Assumes:
 *   - MMU configured for user address space
 *   - GIC initialized
 *   - Flat memory model (no segment selectors)
 * Caller must validate arguments (SEI CERT EXP34-C, JPL Rule 15)
 */

    .section .text
    .global enter_user_mode
    .type enter_user_mode, %function
    .arch armv7-a
    .syntax unified
    .thumb

enter_user_mode:
    /* Save arguments */
    mov r2, r0               /* r2 = user_eip */
    mov r3, r1               /* r3 = user_esp */

    /* Set up user stack */
    mov sp, r3               /* Set SP to user stack (r3) */

    /* Prepare exception return frame */
    ldr r0, =0x00000010      /* User mode CPSR (0b10000, interrupts enabled) */
    push {r0}                /* Push SPSR (user mode, IF=0) */
    push {r2}                /* Push PC (user_eip) */
    mov lr, #0               /* Clear LR for safety (SEI CERT MSC15-C) */

    /* Switch to user mode via exception return */
    movs pc, lr              /* Restore CPSR from SPSR, PC from stack (JPL Rule 1) */
