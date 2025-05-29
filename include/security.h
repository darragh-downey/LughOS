#ifndef SECURITY_H
#define SECURITY_H

#include "lugh.h"

/**
 * Initialize security features of the kernel
 */
void security_init(void);

/**
 * Initialize hardware memory protection
 */
void security_init_memory_protection(void);

/**
 * Verify memory regions for security violations
 * 
 * @return true if memory layout is secure
 */
bool security_verify_memory_layout(void);

/**
 * Sanitize a user-provided buffer
 * Ensures the buffer meets security requirements
 * 
 * @param buffer Pointer to the buffer to sanitize
 * @param size Size of the buffer
 * @return Sanitized buffer (may be a copy if needed)
 */
void* security_sanitize_buffer(void* buffer, size_t size);

/**
 * Validate a memory access to ensure it's within bounds
 * 
 * @param addr Address to check
 * @param size Size of access
 * @param write True if this is a write operation
 * @return true if access is allowed
 */
bool security_validate_memory_access(void* addr, size_t size, bool write);

/**
 * Generate secure random data (for security operations)
 * 
 * @param buffer Buffer to fill with random data
 * @param size Size of data to generate
 * @return true on success
 */
bool security_generate_random(void* buffer, size_t size);

#endif /* SECURITY_H */
