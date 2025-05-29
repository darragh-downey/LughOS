#include "console.h"
#include "lugh.h"
#include "hardware.h"

/**
 * Output a byte to an I/O port
 * 
 * @param port Port number to write to
 * @param val Value to write
 */
#ifdef __i386__
void outb(uint16_t port, uint8_t val) {
    asm volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}
#else
// On non-x86 (e.g., ARM), these are not supported. Provide stubs.
void outb(uint16_t port, uint8_t val) {
    (void)port; (void)val;
    // Optionally log or ignore
}

uint8_t inb(uint16_t port) {
    (void)port;
    return 0;
}
#endif

/**
 * Detect and initialize hardware
 * 
 * @return int 1 if hardware initialization was successful, 0 otherwise
 */
int hw_detect(void) {
    // Placeholder - add actual hardware detection here
    log_message(LOG_INFO, "Performing hardware detection...\n");

    // Implementation would detect CPU type, memory, etc.
    return 1;
}

/**
 * Process any pending system events
 */
void process_events(void) {
    // TODO: process interrupts, timers, etc.
}

/**
 * Put the CPU into a lower state until the next event
 */
void cpu_idle(void) {
    // TODO: use architecture-specific instructions to put the CPU into a low-power state
}