/**
 * RISC-V Initialization Code for LughOS
 */

#include "lugh.h"
#include "hardware.h"

/**
 * Initialize the RISC-V-specific hardware
 * 
 * @return 1 if successful, 0 if failed
 */
int init_riscv(void) {
    log_message(LOG_INFO, "Initializing RISC-V hardware...\n");
    
    // Initialize RISC-V specific hardware here
    
    return 1;
}

/**
 * Initialize RISC-V system call interface
 * This function only logs the initialization, the actual setup is in syscall.S
 */
void init_syscall_riscv_c(void) {
    log_message(LOG_INFO, "Initializing RISC-V system call interface...\n");
    // The actual initialization happens in syscall.S through init_syscall_riscv
    
    // Call the assembly implementation
    extern void init_syscall_riscv(void);
    init_syscall_riscv();
}
