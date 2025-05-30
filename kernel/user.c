#include "lugh.h"
#include "console.h"

/**
 * Switch to user mode with proper validation
 * 
 * Validates that user_eip and user_esp are valid before
 * performing the user mode transition.
 * 
 * @param user_eip User program entry point
 * @param user_esp User stack pointer
 * 
 * Complies with:
 * - SEI CERT EXP34-C: Do not dereference null pointers
 * - JPL Rule 15: Validate parameters before use
 * - NASA Rule 10: Control flow is simple
 */
void switch_to_user_mode(uint32_t user_eip, uint32_t user_esp) {
    // Validate parameters (SEI CERT EXP34-C, JPL Rule 15)
    if (user_eip == 0) {
        log_message(LOG_ERROR, "Invalid user mode entry point");
        return;
    }
    
    if (user_esp == 0) {
        log_message(LOG_ERROR, "Invalid user mode stack pointer");
        return;
    }
    
    // Validate user address ranges (0x400000-0x7FFFFFFF)
    // Standard user space range based on linker script
    if (user_eip < 0x400000 || user_eip > 0x7FFFFFFF) {
        log_message(LOG_ERROR, "User entry point outside valid range");
        return;
    }
    
    if (user_esp < 0x400000 || user_esp > 0x7FFFFFFF) {
        log_message(LOG_ERROR, "User stack pointer outside valid range");
        return;
    }
    
    log_message(LOG_INFO, "Switching to user mode: eip=0x%x, esp=0x%x", (unsigned int)user_eip, (unsigned int)user_esp);
    
    // Transition to user mode (will not return)
    enter_user_mode(user_eip, user_esp);
}

/**
 * Load a user program from a binary image
 * 
 * @param binary_ptr Pointer to the user binary in memory
 * @param size Size of the binary image
 * @param eip Pointer to store the entry point
 * @param esp Pointer to store the stack pointer
 * 
 * @return 0 on success, -1 on failure
 */
int load_user_program(const void* binary_ptr, size_t size, uint32_t* eip, uint32_t* esp) {
    // Validate parameters (SEI CERT EXP34-C, JPL Rule 15)
    if (binary_ptr == NULL || eip == NULL || esp == NULL) {
        log_message(LOG_ERROR, "Invalid parameters to load_user_program");
        return -1;
    }
    
    if (size < 24) { // Minimum size for valid ELF header
        log_message(LOG_ERROR, "Binary too small to be a valid ELF");
        return -1;
    }
    
    // In a real implementation, we would parse the ELF header and load 
    // segments into user memory. For simplicity, we'll just use a fixed
    // address scheme based on our linker scripts.
    
    // NOTE: In a real implementation, we would:
    // 1. Check ELF magic number
    // 2. Verify architecture matches
    // 3. Load each program segment to its proper virtual address
    // 4. Set up appropriate permissions for each segment
    
    // For this simplified implementation:
    // - Copy entire binary to user space starting address (0x400000)
    memcpy((void*)0x400000, binary_ptr, size);
    
    // Set entry point to start of binary
    *eip = 0x400000;  // Entry point from linker script
    
    // Set up user stack at a safe location
    *esp = 0x700000;  // Just a reasonable stack position
    
    log_message(LOG_INFO, "User program loaded: eip=0x%x, esp=0x%x", (unsigned int)*eip, (unsigned int)*esp);
    return 0;
}
