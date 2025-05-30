#include "lugh.h"
#include "console.h"


/**
 * System call handler for LughOS
 * 
 * Handles system calls from user mode programs.
 * 
 * @param num System call number
 * @param arg1 First argument 
 * @param arg2 Second argument
 * @param arg3 Third argument
 * 
 * Complies with:
 * - SEI CERT STR31-C: Guarantee null-terminated strings
 * - JPL Rule 6: Restrict length of data blocks
 * - NASA Rule 1: Simple control flow
 */
void syscall_handler(uint32_t num, uint32_t arg1, uint32_t arg2, uint32_t arg3) {
    switch (num) {
        case SYS_WRITE:
            // Console write system call
            // arg1 = buffer address, arg2 = length
            if (arg1 == 0) {
                log_message(LOG_ERROR, "SYS_WRITE: Null buffer pointer");
                return;
            }
            
            // Validate user pointer (should check if it's in user address space)
            if (arg1 < 0x400000 || arg1 > 0x7FFFFFFF) {
                log_message(LOG_ERROR, "SYS_WRITE: Buffer pointer outside user space");
                return;
            }
            
            // Limit length to prevent buffer overflows (JPL Rule 6)
            if (arg2 > 1024) {
                log_message(LOG_WARNING, "SYS_WRITE: Truncating large write request");
                arg2 = 1024;
            }
            
            // Write to console
            console_write((const char*)arg1, arg2);
            break;
            
        case SYS_IPC_SEND:
            // IPC send system call
            // arg1 = operation, arg2 = message pointer
            if (arg2 == 0) {
                log_message(LOG_ERROR, "SYS_IPC_SEND: Null message pointer");
                return;
            }
            
            // Validate user pointer
            if (arg2 < 0x400000 || arg2 > 0x7FFFFFFF) {
                log_message(LOG_ERROR, "SYS_IPC_SEND: Message pointer outside user space");
                return;
            }
            
            // Process IPC message
            message_t* msg = (message_t*)arg2;
            msg->operation = arg1;  // Set operation from arg1
            
            // Ensure null-termination (SEI CERT STR31-C)
            msg->payload[MAX_MSG_SIZE - 1] = '\0';
            
            // Calculate checksum for message integrity (NASA Power of Ten rule 6)
            msg->checksum = calculate_checksum(msg->payload, strlen(msg->payload));
            
            // Log the IPC message
            log_message(LOG_INFO, "User IPC: op=0x%x, msg=\"%s\"", 
                        msg->operation, msg->payload);
            
            // Here we would actually send the IPC message
            // For now we just log it
            break;
            
        case SYS_EXIT:
            // Exit system call
            // arg1 = exit code
            log_message(LOG_INFO, "User program exited with code: %d", arg1);
            
            // In a real implementation, we would:
            // 1. Clean up resources
            // 2. Return to kernel mode
            // 3. Mark task as terminated
            
            // For now, just hang
            while(1) {
                cpu_idle();
            }
            break;
            
        default:
            log_message(LOG_ERROR, "Unknown syscall: %d", num);
            break;
    }
}
