#ifndef MAIN_C_ADDITIONS
#define MAIN_C_ADDITIONS

// Forward declaration for RISC-V early debug functions
#ifdef __riscv
extern void early_debug_print(const char* str);
#endif

#endif // MAIN_C_ADDITIONS
