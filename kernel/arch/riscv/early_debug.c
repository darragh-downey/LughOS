#include "lugh.h"
#include "console.h"

// RISC-V Early Boot Debug
// This file provides functions to debug early boot process

// UART address (NS16550 compatible) for early debug
#define EARLY_UART_BASE 0x10000000
#define EARLY_UART_THR  0x00  // Transmit Hold Register
#define EARLY_UART_LSR  0x05  // Line Status Register
#define EARLY_UART_LSR_TX_EMPTY 0x20  // Transmitter empty bit

// Very early debug function to send a character to UART directly
static inline void early_uart_putc(char c) {
    volatile uint8_t* uart_lsr = (volatile uint8_t*)(EARLY_UART_BASE + EARLY_UART_LSR);
    volatile uint8_t* uart_thr = (volatile uint8_t*)(EARLY_UART_BASE + EARLY_UART_THR);
    
    // Wait for transmitter to be empty
    while ((*uart_lsr & EARLY_UART_LSR_TX_EMPTY) == 0) {
        // Spin
    }
    
    // Send character
    *uart_thr = c;
}

// Early debug messages
void early_debug_print(const char* str) {
    while (*str) {
        early_uart_putc(*str++);
    }
}

// Print a hexadecimal value
void early_debug_print_hex(uint64_t val) {
    const char hex_chars[] = "0123456789ABCDEF";
    char buffer[19] = "0x";
    
    // Build the hex string
    for (int i = 15; i >= 0; i--) {
        buffer[i + 2] = hex_chars[val & 0xF];
        val >>= 4;
    }
    buffer[18] = '\0';
    
    early_debug_print(buffer);
}

// Call this to print kernel status at boot time
void riscv_early_boot_debug(void) {
    early_debug_print("\r\n[RISCV] LughOS Boot Debug\r\n");
    early_debug_print("[RISCV] Entering kernel main function\r\n");
    
    // Check if we can access the UART for regular console
    early_debug_print("[RISCV] Testing UART access at 0x10000000: ");
    volatile uint8_t* uart_test = (volatile uint8_t*)EARLY_UART_BASE;
    early_debug_print_hex((uint64_t)(uintptr_t)uart_test);
    early_debug_print("\r\n");
}
