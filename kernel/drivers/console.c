#include "lugh.h"
#include "console.h"

// UART definitions
#define SERIAL_PORT 0x3F8  // x86 serial port
#define UART0_BASE 0x10000000  // RISC-V UART0 base address

// RISC-V UART registers
#define UART_RBR 0x00
#define UART_THR 0x00
#define UART_IER 0x01
#define UART_FCR 0x02
#define UART_LCR 0x03
#define UART_MCR 0x04
#define UART_LSR 0x05
#define LSR_TX_EMPTY 0x20

// MMIO operations for RISC-V
#ifdef __riscv
static inline void mmio_write(uintptr_t addr, uint8_t value) {
    *((volatile uint8_t *)addr) = value;
}

static inline uint8_t mmio_read(uintptr_t addr) {
    return *((volatile uint8_t *)addr);
}
#endif

/**
 * Initialize the console for the appropriate architecture
 */
void console_init(void) {
#ifdef __i386__
    // x86 serial port initialization
    outb(SERIAL_PORT + 1, 0x00);    // Disable interrupts
    outb(SERIAL_PORT + 3, 0x80);    // Enable DLAB
    outb(SERIAL_PORT + 0, 0x03);    // Set divisor to 3 (38400 baud)
    outb(SERIAL_PORT + 1, 0x00);    // High byte
    outb(SERIAL_PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
    outb(SERIAL_PORT + 2, 0xC7);    // Enable FIFO, clear them, 14-byte threshold
    outb(SERIAL_PORT + 4, 0x0B);    // IRQs enabled, RTS/DSR set
#elif defined(__riscv)
    // RISC-V UART initialization
    mmio_write(UART0_BASE + UART_IER, 0x00);  // Disable interrupts
    mmio_write(UART0_BASE + UART_LCR, 0x80);  // Enable DLAB
    mmio_write(UART0_BASE + UART_RBR, 0x03);  // Set divisor to 3 (38400 baud)
    mmio_write(UART0_BASE + UART_IER, 0x00);  // Divisor high byte
    mmio_write(UART0_BASE + UART_LCR, 0x03);  // 8 bits, no parity, 1 stop bit
    mmio_write(UART0_BASE + UART_FCR, 0x07);  // Enable FIFO, clear, 14-byte threshold
    mmio_write(UART0_BASE + UART_MCR, 0x03);  // DTR, RTS
#elif defined(__arm__)
    // ARM UART initialization (placeholder)
#endif
}

/**
 * Output a character to the console
 */
void console_putchar(char c) {
#ifdef __i386__
    // x86 serial port output
    while ((inb(SERIAL_PORT + 5) & 0x20) == 0);
    outb(SERIAL_PORT, (uint8_t)c);
#elif defined(__riscv)
    // RISC-V UART output
    while ((mmio_read(UART0_BASE + UART_LSR) & LSR_TX_EMPTY) == 0);
    mmio_write(UART0_BASE + UART_THR, c);
#elif defined(__arm__)
    // Simple ARM UART implementation
    volatile uint32_t *uart0 = (volatile uint32_t *)0x101f1000;
    *uart0 = c;
#endif
}

/**
 * Write a null-terminated string to the console
 */
void console_puts(const char *str) {
    if (str == NULL) {
        return;
    }
    
    for (int i = 0; str[i] != '\0'; i++) {
        console_putchar(str[i]);
    }
}

/**
 * Write a buffer of fixed length to the console
 */
void console_write(const char *buf, size_t len) {
    if (buf == NULL) {
        return;
    }
    
    for (size_t i = 0; i < len; i++) {
        console_putchar(buf[i]);
    }
}
