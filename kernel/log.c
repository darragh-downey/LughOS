#include "lugh.h"
#include "hardware.h"


// Initialize serial port for debugging
static void init_serial() {
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
static void serial_write(char c) {
    // Wait until transmitter holding register is empty
    while ((inb(0x3FD) & 0x20) == 0);
    
    // Send the character
    outb(0x3F8, c);
}

// Basic implementation of kernel console output 
// Outputs to both VGA and serial port
static void kputchar(char c) {
    // Initialize serial port if not done yet
    static int initialized = 0;
    if (!initialized) {
        init_serial();
        initialized = 1;
    }
    
    // Write to serial port for debugging
    serial_write(c);
    
    // Write to VGA buffer
    static volatile unsigned short* vga_buffer = (unsigned short*)0xB8000;
    static int position = 0;
    
    if (c == '\n') {
        position = (position / 80 + 1) * 80; // Move to next line
    } else {
        vga_buffer[position++] = (unsigned short)c | 0x0700; // White on black
    }
}

// Very basic printf-like functionality
static void kprintf(const char* format, va_list args) {
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
                while (*s) kputchar(*s++);
                break;
            }
            case 'd': {
                int value = va_arg(args, int);
                if (value < 0) {
                    kputchar('-');
                    value = -value;
                }
                
                // Convert to string
                char buffer[12]; // Enough for 32-bit int
                int i = 0;
                do {
                    buffer[i++] = '0' + (value % 10);
                    value /= 10;
                } while (value);
                
                // Print in correct order
                while (i > 0) {
                    kputchar(buffer[--i]);
                }
                break;
            }
            case 'c':
                kputchar(va_arg(args, int));
                break;
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
    if (level < LOG_DEBUG || level >= LOG_LEVEL_COUNT) return; // Invalid log level
    
    // Could add level prefixes here (like [INFO], [ERROR], etc.)
    
    va_list args;
    va_start(args, format);
    kprintf(format, args);
    va_end(args);
}