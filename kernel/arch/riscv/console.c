#include "lugh.h"
#include "console.h"

// QEMU RISC-V UART address (NS16550 compatible)
#define UART0_BASE 0x10000000

// Register offsets
#define UART_RBR 0x00 // Receive Buffer Register (read only)
#define UART_THR 0x00 // Transmit Holding Register (write only)
#define UART_IER 0x01 // Interrupt Enable Register
#define UART_FCR 0x02 // FIFO Control Register
#define UART_LCR 0x03 // Line Control Register
#define UART_MCR 0x04 // Modem Control Register
#define UART_LSR 0x05 // Line Status Register
#define UART_MSR 0x06 // Modem Status Register
#define UART_SCR 0x07 // Scratch Register

// Line Status Register bits
#define LSR_RX_READY 0x01   // Receiver Ready
#define LSR_TX_EMPTY 0x20   // Transmitter Empty

// Use memory-mapped I/O for RISC-V
static inline void mmio_write(uintptr_t addr, uint8_t value) {
    *((volatile uint8_t *)addr) = value;
}

static inline uint8_t mmio_read(uintptr_t addr) {
    return *((volatile uint8_t *)addr);
}

// Initialize the RISC-V UART
void riscv_console_init(void) {
    // Initialize UART with 38400 8N1
    mmio_write(UART0_BASE + UART_IER, 0x00);  // Disable interrupts
    mmio_write(UART0_BASE + UART_LCR, 0x80);  // Enable DLAB
    mmio_write(UART0_BASE + UART_RBR, 0x03);  // Set divisor to 3 (38400 baud)
    mmio_write(UART0_BASE + UART_IER, 0x00);  // Divisor high byte
    mmio_write(UART0_BASE + UART_LCR, 0x03);  // 8 bits, no parity, 1 stop bit
    mmio_write(UART0_BASE + UART_FCR, 0x07);  // Enable FIFO, clear, 14-byte threshold
    mmio_write(UART0_BASE + UART_MCR, 0x03);  // DTR, RTS
}

// Write a character to RISC-V UART
void riscv_console_putchar(char c) {
    // Wait for transmitter to be empty
    while ((mmio_read(UART0_BASE + UART_LSR) & LSR_TX_EMPTY) == 0) {
        // Spin
    }
    
    // Write the character
    mmio_write(UART0_BASE + UART_THR, c);
}

// Write a string to RISC-V UART
void riscv_console_write(const char *str, size_t len) {
    for (size_t i = 0; i < len; i++) {
        riscv_console_putchar(str[i]);
    }
}
