#include "console.h"
#include "lugh.h"
#include "hardware.h"

/**
 * Output a byte to an I/O port
 * 
 * @param port Port number to write to
 * @param val Value to write
 */
void outb(uint16_t port, uint8_t val) {
    asm volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

/**
 * Input a byte from an I/O port
 * 
 * @param port Port number to read from
 * @return Value read from the port
 */
uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

/**
 * Detect and initialize hardware
 * 
 * @return bool True if hardware initialization was successful
 */
bool hw_detect(void) {
    // Placeholder - add actual hardware detection here
    log_message(LOG_INFO, "Performing hardware detection...\n");

    // Implementation would detect CPU type, memory, etc.
    return true;
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