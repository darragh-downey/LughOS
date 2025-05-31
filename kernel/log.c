#include "lugh.h"
#include "hardware.h"
#include <stdint.h>

// Include debug headers for early boot on RISC-V
#ifdef __riscv
#include "main_debug.h"
#endif

// Function prototypes
static void init_serial(void);
static void serial_write(uint8_t c);
static void kputchar(char c);
static void kprintf(const char* format, va_list args);

// Simple global tick counter for timestamps
static volatile uint32_t log_ticks = 0;

// Call this from your timer interrupt handler to increment
void log_tick(void) {
    log_ticks++;
}

// Initialize serial port for debugging
static void init_serial(void) {
    // COM1 port (0x3F8)
    // Disable interrupts
    outb(0x3F9, 0x00);
    
    // Set baud rate to 38400 baud
    outb(0x3FB, 0x80);    // Enable DLAB
    outb(0x3F8, 0x03);    // Low byte (38400 baud)
    outb(0x3F9, 0x00);    // High byte
    
    // 8 bits, no parity, one stop bit
    outb(0x3FB, 0x03);
    
    // Enable FIFO, clear, 14-byte threshold
    outb(0x3FA, 0xC7);
    
    // Mark data terminal ready, signal request to send
    outb(0x3FC, 0x0B);
}

// Write a single character to the serial port
static void serial_write(uint8_t c) {
    // Wait until transmitter holding register is empty
    while ((inb(0x3FD) & 0x20) == 0) {
        // spin
    }
    // Send the character
    outb(0x3F8, c);
}

// Basic implementation of kernel console output 
// Outputs to both VGA and serial port
static void kputchar(char c) {
    // JPL Rule 15: Validate parameters at start of public functions (should check c is valid ASCII)
    // SEI CERT ARR30-C: Validate all array indices (bounds check for VGA buffer)
    // Initialize serial port if not done yet
    static int initialized = 0;
    if (!initialized) {
        init_serial();
        initialized = 1;
    }
    serial_write((uint8_t)c); // Explicit cast for sign-conversion
    static volatile unsigned short* vga_buffer = (unsigned short*)0xB8000;
    static int position = 0;
    if (c == '\n') {
        position = ((position / 80) + 1) * 80;
        if (position >= 80 * 25) position = 0; // Bounds check
    } else {
        if (position < 80 * 25) {
            vga_buffer[position++] = (unsigned short)(((uint8_t)c) | 0x0700);
        } else {
            position = 0;
        }
    }
}

// Very basic printf-like functionality
static void kprintf(const char* format, va_list args) {
    // JPL Rule 15: Validate parameters at start of public functions (should check format != NULL)
    // SEI CERT STR31-C: Guarantee storage for strings has space for null terminator
    char c;
    while ((c = *format++)) {
        if (c != '%') {
            kputchar(c);
            continue;
        }
        c = *format++;
        if (!c) break;
        switch (c) {
            case 's': {
                const char* s = va_arg(args, const char*);
                if (s) {
                    for (size_t i = 0; s[i] != '\0'; i++) {
                        kputchar(s[i]);
                    }
                }
                break;
            }
            case 'd': {
                int value = va_arg(args, int);
                if (value < 0) {
                    kputchar('-');
                    value = -value;
                }
                char buffer[12];
                int i = 0;
                do {
                    int digit = value % 10;
                    buffer[i++] = (char)('0' + digit);
                    value /= 10;
                } while (value && i < 11);
                while (i > 0) {
                    kputchar(buffer[--i]);
                }
                break;
            }
            case 'u': {
                unsigned int value = va_arg(args, unsigned int);
                char buffer[12];
                int i = 0;
                do {
                    buffer[i++] = (char)('0' + (value % 10));
                    value /= 10;
                } while (value && i < 11);
                while (i > 0) {
                    kputchar(buffer[--i]);
                }
                break;
            }
            case 'x': {
                unsigned int value = va_arg(args, unsigned int);
                char buffer[8];
                int i = 0;
                do {
                    int digit = value & 0xF;
                    buffer[i++] = (char)((digit < 10) ? ('0' + digit) : ('a' + digit - 10));
                    value >>= 4;
                } while (i < 8); // Always print 8 hex digits
                while (i > 0) {
                    kputchar(buffer[--i]);
                }
                break;
            }
            case 'p': {
                unsigned int value = va_arg(args, unsigned int);
                kputchar('0'); kputchar('x');
                char buffer[8];
                int i = 0;
                do {
                    int digit = value & 0xF;
                    buffer[i++] = (char)((digit < 10) ? ('0' + digit) : ('a' + digit - 10));
                    value >>= 4;
                } while (i < 8); // Always print 8 hex digits for pointers
                while (i > 0) {
                    kputchar(buffer[--i]);
                }
                break;
            }
            case '%':
                kputchar('%');
                break;
            default:
                kputchar('%');
                kputchar(c);
        }
    }
}

void log_message(log_level_t level, const char* format, ...) {
    if ((unsigned)level >= (unsigned)LOG_LEVEL_COUNT) return;
    
#ifdef __riscv
    // For RISC-V, provide early debug output
    early_debug_print("[LOG] ");
#endif
    
    // Print timestamp prefix as 8 hex digits, zero-padded
    kputchar('[');
    unsigned int t = log_ticks;
    char buffer[8];
    int i = 0;
    do {
        int digit = t & 0xF;
        buffer[i++] = (char)((digit < 10) ? ('0' + digit) : ('a' + digit - 10));
        t >>= 4;
    } while (i < 8);
    while (i > 0) {
        kputchar(buffer[--i]);
    }
    kputchar(']');
    kputchar(' ');
    va_list args;
    va_start(args, format);
    kprintf(format, args);
    va_end(args);
    kputchar('\n'); // Always end log messages with a newline for clarity
    
#ifdef __riscv
    // Additional newline for RISC-V console readability
    early_debug_print("\r\n");
#endif
}