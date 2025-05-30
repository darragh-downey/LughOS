#include "lugh.h"

/**
 * Initialize the system call interface for ARM
 * 
 * Sets up the SVC handler for system calls via svc #0
 */
void init_syscall_arm(void) {
    log_message(LOG_INFO, "Initializing ARM system call interface\n");

    // In a real implementation, we would:
    // 1. Set up the vector table
    // 2. Register the SVC handler for syscalls
    // 3. Enable proper memory permissions

    // For now, we'll assume the vector table is set up elsewhere
    // and our syscall_entry function is properly registered

    log_message(LOG_INFO, "ARM system call interface initialized\n");
}
