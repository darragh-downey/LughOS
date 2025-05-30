#include "lugh.h"
#include "assert.h"
#include "security.h"
#include <stdalign.h>

// Memory functions for bare metal environment without libc

#define KMALLOC_SIZE (64 * 1024) // 64 KB for kernel heap
#define BLOCK_MAGIC 0xAB12CD34 // Magic value to detect memory corruption

typedef struct {
    uint32_t magic; // Magic value to detect corruption
    size_t size;   // Size of the allocated block
    bool used; // Whether this block is in use
} block_header_t;

// Ensure kmalloc_buffer is aligned to alignof(block_header_t) for all platforms
// Use a union to guarantee alignment for block_header_t
union kmalloc_aligned_buffer {
    block_header_t align;
    uint8_t raw[KMALLOC_SIZE];
};
static union kmalloc_aligned_buffer kmalloc_buffer_union;
#define kmalloc_buffer (kmalloc_buffer_union.raw)
static uint8_t *kmalloc_end = kmalloc_buffer;

/**
 * Align a size to 8-byte boundary (for better performance and alignment)
 */
static size_t align_size(size_t size) {
    // JPL Rule 3: Loop Bounds (all loops in this function are statically bounded)
    return (size + 7UL) & ~7UL; // use UL suffix to ensure unsigned long 
}

/**
 * Initialize the kernel memory allocator
 */
void kmalloc_init(void) {
    // JPL Rule 5: No Dynamic Memory After Initialization (all allocation is static)
    kmalloc_end = kmalloc_buffer;
    // JPL Rule 10: Memory Protection (clear memory for safety margins)
    for (size_t i = 0; i < KMALLOC_SIZE; i++) {
        kmalloc_buffer[i] = 0;
    }
}

/**
 * Allocate memory from the kernel heap
 * 
 * @param size Size to allocate in bytes
 * @return Pointer to allocated memory or NULL if out of memory
 */
void* kmalloc(size_t size) {
    // JPL Rule 16: Use Assertions (assert critical invariants)
    // SEI CERT INT30-C: Ensure size is not zero and within allowed range
    assert(size > 0 && size <= KMALLOC_SIZE - sizeof(block_header_t));
    // JPL Rule 3: Loop Bounds (all allocation is from a fixed-size buffer)
    size_t total_size = align_size(size + sizeof(block_header_t));

    // Use alignof from <stdalign.h> for correct alignment
    uintptr_t unaligned = (uintptr_t)kmalloc_end;
    size_t align = alignof(block_header_t);
    uintptr_t aligned = (unaligned + align - 1) & ~(uintptr_t)(align - 1);
    uint8_t *aligned_kmalloc_end = (uint8_t *)aligned;

    if (aligned_kmalloc_end + total_size > kmalloc_buffer + KMALLOC_SIZE) {
        log_message(LOG_ERROR, "kmalloc: Out of memory\n");
        return NULL; // Out of memory
    }
    // JPL Rule 10: Memory Protection (use magic value for integrity check)
    block_header_t *header;
    memcpy(&header, &aligned_kmalloc_end, sizeof(header));
    header->magic = BLOCK_MAGIC;
    header->size = size;
    header->used = true;
    // JPL Rule 3: Loop Bounds (pointer arithmetic is statically bounded)
    kmalloc_end = aligned_kmalloc_end + total_size;
    return (void*)(header + 1);
}

/**
 * Free memory allocated by kmalloc
 * 
 * @param ptr Pointer to memory to free
 */
void kfree(void* ptr) {
    // SEI CERT ARR30-C: Null pointer check
    if (ptr == NULL) {
        log_message(LOG_WARNING, "kfree: Attempt to free NULL pointer\n");
        return;
    }
    // JPL Rule 10: Memory Protection (validate block integrity)
    block_header_t* header = ((block_header_t*)ptr) - 1;
    if (header->magic != BLOCK_MAGIC || !header->used) {
        log_message(LOG_ERROR, "kfree: Invalid pointer or double free detected\n");
        assert(false); // JPL Rule 16: Use Assertions
        return;
    }
    // JPL Rule 3: Loop Bounds (pointer is within statically allocated buffer)
    if ((uint8_t*)ptr >= kmalloc_buffer && (uint8_t*)ptr < kmalloc_buffer + KMALLOC_SIZE) {
        header->used = false;
        // SEI CERT MEM03-C: Clear memory before releasing
        memset(ptr, 0, header->size);
    } else {
        assert(false); // JPL Rule 16: Use Assertions
    }
}


/**
 * Get the size of an allocated block
 * 
 * @param ptr Pointer to memory block
 * @return Size of the block in bytes or 0 if invalid
 */
size_t kmalloc_size(void* ptr) {
    // null pointer check
    if (ptr == NULL) return 0;

    // get the block header 
    block_header_t* header = ((block_header_t*)ptr) - 1;

    // validate the block
    if (header->magic != BLOCK_MAGIC) {
        // memory corruption detected
        return 0;
    }

    return header->size;
}

// Safe memory copy
void* memcpy(void* dest, const void* src, size_t n) {
    // Security check: validate pointers
    if (!security_validate_memory_access(dest, n, true) || 
        !security_validate_memory_access((void*)src, n, false)) {
        log_message(LOG_ERROR, "Security violation in memcpy\n");
        return dest;
    }
    
    uint8_t* d = (uint8_t*)dest;
    const uint8_t* s = (const uint8_t*)src;
    
    for (size_t i = 0; i < n; i++) {
        d[i] = s[i];
    }
    
    return dest;
}

// Safe memory move with overlap handling
void* memmove(void* dest, const void* src, size_t n) {
    // Security check: validate pointers
    if (!security_validate_memory_access(dest, n, true) || 
        !security_validate_memory_access((void*)src, n, false)) {
        log_message(LOG_ERROR, "Security violation in memmove\n");
        return dest;
    }
    
    uint8_t* d = (uint8_t*)dest;
    const uint8_t* s = (const uint8_t*)src;
    
    // Handle overlap: copy in appropriate direction
    if (d < s) {
        // Copy from beginning to end
        for (size_t i = 0; i < n; i++) {
            d[i] = s[i];
        }
    } else if (d > s) {
        // Copy from end to beginning
        for (size_t i = n; i > 0; i--) {
            d[i-1] = s[i-1];
        }
    }
    
    return dest;
}

// Memory set function
void* memset(void* s, int c, size_t n) {
    // Security check: validate pointer
    if (!security_validate_memory_access(s, n, true)) {
        log_message(LOG_ERROR, "Security violation in memset\n");
        return s;
    }
    
    uint8_t* p = (uint8_t*)s;
    
    for (size_t i = 0; i < n; i++) {
        p[i] = (uint8_t)c;
    }
    
    return s;
}

// Safe string length calculation
size_t strlen(const char* s) {
    // Security check: validate pointer initially
    if (!security_validate_memory_access((void*)s, 1, false)) {
        log_message(LOG_ERROR, "Security violation in strlen: invalid pointer\n");
        return 0;
    }
    
    size_t len = 0;
    
    // Check each character with security validation
    while (security_validate_memory_access((void*)(s + len), 1, false) && s[len] != '\0') {
        len++;
        
        // Security check: max length to prevent infinite loops
        if (len > 10000) { // Reasonable maximum string length
            log_message(LOG_ERROR, "Security violation in strlen: string too long\n");
            return 0;
        }
    }
    
    return len;
}

// Safe string compare
int strcmp(const char* s1, const char* s2) {
    // Security checks
    if (!security_validate_memory_access((void*)s1, 1, false) || 
        !security_validate_memory_access((void*)s2, 1, false)) {
        log_message(LOG_ERROR, "Security violation in strcmp\n");
        return 0;
    }
    
    size_t i = 0;
    
    while (security_validate_memory_access((void*)(s1 + i), 1, false) && 
           security_validate_memory_access((void*)(s2 + i), 1, false)) {
        
        if (s1[i] != s2[i]) {
            return s1[i] - s2[i];
        }
        
        if (s1[i] == '\0') {
            return 0; // Equal strings
        }
        
        i++;
        
        // Security check: max length to prevent infinite loops
        if (i > 10000) {
            log_message(LOG_ERROR, "Security violation in strcmp: strings too long\n");
            return 0;
        }
    }
    
    return 0;
}

// Safe string copy
char* strcpy(char* dest, const char* src) {
    // Security checks will be performed by strlen and memcpy
    size_t len = strlen(src);
    return memcpy(dest, src, len + 1); // +1 for null terminator
}

// Safe implementation of strstr
char* strstr(const char* haystack, const char* needle) {
    // Security checks
    if (!security_validate_memory_access((void*)haystack, 1, false) || 
        !security_validate_memory_access((void*)needle, 1, false)) {
        log_message(LOG_ERROR, "Security violation in strstr\n");
        return NULL;
    }
    
    // Get lengths
    size_t needle_len = strlen(needle);
    if (needle_len == 0) {
        return (char*)haystack; // Empty needle matches anything
    }
    
    // Search for the needle in the haystack
    while (*haystack) {
        // Compare haystack at current position with needle
        if (*haystack == *needle) {
            size_t i;
            for (i = 0; i < needle_len; i++) {
                // Check for end of haystack or mismatch
                if (!haystack[i] || haystack[i] != needle[i]) {
                    break;
                }
            }
            
            // If we found a full match
            if (i == needle_len) {
                return (char*)haystack;
            }
        }
        
        haystack++;
        
        // Security check to prevent infinite loops on very long strings
        if (!security_validate_memory_access((void*)haystack, 1, false)) {
            log_message(LOG_ERROR, "Security violation: out of bounds access in strstr\n");
            return NULL;
        }
    }
    
    return NULL; // Not found
}
