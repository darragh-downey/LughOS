#include "user.h"

/**
 * Write data to console
 * 
 * @param buf Pointer to data buffer
 * @param len Length of data to write
 */
void write(const char* buf, size_t len) {
#ifdef __riscv
    // Use uintptr_t for RISC-V (64-bit)
    syscall(SYS_WRITE, (uintptr_t)buf, (uint32_t)len, 0);
#else
    // Original 32-bit implementation
    syscall(SYS_WRITE, (uint32_t)buf, len, 0);
#endif
}

/**
 * Calculate string length
 * 
 * @param str String to measure
 * @return Length of string
 */
size_t strlen(const char* str) {
    size_t len = 0;
    while (str[len]) {
        len++;
    }
    return len;
}

/**
 * Print null-terminated string to console
 * 
 * @param str String to print
 */
void print(const char* str) {
    write(str, strlen(str));
}

/**
 * Copy memory region
 * 
 * @param dest Destination buffer
 * @param src Source buffer
 * @param n Number of bytes to copy
 * @return Pointer to destination
 */
void* memcpy(void* dest, const void* src, size_t n) {
    char* d = (char*)dest;
    const char* s = (const char*)src;
    size_t i;
    
    for (i = 0; i < n; i++) {
        d[i] = s[i];
    }
    
    return dest;
}

/**
 * Exit the program
 * 
 * @param code Exit status code
 */
void exit(int code) {
#ifdef __riscv
    // Use proper casting for RISC-V
    syscall(SYS_EXIT, (uint32_t)code, 0, 0);
#else
    // Original implementation
    syscall(SYS_EXIT, (uint32_t)code, 0, 0);
#endif
    // Should not return
    while (1) {}
}

/**
 * Send an IPC message
 * 
 * @param operation Operation code (OP_*)
 * @param message Message string
 * @return 0 on success, -1 on error
 */
int send_ipc(uint32_t operation, const char* message) {
    message_t msg;
    
    // Initialize message
    msg.priority = PRIORITY_MEDIUM;
    
    // Copy message payload safely
    const size_t len = strlen(message);
    const size_t safe_len = (len >= MAX_MSG_SIZE) ? MAX_MSG_SIZE - 1 : len;
    
    memcpy(msg.payload, message, safe_len);
    msg.payload[safe_len] = '\0';
    
    // Checksum will be calculated by kernel
    
    // Send message
#ifdef __riscv
    // Use uintptr_t for RISC-V (64-bit)
    syscall(SYS_IPC_SEND, operation, (uintptr_t)&msg, 0);
#else
    // Original 32-bit implementation
    syscall(SYS_IPC_SEND, operation, (uint32_t)&msg, 0);
#endif
    
    return 0;
}

/**
 * Send a grid alert message
 * 
 * @param message Alert message string
 * @return 0 on success, -1 on error
 */
int send_grid_alert(const char* message) {
    return send_ipc(OP_GRID_ALERT, message);
}
