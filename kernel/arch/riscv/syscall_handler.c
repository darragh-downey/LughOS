#include "lugh.h"
#include "console.h"
#include "syscall.h"

/* C handler for RISC-V syscalls, called from assembly */
void handle_syscall_c(uint32_t num, uint64_t arg1, uint64_t arg2, uint64_t arg3) {
    // Convert to 32-bit for compatibility with existing handler
    // Note that pointers need special care in 64-bit mode
    syscall_handler(num, (uint32_t)arg1, (uint32_t)arg2, (uint32_t)arg3);
}
