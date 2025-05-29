#include "lugh.h"
#include "hardware.h"


// Function prototypes
static void init_serial(void);
static void serial_write(uint8_t c);
static void kputchar(char c);
static void kprintf(const char* format, va_list args);

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
            case 'c': {
                int ch = va_arg(args, int);
                kputchar((char)ch);
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
    // Fix: log_level_t is unsigned, so level < LOG_DEBUG is always false
    if ((unsigned)level >= (unsigned)LOG_LEVEL_COUNT) return;
    va_list args;
    va_start(args, format);
    kprintf(format, args);
    va_end(args);
}